use std::ffi::c_void;
use opencv::core::{CV_8U, hconcat, Mat, Mat_AUTO_STEP, MatTraitConst, Vec3b};
use opencv::highgui::{destroy_all_windows, imshow, wait_key};
use opencv::imgcodecs::{imread, IMREAD_COLOR};

fn main() {
    let img = imread("image.png", IMREAD_COLOR).unwrap();
    imshow("Image", &img).unwrap();

    let mut gray_scale = vec![0; (img.rows() * img.cols()) as usize];

    for r in 0..img.rows() {
        for c in 0..img.cols() {
            let pixel= img.at_2d::<Vec3b>(r, c).unwrap();
            gray_scale[(r * img.cols() + c) as usize] = ((pixel[0] as i32 + pixel[1] as i32 + pixel[2] as i32 ) / 3) as u8;
        }
    }

    unsafe {
        let gray_scale_img = Mat::new_rows_cols_with_data(img.rows(), img.cols(), CV_8U, gray_scale.as_mut_ptr() as *mut c_void, Mat_AUTO_STEP).unwrap();
        imshow("Gray_Scale", &gray_scale_img).unwrap();
    }



    wait_key(0).unwrap();
    destroy_all_windows().unwrap();
}
