use std::cell::RefCell;
use std::rc::Rc;
use js_sys::JsString;
use wasm_bindgen::prelude::*;
use web_sys::{EventListener, MouseEvent, WebGl2RenderingContext, WebGlProgram, WebGlShader, window};

#[derive(Copy, Clone, Debug)]
enum Color {
    RED,
    BLUE,
    GREEN,
    WHITE
}

impl Into<&[f32]> for Color {
    fn into(self) -> &'static [f32] {
        match self {
            Color::RED => &[1.0, 0.0, 0.0, 1.0],
            Color::BLUE => &[0.0, 0.0, 1.0, 1.0],
            Color::GREEN => &[0.0, 1.0, 0.0, 1.0],
            Color::WHITE => &[1.0, 1.0, 1.0, 1.0]
        }
    }
}

impl From<(f32, f32)> for Color {
    fn from(value: (f32, f32)) -> Self {
        if value.0 <= 0.0 {
            if value.1 <= 0.0 {
                Self::GREEN
            } else {
                Self::BLUE
            }
        } else {
            if value.1 <= 0.0 {
                Self::WHITE
            } else {
                Self::RED
            }
        }
    }
}

#[wasm_bindgen(start)]
fn start() -> Result<(), JsValue> {
    let document = web_sys::window().unwrap().document().unwrap();
    let canvas = document.get_element_by_id("canvas").unwrap();
    let canvas: web_sys::HtmlCanvasElement = canvas.dyn_into::<web_sys::HtmlCanvasElement>()?;

    let context = canvas
        .get_context("webgl2")?
        .unwrap()
        .dyn_into::<WebGl2RenderingContext>()?;

    let vert_shader = compile_shader(
        &context,
        WebGl2RenderingContext::VERTEX_SHADER,
        r##"#version 300 es
        uniform vec4 position;
        void main() {
            gl_Position = position;
            gl_PointSize = 10.0;
        }
        "##,
    )?;

    let frag_shader = compile_shader(
        &context,
        WebGl2RenderingContext::FRAGMENT_SHADER,
        r##"#version 300 es
        precision lowp float;
        uniform vec4 color;
        layout(location = 0) out vec4 out_color;
        void main() {
            out_color = color;
        }
        "##,
    )?;
    let program = link_program(&context, &vert_shader, &frag_shader)?;
    context.use_program(Some(&program));
    let color_attr = context.get_uniform_location(&program, "color").unwrap();
    let pos_attr = context.get_uniform_location(&program, "position").unwrap();

    let mut vec: Rc<RefCell<Vec<(f32, f32, Color)>>> = Rc::new(RefCell::new(vec![]));

    let closure = Closure::<dyn Fn(_)>::new(move |input_event: MouseEvent| {
        let context = canvas
            .get_context("webgl2").unwrap()
            .unwrap()
            .dyn_into::<WebGl2RenderingContext>().unwrap();
        let points = ((input_event.offset_x() - 200) as f32 / 200f32, -((input_event.offset_y() - 200) as f32 / 200f32));
        vec.borrow_mut().push((points.0, points.1, Color::from(points)));
        context.clear(WebGl2RenderingContext::COLOR_BUFFER_BIT);
        for v in vec.borrow().iter() {
            let v = *v;
            context.uniform4f(Some(&pos_attr), v.0, v.1, 0f32, 1.0f32);
            context.uniform4fv_with_f32_array(Some(&color_attr), Into::<&[f32]>::into(v.2));
            context.draw_arrays(WebGl2RenderingContext::POINTS, 0, 1);
        }
    });

    context.clear_color(0.0, 0.0, 0.0, 1.0);
    context.clear(WebGl2RenderingContext::COLOR_BUFFER_BIT);

    document.get_element_by_id("canvas").unwrap().add_event_listener_with_callback("mousedown", closure.as_ref().unchecked_ref())?;

    closure.forget();

    Ok(())
}

pub fn compile_shader(
    context: &WebGl2RenderingContext,
    shader_type: u32,
    source: &str,
) -> Result<WebGlShader, String> {
    let shader = context
        .create_shader(shader_type)
        .ok_or_else(|| String::from("Unable to create shader object"))?;
    context.shader_source(&shader, source);
    context.compile_shader(&shader);

    if context
        .get_shader_parameter(&shader, WebGl2RenderingContext::COMPILE_STATUS)
        .as_bool()
        .unwrap_or(false)
    {
        Ok(shader)
    } else {
        Err(context
            .get_shader_info_log(&shader)
            .unwrap_or_else(|| String::from("Unknown error creating shader")))
    }
}

pub fn link_program(
    context: &WebGl2RenderingContext,
    vert_shader: &WebGlShader,
    frag_shader: &WebGlShader,
) -> Result<WebGlProgram, String> {
    let program = context
        .create_program()
        .ok_or_else(|| String::from("Unable to create shader object"))?;

    context.attach_shader(&program, vert_shader);
    context.attach_shader(&program, frag_shader);
    context.link_program(&program);

    if context
        .get_program_parameter(&program, WebGl2RenderingContext::LINK_STATUS)
        .as_bool()
        .unwrap_or(false)
    {
        Ok(program)
    } else {
        Err(context
            .get_program_info_log(&program)
            .unwrap_or_else(|| String::from("Unknown error creating program object")))
    }
}

