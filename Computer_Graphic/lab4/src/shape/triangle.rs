use gl_matrix::common::Mat4;
use js_sys::Float32Array;
use web_sys::{WebGl2RenderingContext, WebGlBuffer, WebGlProgram, WebGlUniformLocation};
use crate::shape::create_array_buffer_view;

pub struct TriangleInformation {
    vertices: Float32Array,
    colors: Float32Array,
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
        let res = Self {
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

    pub fn change_color(&mut self, new_color: &[f32; 9]) {
        self.colors.set(&create_array_buffer_view(new_color), 0);
    }
}
