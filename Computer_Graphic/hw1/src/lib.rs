use std::cell::{RefCell, RefMut};
use std::collections::{HashMap, VecDeque};
use std::f32::consts::PI;
use std::rc::Rc;
use gl_matrix::{mat4, vec2, vec4};
use js_sys::JsString;
use wasm_bindgen::prelude::*;
use web_sys::{KeyboardEvent, MouseEvent, WebGl2RenderingContext, WebGlProgram, WebGlShader, window};
use web_sys::console::log_1;
use shape::Shape;
use crate::color::Color;
use crate::shape::create_array_buffer_view;

mod shape;
mod color;


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
        void main(){
            gl_Position = a_Position;
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


    let cur_type = Rc::new(RefCell::new(Shape::CIRCLE));
    let cur_color = Rc::new(RefCell::new(Color::GREEN));
    let shape_info = Rc::new(RefCell::new(HashMap::new()));

    {
        let cur_type = cur_type.clone();
        let cur_color = cur_color.clone();

        let closure = Closure::<dyn Fn(_)>::new(move |key : KeyboardEvent| {
            let mut key = key.key();
            key = key.to_uppercase();

            if key == "C" {
                *cur_type.borrow_mut() = Shape::CIRCLE;
            } else if key == "P" {
                *cur_type.borrow_mut() = Shape::POINT;
            } else if key == "Q" {
                *cur_type.borrow_mut() = Shape::SQUARE;
            } else if key == "T" {
                *cur_type.borrow_mut() = Shape::TRIANGLE;
            } else if key == "R" {
                *cur_color.borrow_mut() = Color::RED;
            } else if key == "G" {
                *cur_color.borrow_mut() = Color::GREEN;
            } else if key == "B" {
                *cur_color.borrow_mut() = Color::BLUE;
            }
        });

        document.add_event_listener_with_callback("keydown", closure.as_ref().unchecked_ref())?;

        closure.forget();
    }

    let pos_attr = context.get_attrib_location(&program, "a_Position") as u32;
    let color_attr = context.get_attrib_location(&program, "a_Color") as u32;

    let draw = move |shape_info: RefMut<HashMap<Shape, VecDeque<((f32, f32), Color)>>>| {
        context.clear_color(0.0, 0.0, 0.0, 1.0);
        context.clear(WebGl2RenderingContext::COLOR_BUFFER_BIT);

        for (shape_type, shapes) in &*shape_info {
            if shapes.len() == 0 {
                continue;
            }

            let mut color_arr = vec![];
            let mut vert_arr = vec![];

            for shape in shapes {
                let color = Into::<&[f32; 3]>::into(shape.1);
                let mut transform = mat4::create();
                let mut cur_pos = vec![];
                mat4::from_translation(&mut transform, &[shape.0.0, shape.0.1, 0.0]);

                match shape_type {
                    Shape::POINT => {
                        cur_pos.push((0.0f32, 0.0f32));
                        color_arr.extend_from_slice(color);
                    },
                    Shape::CIRCLE => {
                        for idx in 0..200 {
                            let deg = idx as f32 * PI / 100.0;
                            let x = 0.05 * deg.cos();
                            let y = 0.05 * deg.sin();
                            cur_pos.push((x, y));
                            let deg = (idx + 1) as f32 * PI / 100.0;
                            let x = 0.05 * deg.cos();
                            let y = 0.05 * deg.sin();
                            cur_pos.push((x, y));
                            cur_pos.push((0.0, 0.0));
                            color_arr.extend_from_slice(color);
                            color_arr.extend_from_slice(color);
                            color_arr.extend_from_slice(color);
                        }
                    },
                    Shape::TRIANGLE => {
                        cur_pos.push((0.0, 0.04));
                        cur_pos.push((-0.05, -0.05));
                        cur_pos.push((0.05, -0.05));
                        color_arr.extend_from_slice(color);
                        color_arr.extend_from_slice(color);
                        color_arr.extend_from_slice(color);
                    },
                    Shape::SQUARE => {
                        cur_pos.push((-0.05, -0.05));
                        cur_pos.push((-0.05, 0.05));
                        cur_pos.push((0.05, -0.05));
                        cur_pos.push((0.05, -0.05));
                        cur_pos.push((0.05, 0.05));
                        cur_pos.push((-0.05, 0.05));
                        color_arr.extend_from_slice(color);
                        color_arr.extend_from_slice(color);
                        color_arr.extend_from_slice(color);
                        color_arr.extend_from_slice(color);
                        color_arr.extend_from_slice(color);
                        color_arr.extend_from_slice(color);
                    }
                }

                for pos in cur_pos {
                    let mut new_pos = vec4::create();
                    let pos = vec4::from_values(pos.0, pos.1, 0.0, 1.0);
                    vec4::transform_mat4(&mut new_pos, &pos, &transform);
                    vert_arr.push(new_pos[0]);
                    vert_arr.push(new_pos[1]);
                    vert_arr.push(new_pos[2]);
                    vert_arr.push(new_pos[3]);
                }
            }

            let color_arr = create_array_buffer_view(color_arr.as_slice());
            let vert_arr = create_array_buffer_view(vert_arr.as_slice());

            context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, context.create_buffer().as_ref());
            context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &color_arr, WebGl2RenderingContext::STATIC_DRAW);
            context.vertex_attrib_pointer_with_i32(color_attr, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
            context.enable_vertex_attrib_array(color_attr);

            context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, context.create_buffer().as_ref());
            context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &vert_arr, WebGl2RenderingContext::STATIC_DRAW);
            context.vertex_attrib_pointer_with_i32(pos_attr, 4, WebGl2RenderingContext::FLOAT, false, 0, 0);
            context.enable_vertex_attrib_array(pos_attr);

            match shape_type {
                Shape::POINT => {
                    context.draw_arrays(WebGl2RenderingContext::POINTS, 0, shapes.len() as i32);
                },
                Shape::CIRCLE => {
                    context.draw_arrays(WebGl2RenderingContext::TRIANGLES, 0, shapes.len() as i32 * 600);
                },
                Shape::TRIANGLE => {
                    context.draw_arrays(WebGl2RenderingContext::TRIANGLES, 0, shapes.len() as i32 * 3);
                },
                Shape::SQUARE => {
                    context.draw_arrays(WebGl2RenderingContext::TRIANGLES, 0, shapes.len() as i32 * 6);
                },
            }
        }
    };

    draw(shape_info.borrow_mut());

    {
        let cur_type = cur_type.clone();
        let cur_color = cur_color.clone();
        let shape_info = shape_info.clone();

        let closure = Closure::<dyn Fn(_)>::new(move |mouse : MouseEvent| {
            let loc = (mouse.offset_x() as f32 / 400.0 - 1.0, -(mouse.offset_y() as f32 / 400.0 - 1.0));

            let mut shape_info = shape_info.borrow_mut();
            let cur_type = *cur_type.borrow();
            let cur_color = *cur_color.borrow();

            let mut entry = shape_info.entry(cur_type).or_insert(VecDeque::new());

            entry.push_back((loc, cur_color));
            if entry.len() == 4 {
                entry.pop_front();
            }

            draw(shape_info);
        });

        document.get_element_by_id("canvas").unwrap().add_event_listener_with_callback("mousedown", closure.as_ref().unchecked_ref())?;
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

