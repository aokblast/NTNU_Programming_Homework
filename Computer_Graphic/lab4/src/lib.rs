use std::cell::{RefCell, RefMut};
use std::f32::consts::PI;
use std::rc::Rc;

use gl_matrix::common::{Vec2, Vec3};
use gl_matrix::mat4;
use gl_matrix::vec2::transform_mat4;
use wasm_bindgen::prelude::*;
use web_sys::{KeyboardEvent, WebGl2RenderingContext, WebGlProgram, WebGlShader, window};

use color::Color;
use shape::circle::CircleInformation;
use shape::triangle::TriangleInformation;

mod shape;
mod color;

fn in_circle(radius: f32, circle: &[f32; 2], point: &[f32; 2]) -> bool {
    let x_diff = circle[0] - point[0];
    let y_diff = circle[1] - point[1];

    if x_diff * x_diff + y_diff * y_diff <= radius * radius {
        return true;
    }

    return false;
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

    let mut circle = CircleInformation::new(0.1, [1.0, 0.0, 0.0], context.clone(), program.clone());

    let t1_x_move = Rc::new(RefCell::new(0.0));
    let t1_y_move = Rc::new(RefCell::new(0.0));
    let t2_angle = Rc::new(RefCell::new(125.0));
    let t3_angle = Rc::new(RefCell::new(0.0));
    let t3_x_move = Rc::new(RefCell::new(0.5));
    let t3_y_move = Rc::new(RefCell::new(0.0));
    let is_grab = Rc::new(RefCell::new(false));

    let mut draw = move |t1_x_move, t1_y_move, t2_angle, t3_angle, is_grab, mut t3_x_move: RefMut<f32>, mut t3_y_move: RefMut<f32>| {
        let mut mat = mat4::create();
        let mut matb = mat.clone();

        context.clear_color(0.0, 0.0, 0.0, 1.0);
        context.clear(WebGl2RenderingContext::COLOR_BUFFER_BIT);
        // PartA
        mat4::translate(&mut mat, &matb, &Vec3::from([t1_x_move, t1_y_move, 0.0]));
        triangle_a.draw_with_transformation(mat);
        matb = mat.clone();
        mat4::translate(&mut mat, &matb, &Vec3::from([0.0, 0.2, 0.0]));
        matb = mat.clone();
        mat4::rotate_z(&mut mat, &matb, t2_angle / (180.0f32) * PI);
        triangle_b.draw_with_transformation(mat);

        // PartB
        let mut v = Vec2::default();
        transform_mat4(&mut v, &Vec2::from([-0.1, -0.5]), &mat);
        let in_c = in_circle(0.1, &[*t3_x_move, *t3_y_move], &v);

        if is_grab {
            *t3_x_move = v[0];
            *t3_y_move = v[1];
        }

        if in_c {
            if is_grab {
                circle.change_color(Color::DARKGREEN.into());
            } else {
                circle.change_color(Color::GREEN.into());
            }
        } else {
            circle.change_color(Color::RED.into());
        }

        mat = mat4::create();
        matb = mat.clone();
        mat4::translate(&mut mat, &matb, &Vec3::from([*t3_x_move, *t3_y_move, 0.0]));

        circle.draw_with_transformation(mat);
        matb = mat.clone();
        mat4::rotate_z(&mut mat, &matb, t3_angle / (180.0f32) * PI);

        triangle_b.draw_with_transformation(mat);
    };

    draw(*t1_x_move.borrow(), *t1_y_move.borrow(), *t2_angle.borrow(), *t3_angle.borrow(), *is_grab.borrow(), t3_x_move.borrow_mut(), t3_y_move.borrow_mut());

    {
        let t1_x_move = t1_x_move.clone();
        let t1_y_move = t1_y_move.clone();
        let t2_angle = t2_angle.clone();
        let is_grab = is_grab.clone();
        let t3_x_move = t3_x_move.clone();
        let t3_y_move = t3_y_move.clone();

        let closure = Closure::<dyn Fn(_)>::new(move |input_event: KeyboardEvent| {
            let mut key = input_event.key();
            key = key.to_uppercase();

            let mut t1_x_move = t1_x_move.borrow_mut();
            let mut t1_y_move = t1_y_move.borrow_mut();
            let mut t2_angle = t2_angle.borrow_mut();
            let mut is_grab = is_grab.borrow_mut();
            let t3_x_move = t3_x_move.borrow_mut();
            let t3_y_move = t3_y_move.borrow_mut();

            if key == "A" {
                *t1_x_move -= 0.05;
            } else if key == "D" {
                *t1_x_move += 0.05;
            } else if key == "S" {
                *t1_y_move -= 0.1;
            } else if key == "W" {
                *t1_y_move += 0.1;
            } else if key == "R" {
                *t2_angle += 10.0;
            } else if key == "G" {
                let mut mat = mat4::create();
                let mut matb = mat.clone();

                mat4::translate(&mut mat, &matb, &Vec3::from([*t1_x_move, *t1_y_move, 0.0]));
                matb = mat.clone();
                mat4::translate(&mut mat, &matb, &Vec3::from([0.0, 0.2, 0.0]));
                matb = mat.clone();
                mat4::rotate_z(&mut mat, &matb, *t2_angle / (180.0f32) * PI);
                let mut v = Vec2::default();
                transform_mat4(&mut v, &Vec2::from([-0.1, -0.5]), &mat);
                let in_c = in_circle(0.1, &[*t3_x_move, *t3_y_move], &v);

                if in_c {
                    *is_grab = !*is_grab;
                }
            }
        });

        document.add_event_listener_with_callback("keydown", closure.as_ref().unchecked_ref()).unwrap();
        closure.forget();
    }

    {
        let t1_x_move = t1_x_move.clone();
        let t1_y_move = t1_y_move.clone();
        let t2_angle = t2_angle.clone();
        let t3_angle = t3_angle.clone();
        let is_grab = is_grab.clone();
        let t3_x_move = t3_x_move.clone();
        let t3_y_move = t3_y_move.clone();

        let closure = Rc::new(RefCell::new(None::<Closure<dyn FnMut()>>));
        let closure_copy = closure.clone();

        *closure_copy.borrow_mut() = Some(Closure::new(move || {
            *t3_angle.borrow_mut() += 1.0;
            draw(*t1_x_move.borrow(), *t1_y_move.borrow(), *t2_angle.borrow(), *t3_angle.borrow(), *is_grab.borrow(), t3_x_move.borrow_mut(), t3_y_move.borrow_mut());
            window().unwrap().request_animation_frame(closure.borrow().as_ref().unwrap().as_ref().unchecked_ref()).unwrap();
        }));

        window().unwrap().request_animation_frame(closure_copy.borrow().as_ref().unwrap().as_ref().unchecked_ref()).unwrap();
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

