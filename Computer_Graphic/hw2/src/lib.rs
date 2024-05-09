use std::cell::{Ref, RefCell, RefMut};
use std::f32::consts::PI;
use std::fmt::format;
use std::ops::Range;
use std::rc::Rc;
use std::task::Context;

use gl_matrix::common::{Mat4, Vec3};
use gl_matrix::mat2::{create, from_rotation};
use gl_matrix::{mat4, vec2, vec3};
use gl_matrix::mat3::translate;
use gl_matrix::mat4::{from_z_rotation, ortho, perspective};
use gl_matrix::vec2::squared_distance;
use js_sys::JsString;
use js_sys::Math::min;
use wasm_bindgen::prelude::*;
use web_sys::{KeyboardEvent, MouseEvent, WebGl2RenderingContext, window};
use web_sys::console::log_1;
use crate::color::Color;
use crate::shape::circle::CircleInformation;

use crate::shape::square::SquareInformation;
use crate::shape::triangle::TriangleInformation;
use crate::webgl_link::{compile_shader, link_program};

mod shape;
mod color;
mod webgl_link;

#[derive(Default)]
struct Query1 {
    pub offset_x: f32,
    pub offset_y: f32,
    pub degree: [f32; 3],
    pub pressed: bool,
    pub zoom: f32,
}

#[derive(Default)]
struct Query2 {
    pub offset_x: f32,
    pub offset_y: f32,
    pub dragged: bool,
    pub degree: [f32; 2]
}

fn in_circle(radius: f32, circle: &[f32; 2], point: &[f32; 2]) -> bool {
    let x_diff = circle[0] - point[0];
    let y_diff = circle[1] - point[1];

    if x_diff * x_diff + y_diff * y_diff <= radius * radius {
        return true;
    }

    return false;
}

fn draw_robot(base: &Mat4, q1: &RefMut<Query1>, perspective: &Mat4, square: &mut SquareInformation, triangle: &mut TriangleInformation, circle: &mut CircleInformation, context: &WebGl2RenderingContext) -> Mat4 {
    let mut stack = vec![];
    stack.push(base.clone());

    // draw body
    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::scale(&mut cur, &tmp, &vec3::from_values(1.5, 3.5, 0.0));
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    square.change_color(&[1.0, 0.0, 0.0, 1.0, 0.0, 0.0]);
    square.draw_with_transformation(&cur);

    // Left circle
    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(-0.08, -0.21, 0.0));
    let tmp = cur.clone();
    mat4::scale(&mut cur, &tmp, &vec3::from_values(1.5, 1.5, 0.0));
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    circle.change_color(&[0.5, 0.3, 0.5]);
    circle.draw_with_transformation(&cur);

    // Right circle
    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(0.08, -0.21, 0.0));
    let tmp = cur.clone();
    mat4::scale(&mut cur, &tmp, &vec3::from_values(1.5, 1.5, 0.0));
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    circle.change_color(&[0.5, 0.3, 0.5]);
    circle.draw_with_transformation(&cur);

    // Head
    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(0.0, 0.25, 0.0));
    let tmp = cur.clone();
    mat4::scale(&mut cur, &tmp, &vec3::from_values(3.0, 3.0, 0.0));
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    circle.change_color(&[1.0, 1.0, 0.0]);
    circle.draw_with_transformation(&cur);

    // Right hand
    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(0.1025, 0.1, 0.0));
    stack.push(cur);
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    circle.change_color(&[0.0, 1.0, 1.0]);
    circle.draw_with_transformation(&cur);

    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(0.125, 0.0, 0.0));
    let tmp = cur.clone();
    mat4::scale(&mut cur, &tmp, &vec3::from_values(2.0, 0.5, 0.0));
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    square.change_color(&[0.0, 1.0, 0.0, 0.0, 1.0, 0.0]);
    square.draw_with_transformation(&cur);
    stack.pop();

    // Articulation point 1
    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(-0.1025, 0.1, 0.0));
    stack.push(cur);
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    circle.change_color(&[0.0, 1.0, 1.0]);
    circle.draw_with_transformation(&cur);

    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::rotate_z(&mut cur, &tmp, q1.degree[0] / 180.0 * PI);
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(-0.125, 0.0, 0.0));
    stack.push(cur);
    let tmp = cur.clone();
    mat4::scale(&mut cur, &tmp, &vec3::from_values(2.0, 0.5, 0.0));
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    square.change_color(&[0.0, 1.0, 0.0, 0.0, 1.0, 0.0]);
    square.draw_with_transformation(&cur);

    // Articulation point 2
    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(-0.125, 0.0, 0.0));
    stack.push(cur);
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    circle.change_color(&[0.0, 1.0, 1.0]);
    circle.draw_with_transformation(&cur);

    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::rotate_z(&mut cur, &tmp, q1.degree[1] / 180.0 * PI);
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(-0.125, 0.0, 0.0));
    stack.push(cur);
    let tmp = cur.clone();
    mat4::scale(&mut cur, &tmp, &vec3::from_values(2.0, 0.5, 0.0));
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    square.change_color(&[0.0, 1.0, 0.0, 0.0, 1.0, 0.0]);
    square.draw_with_transformation(&cur);

    // Articulation point 3
    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(-0.125, 0.0, 0.0));
    stack.push(cur);
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    circle.change_color(&[0.0, 1.0, 1.0]);
    circle.draw_with_transformation(&cur);

    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::rotate_z(&mut cur, &tmp, -0.5 * PI);
    let tmp = cur.clone();
    mat4::rotate_z(&mut cur, &tmp, q1.degree[2] / 180.0 * PI);
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(-0.0, -0.06, 0.0));
    stack.push(cur);
    let tmp = cur.clone();
    mat4::scale(&mut cur, &tmp, &vec3::from_values(0.75, 0.75, 0.0));
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    triangle.change_color(&[0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0]);
    triangle.draw_with_transformation(&cur);

    *stack.last().unwrap()
}

