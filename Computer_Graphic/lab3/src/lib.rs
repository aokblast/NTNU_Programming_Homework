use std::cell::RefCell;
use std::f32::consts::PI;
use std::fmt::format;
use std::rc::Rc;
use gl_matrix::common::{Mat4, Vec3};
use js_sys::{Float32Array, Float64Array, JsString};
use wasm_bindgen::prelude::*;
use web_sys::{EventListener, KeyboardEvent, WebGl2RenderingContext, WebGlBuffer, WebGlProgram, WebGlShader, WebGlUniformLocation, window};
use gl_matrix::mat4;
use gl_matrix::quat2::get_translation;

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

struct TriangleInformation {
    pub vertices: Float32Array,
    pub colors: Float32Array,
    u_matrix: WebGlUniformLocation,
    a_position: u32,
    a_color: u32,
    vert_buffer: WebGlBuffer,
    color_buffer: WebGlBuffer,
    context: WebGl2RenderingContext,
    program: WebGlProgram
}

impl TriangleInformation {
    pub fn new(vert: [f32; 9], color: [f32; 9], context: WebGl2RenderingContext, program: WebGlProgram) -> Self {
        let mut res = Self {
            vertices: create_array_buffer_view(&vert),
            colors: create_array_buffer_view(&color),
            u_matrix: context.get_uniform_location(&program, "u_modelMatrix").unwrap(),
            a_position: context.get_attrib_location(&program, "a_Position") as u32,
            a_color: context.get_attrib_location(&program, "a_Color") as u32,
            vert_buffer: context.create_buffer().unwrap(),
            color_buffer: context.create_buffer().unwrap(),
            context,
            program,
        };

        res.init_bind_buffer();

        res
    }

