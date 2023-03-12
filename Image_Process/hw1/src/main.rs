use opencv::core::{CV_8UC3, hconcat2, Scalar, Vec3b, Vector};
use opencv::highgui::{destroy_all_windows, imshow, wait_key};
use opencv::imgcodecs::{imread, IMREAD_COLOR, imwrite};
use opencv::prelude::*;

fn main() {
    let source_image = imread("ccm.jpeg", IMREAD_COLOR).unwrap();

    let mut a_img = Mat::new_size_with_default(
        source_image.size().unwrap(), CV_8UC3, Scalar::all(0 as f64)).unwrap();

    let mut b_img = Mat::new_size_with_default(
        source_image.size().unwrap(), CV_8UC3, Scalar::all(0 as f64)).unwrap();

    let d1 = vec![vec![0u8, 128, 32, 160]
        , vec![192, 64, 224, 96]
        , vec![48, 176, 16, 144]
        , vec![240, 112, 208, 80]];

    let d2 = vec![vec![0u8, 56], vec![84, 28]];

    let for_each_pixel = |source_image: &Mat, result_image: &mut Mat, d: &Vec<Vec<u8>>, handler: fn(u8, u8) -> u8 | {
        for x in (0..source_image.rows()).step_by(d.len()) {
            for y in (0..source_image.cols()).step_by(d[0].len()) {
                for i in 0..(d.len() as i32) {
                    for j in 0..(d[0].len() as i32) {
                        let current_pixel;
                        let res_pixel;

                        if let Ok(cp) = source_image.at_2d::<Vec3b>(x + i, y + j) {
                            current_pixel = cp;
                        } else {
                            continue;
                        }

                        if let Ok(cp) = result_image.at_2d_mut::<Vec3b>(x + i, y + j) {
                            res_pixel = cp;
                        } else {
                            continue;
                        }

                        let c = handler(current_pixel[0], d[i as usize][j as usize] as u8);

                        res_pixel[0] = c;
                        res_pixel[1] = c;
                        res_pixel[2] = c;
                    }
                }
            }
        }
    };

    for_each_pixel(&source_image, &mut a_img, &d1
            , |c, d|{ if c > d {255} else {0}});

    for_each_pixel(&source_image, &mut b_img, &d2
                   , |c, d|{ (c / 85 + (if (c % 85) > d { 1 } else { 0 })) * 85 });

    let mut a_res = Mat::new_rows_cols_with_default(source_image.rows()
        , source_image.cols() * 2, CV_8UC3, Scalar::all(0 as f64)).unwrap();

    let mut b_res = Mat::new_rows_cols_with_default(source_image.rows()
        , source_image.cols() * 2, CV_8UC3, Scalar::all(0 as f64)).unwrap();

    hconcat2(&source_image, &a_img, &mut a_res).unwrap();
    hconcat2(&source_image, &b_img, &mut b_res).unwrap();

    imshow("A", &a_res).unwrap();
    imshow("B", &b_res).unwrap();

    imwrite("res_A.jpg", &a_res, &Vector::<i32>::new()).unwrap();
    imwrite("res_B.jpg", &b_res, &Vector::<i32>::new()).unwrap();

    wait_key(0).unwrap();
    destroy_all_windows().unwrap();
}
