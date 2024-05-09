use std::cell::RefCell;
use std::ops::Range;
use std::rc::Rc;

use gl_matrix::common::{Mat4, Vec3};
use gl_matrix::mat4;
use gl_matrix::mat4::{ortho, perspective};
use wasm_bindgen::prelude::*;
use web_sys::{MouseEvent, WebGl2RenderingContext, window};

use crate::shape::create_array_buffer_view;
use crate::webgl_link::{compile_shader, link_program};

mod shape;
mod color;
mod webgl_link;

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

    context.enable(WebGl2RenderingContext::DEPTH_TEST);
    context.enable(WebGl2RenderingContext::SCISSOR_TEST);

    // Bind buffer
    let vertices = create_array_buffer_view(
        &[0.0, 1.0, -2.0,
        -0.5, -1.0, -2.0,
        0.5, -1.0, -2.0,
        0.0, 1.0, -0.0,
        -0.5, -1.0, -0.0,
        0.5, -1.0, -0.0,
        0.0, 1.0, 2.0,
        -0.5, -1.0, 2.0,
        0.5, -1.0, 2.0]);

    let colors = create_array_buffer_view(&[
        0.7, 0.0, 0.0, //r, g, b of the 1st vertex of the 1st triangle
        0.7, 0.0, 0.0,
        0.7, 0.0, 0.0,
        0.0, 0.7, 0.0,
        0.0, 0.7, 0.0,
        0.0, 0.7, 0.0,
        0.0, 0.0, 0.7,
        0.0, 0.0, 0.7,
        0.0, 0.0, 0.7, ]);

    let vertices_buffer = context.create_buffer().unwrap();
    context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&vertices_buffer));
    context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &vertices, WebGl2RenderingContext::STATIC_DRAW);

    let color_buffer = context.create_buffer().unwrap();
    context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&color_buffer));
    context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &colors, WebGl2RenderingContext::STATIC_DRAW);

    context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, None);
    context.bind_buffer(WebGl2RenderingContext::ELEMENT_ARRAY_BUFFER, None);

    // Variables shared in events
    let mouse_dragged = Rc::new(RefCell::new(false));
    let last_x = Rc::new(RefCell::new(0.0));
    let last_y = Rc::new(RefCell::new(0.0));

    // MouseDown, detect drag and set x, y
    {
        let mouse_dragged = mouse_dragged.clone();
        let last_x = last_x.clone();
        let last_y = last_y.clone();

        let closure = Closure::<dyn Fn(_)>::new(move |mouse_event: MouseEvent| {
            let rect = mouse_event.target().unwrap().dyn_into::<web_sys::HtmlCanvasElement>().unwrap().get_bounding_client_rect();
            let (x, y) = (mouse_event.client_x() as f64, mouse_event.client_y() as f64);

            if (rect.left()..rect.right()).contains(&x)  && (rect.top()..rect.bottom()).contains(&y) {
                *last_x.borrow_mut() = x;
                *last_y.borrow_mut() = y;
                *mouse_dragged.borrow_mut() = true;
            }
        });

        canvas.add_event_listener_with_callback("mousedown", closure.as_ref().unchecked_ref()).unwrap();
        closure.forget();
    }


    // Mouseup, stop dragged
    {
        let mouse_dragged = mouse_dragged.clone();

        let closure = Closure::<dyn Fn(_)>::new(move |mouse_event: MouseEvent| {
            *mouse_dragged.borrow_mut() = false;
        });

        canvas.add_event_listener_with_callback("mouseup", closure.as_ref().unchecked_ref()).unwrap();
        closure.forget();
    }

    let last_x = last_x.clone();
    let last_y = last_y.clone();

    let angle_x = Rc::new(RefCell::new(0.0));
    let angle_y = Rc::new(RefCell::new(0.0));

    let draw_with_view = move |view_mat: &Mat4, perspect_mat: &Mat4, model_matrix: &Mat4, l: Range<i32>, r: Range<i32>, color: (f32, f32, f32)| {
        let color_idx = context.get_attrib_location(&program, "a_Color") as u32;
        let pos_idx = context.get_attrib_location(&program, "a_Position") as u32;

        context.viewport(l.start, r.start, l.end - l.start, r.end - r.start);
        context.scissor(l.start, r.start, l.end - l.start, r.end - r.start);

        context.clear_color(color.0, color.1, color.2, 1.0);
        context.clear(WebGl2RenderingContext::DEPTH_BUFFER_BIT | WebGl2RenderingContext::COLOR_BUFFER_BIT);

        context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&vertices_buffer));
        context.vertex_attrib_pointer_with_i32(pos_idx, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
        context.enable_vertex_attrib_array(pos_idx);

        context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&color_buffer));
        context.vertex_attrib_pointer_with_i32(color_idx, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
        context.enable_vertex_attrib_array(color_idx);

        // Apply perspect and viewpoin
        let mut transform_mat = perspect_mat.clone();
        let tmp = transform_mat.clone();
        mat4::multiply(&mut transform_mat, &tmp, &view_mat);
        let tmp = transform_mat.clone();
        mat4::multiply(&mut transform_mat, &tmp, &model_matrix);
        let tmp = transform_mat.clone();
        mat4::translate(&mut transform_mat, &tmp, &Vec3::from([1.0, 0.0, 0.0]));

        context.uniform_matrix4fv_with_f32_array(Some(&context.get_uniform_location(&program, "u_modelMatrix").unwrap()), false, &transform_mat);
        context.draw_arrays(WebGl2RenderingContext::TRIANGLES, 0, 9);

        context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&vertices_buffer));
        context.vertex_attrib_pointer_with_i32(pos_idx, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
        context.enable_vertex_attrib_array(pos_idx);

        context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&color_buffer));
        context.vertex_attrib_pointer_with_i32(color_idx, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
        context.enable_vertex_attrib_array(color_idx);
        let tmp = transform_mat.clone();
        mat4::translate(&mut transform_mat, &tmp, &Vec3::from([-2.0, 0.0, 0.0]));
        context.uniform_matrix4fv_with_f32_array(Some(&context.get_uniform_location(&program, "u_modelMatrix").unwrap()), false, &transform_mat);
        context.draw_arrays(WebGl2RenderingContext::TRIANGLES, 0, 9);
    };

    let draw = move |x: f64, y: f64, mouse_dragged: bool| {
        if mouse_dragged {
            let last_x = *last_x.borrow();
            let last_y = *last_y.borrow();
            let factor = 100.0 / canvas.height() as f64;
            let (dx, dy) = (factor * (x - last_x), factor * (y - last_y));

            *angle_x.borrow_mut() += dx;
            *angle_y.borrow_mut() += dy;
        }

        *last_x.borrow_mut() = x;
        *last_y.borrow_mut() = y;

        // Create model matrix
        let mut model_matrix = mat4::create();
        mat4::from_rotation(&mut model_matrix, -(*angle_y.borrow() as f32), &Vec3::from([1.0, 0.0, 0.0]));
        let tmp = model_matrix.clone();
        mat4::rotate(&mut model_matrix, &tmp, (*angle_x.borrow()) as f32, &Vec3::from([0.0, 1.0, 0.0]));
        let tmp = model_matrix.clone();
        mat4::translate(&mut model_matrix, &tmp, &Vec3::from([0.0, 0.0, 0.0]));

        // View matrix
        let mut view_mat = mat4::create();
        mat4::look_at(&mut view_mat, &Vec3::from([0.0, 0.0, -5.0]), &Vec3::from([0.0, 0.0, 100.0]), &Vec3::from([0.0, -1.0, 0.0]));

        // Perspect Matrix
        let mut perspective_mat = mat4::create();
        perspective(&mut perspective_mat, 30.0, 2.0, 1.0, Some(100.0));
        draw_with_view(&view_mat, &perspective_mat, &model_matrix, 0..400, 200..400, (0.7, 0.7, 0.7));

        perspective(&mut perspective_mat, 30.0, 1.0, 1.0, Some(100.0));
        mat4::look_at(&mut view_mat, &Vec3::from([0.0, 0.0, 5.0]), &Vec3::from([0.0, 0.0, -100.0]), &Vec3::from([0.0, -1.0, 0.0]));
        draw_with_view(&view_mat, &perspective_mat, &model_matrix, 0..200, 0..200, (0.0, 0.0, 0.0));

        let mut orthogonol = mat4::create();
        ortho(&mut orthogonol, 3.0, -3.0, 3.0, -3.0, 0.0, 10.0);
        draw_with_view(&view_mat, &orthogonol, &model_matrix, 200..400, 0..200, (0.5, 0.5, 0.5));
    };

    draw(0.0, 0.0, false);

    {
        let mouse_dragged = mouse_dragged.clone();

        let closure = Closure::<dyn Fn(_)>::new(move |mouse_event: MouseEvent| {
            let dragged = *mouse_dragged.borrow();

            if !dragged {
                return;
            }

            draw(mouse_event.client_x() as f64, mouse_event.client_y() as f64, dragged);
        });

        window().unwrap().document().unwrap().get_element_by_id("canvas").unwrap().dyn_into::<web_sys::HtmlCanvasElement>().unwrap().add_event_listener_with_callback("mousemove", closure.as_ref().unchecked_ref()).unwrap();
        closure.forget();
    }

    Ok(())
}

