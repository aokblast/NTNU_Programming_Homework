use std::f32::consts::PI;

use gl_matrix::common::Mat4;
use js_sys::{Array, Float32Array};
use wasm_bindgen::JsValue;
use web_sys::{WebGl2RenderingContext, WebGlBuffer, WebGlProgram, WebGlUniformLocation};

pub struct CircleInformation {
    vertices: Float32Array,
    colors: Float32Array,
    u_matrix: WebGlUniformLocation,
    a_position: u32,
    a_color: u32,
    vert_buffer: WebGlBuffer,
    color_buffer: WebGlBuffer,
    context: WebGl2RenderingContext,
}

impl CircleInformation {
    pub fn new(radius: f32, color: [f32; 3], context: WebGl2RenderingContext, program: WebGlProgram) -> Self {
        let mut res = Self {
            vertices: Float32Array::new_with_length(0),
            colors: Float32Array::new_with_length(0),
            u_matrix: context.get_uniform_location(&program, "u_modelMatrix").unwrap(),
            a_position: context.get_attrib_location(&program, "a_Position") as u32,
            a_color: context.get_attrib_location(&program, "a_Color") as u32,
            vert_buffer: context.create_buffer().unwrap(),
            color_buffer: context.create_buffer().unwrap(),
            context,
        };

        res.init_circle(radius, color);
        res.init_bind_buffer();

        res
    }

    fn init_circle(&mut self, radius: f32, color: [f32; 3]) {
        let vert = Array::new_with_length(0);
        let colors = Array::new_with_length(0);
        for idx in 0..1001 {
            let deg = idx as f32 * PI / 100.0;
            let x = radius * deg.cos();
            let y = radius * deg.sin();
            vert.push(&JsValue::from(x));
            vert.push(&JsValue::from(y));
            colors.push(&JsValue::from(color[0]));
            colors.push(&JsValue::from(color[1]));
            colors.push(&JsValue::from(color[2]));
        }

        self.vertices = Float32Array::new(&vert);
        self.colors = Float32Array::new(&colors);
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
        self.context.draw_arrays(WebGl2RenderingContext::TRIANGLES, 0, 2002 / 3);
    }

    pub fn change_color(&mut self, color: &[f32; 3]) {
        let colors = Array::new_with_length(0);
        for _ in 0..1001 {
            colors.push(&JsValue::from(color[0]));
            colors.push(&JsValue::from(color[1]));
            colors.push(&JsValue::from(color[2]));
        }

        self.colors = Float32Array::new(&colors);
        self.context.bind_buffer(WebGl2RenderingContext::ARRAY_BUFFER, Some(&self.color_buffer));
        self.context.buffer_data_with_array_buffer_view(WebGl2RenderingContext::ARRAY_BUFFER, &self.colors, WebGl2RenderingContext::STATIC_DRAW);
    }
}