    fn init_bind_buffer(&self) {
        self.context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&self.vert_buffer));
        self.context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &self.vertices, WebGl2RenderingContext::STATIC_DRAW);

        self.context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&self.color_buffer));
        self.context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &self.colors, WebGl2RenderingContext::STATIC_DRAW);

        self.context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, None);
        self.context.bind_buffer(WebGl2RenderingContext::ELEMENT_ARRAY_BUFFER, None);
    }

    pub fn draw_with_transformation(&self, mat: Mat4) {
        self.context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&self.color_buffer));
        self.context.vertex_attrib_pointer_with_i32(self.a_color, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
        self.context.enable_vertex_attrib_array(self.a_color);
        self.context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&self.vert_buffer));
        self.context.vertex_attrib_pointer_with_i32(self.a_position, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
        self.context.enable_vertex_attrib_array(self.a_position);
        self.context.uniform_matrix4fv_with_f32_array(Some(&self.u_matrix), false, &mat);
        self.context.draw_arrays(WebGl2RenderingContext::TRIANGLES, 0, 3);
    }

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

    let context2 = canvas
        .get_context("webgl2")?
        .unwrap()
        .dyn_into::<WebGl2RenderingContext>()?;

    let vert_shader = compile_shader(
        &context,
        WebGl2RenderingContext::VERTEX_SHADER,
        r##"#version 300 es
        in vec4 a_Position;
        in vec4 a_Color;
        out vec4 v_Color;
        uniform mat4 u_modelMatrix;
        void main(){
            gl_Position = u_modelMatrix * a_Position;
            gl_PointSize = 10.0;
            v_Color = a_Color;
        }
        "##,
    )?;

    let frag_shader = compile_shader(
        &context,
        WebGl2RenderingContext::FRAGMENT_SHADER,
        r##"#version 300 es
        precision lowp float;
        in vec4 v_Color;
        layout(location = 0) out vec4 out_color;
        void main() {
            out_color = v_Color;
        }
        "##,
    )?;
    let program = link_program(&context, &vert_shader, &frag_shader)?;
    context.use_program(Some(&program));

    let triangle_a = TriangleInformation::new([0.0, 0.2, 0.0, -0.1, -0.3, 0.0, 0.1, -0.3, 0.0],
                                              [0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0 ],
                                                context.clone(), program.clone());

    let triangle_b = TriangleInformation::new([0.0, 0.0, 0.0, -0.1, -0.5, 0.0, 0.1, -0.5, 0.0],
                                              [0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0 ],
                                              context.clone(), program.clone());

    let triangle_c = TriangleInformation::new([0.0, 0.0, 0.0, -0.1, -0.5, 0.0, 0.1, -0.5, 0.0],
                                              [0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0 ],
                                              context.clone(), program.clone());

    let mut t1_x_move = Rc::new(RefCell::new(0.0));
    let mut t2_angle = Rc::new(RefCell::new(125.0));;
    let mut t2_height_scale = Rc::new(RefCell::new(1.0));;
    let mut t3_angle = Rc::new(RefCell::new(0.0));

    let mut t1_x_move_c = t1_x_move.clone();
    let mut t2_angle_c = t2_angle.clone();;
    let mut t2_height_scale_c = t2_height_scale.clone();;
    let mut t3_angle_c = t3_angle.clone();

    let draw = move || {
        context2.clear_color(0.0, 0.0, 0.0, 1.0);
        context2.clear(WebGl2RenderingContext::COLOR_BUFFER_BIT);
        let mut mat4 = mat4::create();
        mat4 = mat4::from_translation(&mut mat4, &Vec3::from([*t1_x_move.as_ref().borrow() as f32, 0.0, 0.0]));
        triangle_a.draw_with_transformation(mat4);
        let mut mat42 = mat4::create();
        mat4 = mat4::translate(&mut mat42, &mat4, &Vec3::from([0.0, 0.2, 0.0]));
        mat4 = mat4::rotate(&mut mat42, &mat4, *t2_angle.as_ref().borrow() * PI / 180.0, &Vec3::from([0.0, 0.0, 1.0])).unwrap();
        mat4 = mat4::scale(&mut mat42, &mat4, &Vec3::from([1.0, *t2_height_scale.as_ref().borrow() as f32, 0.0]));
        triangle_b.draw_with_transformation(mat42);
        let mut mat43 = mat4::create();
        mat42 = mat4::translate(&mut mat43, &mat42, &Vec3::from([0.1, -0.5, 0.0]));
        mat42 = mat4::rotate(&mut mat43, &mat42, *t3_angle.as_ref().borrow() * PI / 180.0, &Vec3::from([0.0, 0.0, 1.0])).unwrap();
        mat42 = mat4::scale(&mut mat43, &mat42, &Vec3::from([1.0, 1.0 / *t2_height_scale.as_ref().borrow(), 0.0]));
        triangle_c.draw_with_transformation(mat43);
    };

    draw();

    {
        let closure = Closure::<dyn Fn(_)>::new(move |input_event: KeyboardEvent| {
            let mut key = input_event.key();
            key = key.to_uppercase();

            if key == "A" {
                web_sys::console::log_1(&JsString::from("A"));
                *t1_x_move_c.as_ref().borrow_mut() -= 0.05;
                draw();
            } else if key == "D" {
                web_sys::console::log_1(&JsString::from("D"));
                *t1_x_move_c.as_ref().borrow_mut() += 0.05;
                draw();
            } else if key == "R" {
                web_sys::console::log_1(&JsString::from("R"));
                *t2_angle_c.as_ref().borrow_mut() += 10.0;
                draw();
            } else if *t2_height_scale_c.as_ref().borrow_mut() < 1.5 && key == "L" {
                web_sys::console::log_1(&JsString::from("L"));
                *t2_height_scale_c.as_ref().borrow_mut() += 0.1;
                draw();
            } else if *t2_height_scale_c.as_ref().borrow_mut() > 0.2 && key == "S" {
                web_sys::console::log_1(&JsString::from("S"));
                *t2_height_scale_c.as_ref().borrow_mut() -= 0.1;
                draw();
            } else if key == "O" {
                web_sys::console::log_1(&JsString::from("O"));
                *t3_angle_c.as_ref().borrow_mut() += 10.0;
                draw();
            }

        });
        document.add_event_listener_with_callback("keydown", closure.as_ref().unchecked_ref()).unwrap();
        closure.forget();
    }

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

