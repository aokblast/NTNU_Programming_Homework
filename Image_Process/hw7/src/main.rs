use opencv::core::{CV_8UC1, Scalar, VecN, Vector};
use opencv::highgui::{destroy_all_windows, imshow, wait_key};
use opencv::imgcodecs::{imread, IMREAD_GRAYSCALE, imwrite};
use opencv::prelude::*;

fn main() {
    let img = imread("image.png", IMREAD_GRAYSCALE).unwrap();
    let mut result = Mat::new_size_with_default(img.size().unwrap()
                                            , CV_8UC1, Scalar::all(0.0)).unwrap();

    let tot = img.rows() * img.cols();

    let mut mp = [0; 256];


    for (_point, pixel) in img.iter::<VecN<u8, 1>>().unwrap() {
        mp[pixel[0] as usize] += 1;
    }

    let mut mp = mp.map(f64::from);
    let mut ma = [0.0; 256];

    for idx in 1..mp.len() {
        mp[idx] /= tot as f64;
        ma[idx] = ma[idx - 1] + mp[idx] * idx as f64;
        mp[idx] += mp[idx - 1];
    }

    let (mut t, mut mx) = (0, 0.0);

    for idx in 1..mp.len() {
        let fz = ma[idx] - ma[255] * mp[idx];
        let cur  = fz * fz / (mp[idx] * (mp[255] - mp[idx]));

        if mx < cur && cur.is_finite() {
            mx = cur;
            t = idx;
        }
    }

    let t = t as u8;

    for (point, pixel) in img.iter::<VecN<u8, 1>>().unwrap() {
        (result.at_2d_mut::<VecN<u8, 1>>(point.y, point.x).unwrap())[0]
            = if pixel[0] >= t { 255 } else { 0 };
    }

    imshow("SRC", &img).unwrap();
    imshow("RES", &result).unwrap();

    let param = Vector::new();
    imwrite("src.png", &img, &param).unwrap();
    imwrite("res.png", &result, &param).unwrap();

    wait_key(0).unwrap();
    destroy_all_windows().unwrap();
}
