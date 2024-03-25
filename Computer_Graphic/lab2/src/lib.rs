use std::cell::RefCell;
use std::fmt::format;
use std::rc::Rc;
use js_sys::{Float32Array, Float64Array, JsString};
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

fn create_array_buffer_view(buffer: &[f32]) -> Float32Array {
    unsafe {
        Float32Array::view(buffer)
    }
}

fn associate_attribute_pointer(context: &WebGl2RenderingContext, program: &WebGlProgram, size: i32, attribute: &str, stride: i32, offset: i32) {
    let attrib = context.get_attrib_location(&program, attribute);
    context.vertex_attrib_pointer_with_i32(attrib as u32, size, WebGl2RenderingContext::FLOAT, false, stride, offset);
    context.enable_vertex_attrib_array(attrib as u32);
}

#[wasm_bindgen(start)]
fn start() -> Result<(), JsValue> {
    let document = window().unwrap().document().unwrap();
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
        in vec2 position;
        in vec3 color;
        out vec4 var_color;
        void main() {
            gl_Position = vec4(position[0], position[1], 0.0, 1.0);
            var_color = vec4(color[0], color[1], color[2], 1.0);
        }
        "##,
    )?;

    let frag_shader = compile_shader(
        &context,
        WebGl2RenderingContext::FRAGMENT_SHADER,
        r##"#version 300 es
        precision lowp float;
        in vec4 var_color;
        layout(location = 0) out vec4 out_color;
        void main() {
            out_color = var_color;
        }
        "##,
    )?;
    let program = link_program(&context, &vert_shader, &frag_shader)?;
    context.use_program(Some(&program));

    // Create Vertices
    let vertices: [f32; 20] = [-0.5, 0.5, 1.0, 0.0, 0.0
            , -0.5, -0.5, 1.0, 1.0, 1.0
            , 0.5, 0.5, 1.0, 1.0, 1.0
            , 0.5, -0.5, 0.0, 0.0, 1.0];

    let vertices = create_array_buffer_view(&vertices);
    context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, context.create_buffer().as_ref());
    context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &vertices, WebGl2RenderingContext::STATIC_DRAW);

    // Associate attribute
    let f32_size = std::mem::size_of::<f32>() as i32;
    associate_attribute_pointer(&context, &program,  2, "position",  f32_size * 5 , 0);
    associate_attribute_pointer(&context, &program,  3 ,"color", f32_size * 5, f32_size * 2);

    context.clear_color(0.0, 0.0, 0.0, 1.0);
    context.clear(WebGl2RenderingContext::COLOR_BUFFER_BIT);
    context.draw_arrays(WebGl2RenderingContext::TRIANGLE_STRIP, 0, 4);

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