fn draw_object(base: &Mat4, q2: &RefMut<Query2>, perspective: &Mat4, square: &mut SquareInformation, triangle: &mut TriangleInformation, circle: &mut CircleInformation, context: &WebGl2RenderingContext) {
    let mut stack = vec![];
    stack.push(base.clone());

    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::scale(&mut cur, &tmp, &vec3::from_values(0.5, 0.5, 0.0));
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    square.change_color(&[0.3, 0.3, 0.5, 0.3, 0.3, 0.5]);
    square.draw_with_transformation(&cur);

    // Articulation point 1
    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(-0.05, 0.0, 0.0));
    stack.push(cur);
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    circle.change_color(&[0.0, 1.0, 1.0]);
    circle.draw_with_transformation(&cur);

    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::rotate_z(&mut cur, &tmp, q2.degree[0] / 180.0 * PI);
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(-0.06, 0.0, 0.0));
    stack.push(cur);
    let tmp = cur.clone();
    mat4::rotate_z(&mut cur, &tmp, 0.25 * PI);
    let tmp = cur.clone();
    mat4::scale(&mut cur, &tmp, &vec3::from_values(0.5, 0.5, 0.0));
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    square.change_color(&[0.3, 0.3, 0.5, 0.3, 0.3, 0.5]);
    square.draw_with_transformation(&cur);

    // Articulation point 2
    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(-0.06, 0.0, 0.0));
    stack.push(cur);
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    circle.change_color(&[0.0, 1.0, 1.0]);
    circle.draw_with_transformation(&cur);

    let base = stack.last().unwrap();
    let mut cur = base.clone();
    let tmp = cur.clone();
    mat4::rotate_z(&mut cur, &tmp, -0.5 * PI);
    let tmp = cur.clone();
    mat4::rotate_z(&mut cur, &tmp, q2.degree[1] / 180.0 * PI);
    let tmp = cur.clone();
    mat4::translate(&mut cur, &tmp, &vec3::from_values(-0.0, -0.06, 0.0));
    stack.push(cur);
    let tmp = cur.clone();
    mat4::scale(&mut cur, &tmp, &vec3::from_values(0.75, 0.75, 0.0));
    let tmp = cur.clone();
    mat4::multiply(&mut cur, &perspective, &tmp);
    triangle.change_color(&[0.3, 0.3, 0.5, 0.3, 0.3, 0.5, 0.3, 0.3, 0.5]);
    triangle.draw_with_transformation(&cur);
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

    let q1 = Rc::new(RefCell::new(Query1::default()));
    let q2 = Rc::new(RefCell::new(Query2::default()));

    let draw = {
        move |mut q1: RefMut<Query1>, mut q2: RefMut<Query2>| {
            let mut circle = CircleInformation::new(0.025, &[1.0, 1.0, 1.0], context.clone(), &program);
            let mut square = SquareInformation::new(&[-0.05, -0.05, 0.05, 0.05], &[0.0, 0.0, 0.0, 0.0, 0.0, 0.0], context.clone(), &program);
            let mut triangle = TriangleInformation::new(&[-0.05, -0.05, 0.05, -0.05, 0.0, 0.05], &[1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], context.clone(), &program);
            context.viewport(0, 0, 800, 800);
            context.scissor(0, 0, 800, 800);
            context.clear_color(0.0, 0.0, 0.0, 1.0);
            context.clear(WebGl2RenderingContext::COLOR_BUFFER_BIT | WebGl2RenderingContext::DEPTH_BUFFER_BIT);

            // Perspective matrix
            let mut ortho_mat = mat4::create();
            ortho(&mut ortho_mat, 1.0 - q1.zoom, -1.0 + q1.zoom, 1.0 - q1.zoom, -1.0 + q1.zoom, 2.0, 0.0);

            // view matrix
            let mut view_mat = mat4::create();
            mat4::look_at(&mut view_mat, &vec3::from_values(0.0, 0.0, 1.0), &vec3::from_values(0.0, 0.0, -1.0), &vec3::from_values(0.0, -1.0, 0.0));
            let tmp = view_mat.clone();
            mat4::multiply(&mut view_mat, &ortho_mat, &tmp);

            log_1(&JsString::from(format!("{}", q1.zoom)));

            // Draw robot
            let mut base = mat4::create();
            mat4::from_translation(&mut base, &vec3::from_values(q1.offset_x, q1.offset_y, 0.0));
            let trans = draw_robot(&base, &q1, &view_mat, &mut square, &mut triangle, &mut circle, &context);

            // Check if in circle
            let mut v2 = vec2::create();
            let tmp = v2.clone();
            vec2::transform_mat4(&mut v2, &tmp, &trans);

            if q1.pressed {
                q1.pressed = false;
                if in_circle(0.05, &[v2[0], v2[1]], &[q2.offset_x, q2.offset_y]) {
                    q2.dragged = !q2.dragged;
                }
            }

            if q2.dragged {
                log_1(&JsString::from(format!("{:?}, {:?}{:?}, {}\n", v2, q2.offset_x, q2.offset_y, q2.dragged)));
                q2.offset_x = v2[0];
                q2.offset_y = v2[1];
            }

            // Draw object
            let mut base = mat4::create();
            mat4::from_translation(&mut base, &vec3::from_values(q2.offset_x, q2.offset_y, 0.0));
            draw_object(&base, &q2, &view_mat, &mut square, &mut triangle, &mut circle, &context);
        }
    };

    draw(q1.borrow_mut(), q2.borrow_mut());

    {
        let q1 = q1.clone();
        let q2 = q2.clone();

        let closure = Closure::<dyn Fn(_)>::new(move |keyboard_event: KeyboardEvent| {
            let mut key = keyboard_event.key();
            key = key.to_uppercase();

            let mut q1 = q1.borrow_mut();
            let mut q2 = q2.borrow_mut();

            match &key[..] {
                "A" => q1.offset_x -= 0.05,
                "D" => q1.offset_x += 0.05,
                "S" => q1.offset_y -= 0.05,
                "W" => q1.offset_y += 0.05,
                "I" => q1.degree[0] += 10.0,
                "O" => q1.degree[0] -= 10.0,
                "K" => q1.degree[1] += 10.0,
                "L" => q1.degree[1] -= 10.0,
                "N" => q1.degree[2] += 10.0,
                "M" => q1.degree[2] -= 10.0,
                "Z" => q1.zoom += 0.05,
                "X" => q1.zoom -= 0.05,
                "R" => q1.pressed = true,
                "Y" => q2.degree[0] += 10.0,
                "U" => q2.degree[0] -= 10.0,
                "H" => q2.degree[1] += 10.0,
                "J" => q2.degree[1] -= 10.0,
                _ => {}
            }

            q1.zoom = f32::min(0.25, q1.zoom);
            q1.zoom = f32::max(-0.25, q1.zoom);

            draw(q1, q2);
        });

        document.add_event_listener_with_callback("keydown", closure.as_ref().unchecked_ref()).unwrap();
        closure.forget();
    }

    Ok(())
}
