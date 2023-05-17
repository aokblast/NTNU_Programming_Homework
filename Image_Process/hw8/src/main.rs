use opencv::core::{CV_8U, Mat, Scalar, VecN, Vector};
use opencv::highgui::{imshow, wait_key};
use opencv::imgcodecs::{imread, IMREAD_GRAYSCALE, imwrite};
use opencv::prelude::*;

fn dilation(src: &Mat, filter: &Vec<Vec<u8>> ) -> Mat {
    let mut result = Mat::new_size_with_default(src.size().unwrap(), CV_8U, Scalar::all(0 as f64)).unwrap();

    for x in 0..src.rows() {
        for y in 0..src.cols() {
            let mut cur = 0 as u8;
            for i in (-(filter.len() as i32) / 2)..(filter.len() as i32 / 2 + 1) {
                for j in (-(filter[0].len() as i32) / 2)..(filter[0].len() as i32 / 2 + 1) {
                    if let Ok(s) = src.at_2d::<VecN<u8, 1>>(x + i, y + j) {
                        if filter[(i + filter.len() as i32 / 2) as usize][(j + filter[0].len() as i32 / 2) as usize] == 255 && s[0] == 255 {
                            cur = 255;
                        }
                    } else {
                        break;
                    }
                }
            }
            (result.at_2d_mut::<VecN<u8, 1>>(x, y).unwrap())[0] = cur;
        }
    }

    result
}

fn erosion(src: &Mat, filter: &Vec<Vec<u8>>) -> Mat {
    let mut result = Mat::new_size_with_default(src.size().unwrap(), CV_8U, Scalar::all(0 as f64)).unwrap();

    for x in 0..src.rows() {
        for y in 0..src.cols() {
            let mut cur = 255 as u8;
            for i in (-(filter.len() as i32) / 2)..(filter.len() as i32 / 2 + 1) {
                for j in (-(filter[0].len() as i32) / 2)..(filter[0].len() as i32 / 2 + 1) {
                    if let Ok(s) = src.at_2d::<VecN<u8, 1>>(x + i, y + j) {
                        if filter[(i + filter.len() as i32 / 2) as usize][(j + filter[0].len() as i32 / 2) as usize] == 255 && s[0] == 0 {
                            cur = 0;
                        }
                    } else {
                        cur = 0;
                        break;
                    }
                }
            }
            (result.at_2d_mut::<VecN<u8, 1>>(x, y).unwrap())[0] = cur;
        }
    }

    result
}

fn opening(src: &Mat, filter: &Vec<Vec<u8>>) -> Mat {
    dilation(&erosion(src, filter), filter)
}

fn minus(lhs: &Mat, rhs: &Mat) -> Mat {
    let mut result = Mat::new_size_with_default(lhs.size().unwrap(), CV_8U, Scalar::all(0 as f64)).unwrap();

    for x in 0..lhs.rows() {
        for y in 0..lhs.cols() {
            if lhs.at_2d::<VecN<u8, 1>>(x, y).unwrap()[0] == 255
                && rhs.at_2d::<VecN<u8, 1>>(x, y).unwrap()[0] == 0 {
                (result.at_2d_mut::<VecN<u8, 1>>(x, y).unwrap())[0] = 255;
            }
        }
    }

     result
}

fn add_inplace(lhs: &mut Mat, rhs: &Mat) {
    for x in 0..lhs.rows() {
        for y in 0..lhs.cols() {
            if rhs.at_2d::<VecN<u8, 1>>(x, y).unwrap()[0] == 255 {
                (lhs.at_2d_mut::<VecN<u8, 1>>(x, y).unwrap())[0] = 255;
            }
        }
    }
}


fn preprocess(src: &mut Mat) {
    for x in 0..src.rows() {
        for y in 0..src.cols() {
            (src.at_2d_mut::<VecN<u8, 1>>(x, y).unwrap())[0] =
                if (src.at_2d::<VecN<u8, 1>>(x, y).unwrap())[0] > 128 {255} else {0};
        }
    }
}

fn check_all_zero(src: &Mat) -> bool {
    for x in 0..src.rows() {
        for y in 0..src.cols() {
            if (src.at_2d::<VecN<u8, 1>>(x, y).unwrap())[0] != 0 {
                return false;
            }
        }
    }
    true
}

fn main() {
    let mut img = imread("src.png", IMREAD_GRAYSCALE).unwrap();
    let mut result = Mat::new_size_with_default(img.size().unwrap(), CV_8U, Scalar::all(0 as f64)).unwrap();
    let filter = vec![vec![0, 255 as u8, 0], vec![255, 255, 255], vec![0, 255, 0]];

    let param  = Vector::new();

    preprocess(&mut img);
    imshow("SRC", &img).unwrap();
    imwrite("src.png", &img, &param).unwrap();

    loop {
        let op = opening(&img, &filter);

        if check_all_zero(&op) {
            break;
        }

        add_inplace(&mut result, &minus(&img, &op));
        img = erosion(&img, &filter);
    }


    imshow("RES", &result).unwrap();
    imwrite("res.png", &result, &param).unwrap();
    wait_key(0).unwrap();
}
