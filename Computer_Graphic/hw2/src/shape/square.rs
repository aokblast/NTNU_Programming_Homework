use std::f32::consts::PI;
use gl_matrix::common::Mat4;
use js_sys::{Array, Float32Array};
use wasm_bindgen::JsValue;
use web_sys::{WebGl2RenderingContext, WebGlBuffer, WebGlProgram, WebGlUniformLocation};
use crate::shape::create_array_buffer_view;

pub struct SquareInformation {
    vertices: Float32Array,
    colors: Float32Array,
    u_matrix: WebGlUniformLocation,
    a_position: u32,
    a_color: u32,
    vert_buffer: WebGlBuffer,
    color_buffer: WebGlBuffer,
    context: WebGl2RenderingContext,
}

impl SquareInformation {
    pub fn new(vert: &[f32; 4], color: &[f32; 6], context: WebGl2RenderingContext, program: &WebGlProgram) -> Self {
        let mut res = Self {
            vertices: Float32Array::new_with_length(0),
            colors: Float32Array::new_with_length(0),
            u_matrix: context.get_uniform_location(program, "u_modelMatrix").unwrap(),
            a_position: context.get_attrib_location(program, "a_Position") as u32,
            a_color: context.get_attrib_location(program, "a_Color") as u32,
            vert_buffer: context.create_buffer().unwrap(),
            color_buffer: context.create_buffer().unwrap(),
            context,
        };

        res.init_triangle(vert, color);
        res.init_bind_buffer();

        res
    }

    fn init_triangle(&mut self, vert: &[f32; 4], color: &[f32; 6]) {
        let vert_arr = Array::new_with_length(0);
        let colors_arr = Array::new_with_length(0);

        vert_arr.push(&JsValue::from(vert[0]));
        vert_arr.push(&JsValue::from(vert[1]));
        vert_arr.push(&JsValue::from(vert[0]));
        vert_arr.push(&JsValue::from(vert[3]));
        vert_arr.push(&JsValue::from(vert[2]));
        vert_arr.push(&JsValue::from(vert[1]));

        vert_arr.push(&JsValue::from(vert[2]));
        vert_arr.push(&JsValue::from(vert[3]));
        vert_arr.push(&JsValue::from(vert[0]));
        vert_arr.push(&JsValue::from(vert[3]));
        vert_arr.push(&JsValue::from(vert[2]));
        vert_arr.push(&JsValue::from(vert[1]));


        for _ in 0..3 {
            for x in &color[0..3] {
                colors_arr.push(&JsValue::from(*x));
            }
        }

        for _ in 0..3 {
            for x in &color[3..6] {
                colors_arr.push(&JsValue::from(*x));
            }
        }

        self.vertices = Float32Array::new(&vert_arr);
        self.colors = Float32Array::new(&colors_arr);
    }

    fn init_bind_buffer(&self) {
        self.context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&self.vert_buffer));
        self.context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &self.vertices, WebGl2RenderingContext::STATIC_DRAW);

        self.context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&self.color_buffer));
        self.context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &self.colors, WebGl2RenderingContext::STATIC_DRAW);

        self.context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, None);
        self.context.bind_buffer(WebGl2RenderingContext::ELEMENT_ARRAY_BUFFER, None);
    }

    pub fn draw_with_transformation(&self, mat: &Mat4) {
        self.context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&self.color_buffer));
        self.context.vertex_attrib_pointer_with_i32(self.a_color, 3, WebGl2RenderingContext::FLOAT, false, 0, 0);
        self.context.enable_vertex_attrib_array(self.a_color);
        self.context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&self.vert_buffer));
        self.context.vertex_attrib_pointer_with_i32(self.a_position, 2, WebGl2RenderingContext::FLOAT, false, 0, 0);
        self.context.enable_vertex_attrib_array(self.a_position);
        self.context.uniform_matrix4fv_with_f32_array(Some(&self.u_matrix), false, mat);
        self.context.draw_arrays(WebGl2RenderingContext::TRIANGLES, 0, 6);
    }

    pub fn change_color(&mut self, new_color: &[f32; 6]) {
        let colors_arr = Array::new_with_length(0);

        for _ in 0..3 {
            for x in &new_color[0..3] {
                colors_arr.push(&JsValue::from(*x));
            }
        }

        for _ in 0..3 {
            for x in &new_color[3..6] {
                colors_arr.push(&JsValue::from(*x));
            }
        }

        self.colors = Float32Array::new(&colors_arr);
        self.context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&self.color_buffer));
        self.context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &self.colors, WebGl2RenderingContext::STATIC_DRAW);
    }
}
