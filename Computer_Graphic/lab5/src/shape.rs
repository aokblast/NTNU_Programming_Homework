use js_sys::Float32Array;

pub mod triangle;
pub mod circle;

pub fn create_array_buffer_view(buffer: &[f32]) -> Float32Array {
    unsafe {
        Float32Array::view(buffer)
    }
}