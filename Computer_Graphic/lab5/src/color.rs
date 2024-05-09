#[derive(Copy, Clone, Debug)]
pub enum Color {
    RED,
    BLUE,
    GREEN,
    DARKGREEN,
    WHITE
}

impl Into<&[f32; 4]> for Color {
    fn into(self) -> &'static [f32; 4] {
        match self {
            Color::RED => &[1.0, 0.0, 0.0, 1.0],
            Color::BLUE => &[0.0, 0.0, 1.0, 1.0],
            Color::GREEN => &[0.0, 1.0, 0.0, 1.0],
            Color::DARKGREEN => &[0.0, 1.0, 0.6, 1.0],
            Color::WHITE => &[1.0, 1.0, 1.0, 1.0]
        }
    }
}

impl Into<&[f32; 3]> for Color {
    fn into(self) -> &'static [f32; 3] {
        match self {
            Color::RED => &[1.0, 0.0, 0.0],
            Color::BLUE => &[0.0, 0.0, 1.0],
            Color::GREEN => &[0.0, 1.0, 0.0],
            Color::DARKGREEN => &[0.0, 0.5, 0.32],
            Color::WHITE => &[1.0, 1.0, 1.0]
        }
    }
}

impl From<(f32, f32)> for Color {
    fn from(value: (f32, f32)) -> Self {
        if value.0 <= 0.0 {
            if value.1 <= 0.0 {
                Self::GREEN
            } else {
                Self::BLUE
            }
        } else {
            if value.1 <= 0.0 {
                Self::WHITE
            } else {
                Self::RED
            }
        }
    }
}