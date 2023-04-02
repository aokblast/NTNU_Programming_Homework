use std::f64::consts::PI;
use opencv::core::{CV_8UC3, Rect2i, Scalar, Size, Vec3b, Vector};
use opencv::highgui::{destroy_all_windows, imshow, wait_key};
use opencv::imgcodecs::imwrite;
use opencv::prelude::*;

const HEIGHT: f64 = 800.0;
const WIDTH: f64 = 1200.0;
const DEG: f64 = (30.0 / 180.0) * PI;

fn filter(src: &Mat, result: &mut Mat, func: fn(mat: &Mat, x: f64, y: f64) -> Vec3b) {
    let h = result.rows() / 2;
    let w = result.cols() / 2;

    for y in 0..result.rows() {
        for x in 0..result.cols() {
            let a = ((w - x) as f64 * DEG.cos()) + ((y - h) as f64 * DEG.sin()) + (src.cols() / 2) as f64;
            let b = ((w - x) as f64 * -DEG.sin()) + ((y - h) as f64 * DEG.cos()) + (src.rows() / 2) as f64;

            *(result.at_2d_mut(y, x).unwrap()) = func(src, b, a);
        }
    }
}

fn neighbor_filter(mat: &Mat, x: f64, y: f64) -> Vec3b {
    *(mat.at_2d::<Vec3b>((x.round() as i32).max(0).min(mat.rows() - 1), (y.round() as i32).max(0).min(mat.cols() - 1)).unwrap())
}

fn bilinear_filter(mat: &Mat, x: f64, y: f64) -> Vec3b {
    let x1 = (x.floor() as i32).max(0).min(mat.rows() - 1);
    let x2 = (x.ceil() as i32).max(0).min(mat.rows() - 1);
    let y1 = (y.floor() as i32).max(0).min(mat.cols() - 1);
    let y2 = (y.ceil() as i32).max(0).min(mat.cols() - 1);

    let p1 = mat.at_2d::<Vec3b>(x1, y1).unwrap();
    let p2 = mat.at_2d::<Vec3b>(x2, y1).unwrap();
    let p3 = mat.at_2d::<Vec3b>(x1, y2).unwrap();
    let p4 = mat.at_2d::<Vec3b>(x2, y2).unwrap();

    let mut m1 = Vec3b::all(0);
    let mut m2 = Vec3b::all(0);


    if x1 == x2 {
        m1 = *p1;
        m2 = *p3;

    } else {
        for i in 0..p1.len() {
            m1[i] = ((p1[i] as f64 - p2[i] as f64) * (x - x2 as f64) / (x1 - x2) as f64 + p2[i] as f64) as u8;
        }

        for i in 0..p2.len() {
            m2[i] = ((p3[i] as f64 - p4[i] as f64) * (x - x2 as f64) / (x1 - x2) as f64 + p4[i] as f64) as u8;
        }
    }

    let mut res = Vec3b::all(0);

    if y1 == y2 {
        res = m1;
    } else {
        for i in 0..res.len() {
            res[i] = ((m2[i] as f64 - m1[i] as f64) * (y - y1 as f64) / (y2 - y1) as f64 + m1[i] as f64) as u8;
        }
    }

    res
}


fn main() {

    let source_img = Mat::new_size_with_default(Size::new(WIDTH as i32, HEIGHT as i32), CV_8UC3, Scalar::all(0 as f64)).unwrap();
    let mut neighbor_img = Mat::new_size_with_default(Size::new(WIDTH as i32, HEIGHT as i32), CV_8UC3, Scalar::all(0 as f64)).unwrap();
    let mut bilinear_img = Mat::new_size_with_default(Size::new(WIDTH as i32, HEIGHT as i32), CV_8UC3, Scalar::all(0 as f64)).unwrap();

    source_img.apply_1(Rect2i::new(250, 150, 700, 500)).unwrap().set(Scalar::all(255 as f64)).unwrap();

    filter(&source_img, &mut neighbor_img, neighbor_filter);
    filter(&source_img, &mut bilinear_img, bilinear_filter);

    imshow("Orig", &source_img).unwrap();
    imshow("Neighbor", &neighbor_img).unwrap();
    imshow("Bilinear", &bilinear_img).unwrap();

    let param = Vector::new();

    imwrite("orig.png", &source_img, &param).unwrap();
    imwrite("Neighbor.png", &neighbor_img, &param).unwrap();
    imwrite("Bilinear.png", &bilinear_img, &param).unwrap();

    wait_key(0).unwrap();
    destroy_all_windows().unwrap();
}
