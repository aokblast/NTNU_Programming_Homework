use std::f64::consts::PI;
use opencv::core::{CV_8UC3, Scalar, Size, Vec3b, Vector};
use opencv::highgui::{destroy_all_windows, imshow, wait_key};
use opencv::imgcodecs::imwrite;
use opencv::prelude::*;
use rand::prelude::*;
use histo::Histogram;

const H: i32 = 600;
const W: i32 = 400;
const _MU: f64 = 0.0;
const THETA: f64 = 25.0;

fn main() {
    let source_img = Mat::new_size_with_default(
        Size::new(W, H), CV_8UC3 ,Scalar::all(100 as f64)).unwrap();

    let mut target_img = Mat::new_size_with_default(
        Size::new(W, H), CV_8UC3 ,Scalar::all(100 as f64)).unwrap();

    let mut histogram = Histogram::with_buckets(50);

    for x in 0..target_img.rows() {
        for y in (0..(target_img.cols() - 1)).step_by(2) {
            let r = random::<f64>();
            let phi = random::<f64>();

            let z1 = THETA * (2.0 * PI * phi).cos() * (-2.0 * r.ln()).sqrt();
            let z2= THETA * (2.0 * PI * phi).sin() * (-2.0 * r.ln()).sqrt();

            let val = (target_img.at_2d::<Vec3b>(x, y).unwrap()[0] as f64 + z1)
                .clamp(0.0, u8::MAX as f64);
            *(target_img.at_2d_mut::<Vec3b>(x, y).unwrap()) = Vec3b::all(val as u8);

            histogram.add(val as u64);

            let val = (target_img.at_2d::<Vec3b>(x, y + 1).unwrap()[0] as f64 + z2)
                .clamp(0.0, u8::MAX as f64);
            *(target_img.at_2d_mut::<Vec3b>(x, y + 1).unwrap()) = Vec3b::all(val as u8);
            histogram.add(val as u64);
        }
    }

    let param = Vector::new();

    println!("{}", histogram);

    imshow("SRC", &source_img).unwrap();
    imshow("TRG", &target_img).unwrap();

    imwrite("SRC.png", &source_img, &param).unwrap();
    imwrite("TRG.png", &target_img, &param).unwrap();


    wait_key(0).unwrap();
    destroy_all_windows().unwrap();
}
