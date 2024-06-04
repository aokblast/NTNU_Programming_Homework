use js_sys::Float32Array;
use web_sys::WebGl2RenderingContext;

pub mod triangle;
pub mod circle;
pub mod square;

pub fn create_array_buffer_view(buffer: &[f32]) -> Float32Array {
    unsafe {
        Float32Array::view(buffer)
    }
}
