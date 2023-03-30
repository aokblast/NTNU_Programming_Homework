use opencv::prelude::*;
use opencv::core::{CV_8UC3, hconcat2, Mat, Scalar, Vec3b, Vector};
use opencv::highgui::{destroy_all_windows, imshow, wait_key};
use opencv::imgcodecs::{imread, IMREAD_COLOR, imwrite};

fn main() {
    let img = imread("image.png", IMREAD_COLOR).unwrap();

    let mut gray_scale_img = Mat::new_size_with_default(
        img.size().unwrap(), CV_8UC3, Scalar::all(0 as f64)).unwrap();

    for r in 0..img.rows() {
        for c in 0..img.cols() {
            let pixel= img.at_2d::<Vec3b>(r, c).unwrap();
            let cur = ((pixel[0] as i32 + pixel[1] as i32 + pixel[2] as i32 ) / 3) as u8;
            let cur_pixel = gray_scale_img.at_2d_mut::<Vec3b>(r, c).unwrap();
            cur_pixel[0] = cur;
            cur_pixel[1] = cur;
            cur_pixel[2] = cur;
        }
    }

    let mut res = Mat::new_rows_cols_with_default(img.rows(), img.cols() * 2, CV_8UC3, Scalar::all(0 as f64)).unwrap();

    hconcat2(&img, &gray_scale_img, &mut res).unwrap();

    imshow("Image", &res).unwrap();
    imwrite("res.jpg", &res, &Vector::<i32>::new()).unwrap();



    wait_key(0).unwrap();
    destroy_all_windows().unwrap();
}
