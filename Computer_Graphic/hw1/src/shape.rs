use js_sys::Float32Array;

pub mod triangle;
pub mod circle;

pub fn create_array_buffer_view(buffer: &[f32]) -> Float32Array {
    unsafe {
        Float32Array::view(buffer)
    }
}


#[derive(Debug, PartialEq, Eq, Hash, Copy, Clone)]
pub enum Shape {
    POINT = 0,
    SQUARE = 1,
    CIRCLE = 2,
    TRIANGLE = 3,
}