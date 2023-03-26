use opencv::core::{CV_8UC3, Scalar, Vec3b, Vector};
use opencv::highgui::{destroy_all_windows, imshow, wait_key};
use opencv::imgcodecs::{imread, IMREAD_COLOR, imwrite};
use opencv::prelude::*;

fn filter(src: &Mat, func: fn(Mat, &Vec3b) -> Vec3b, n: i32) -> Mat {
    let mut result = Mat::new_size_with_default(src.size().unwrap(), CV_8UC3, Scalar::all(0 as f64)).unwrap();

    for x in 0..src.rows() {
        for y in 0..src.cols() {
            let pixels;

            if let Ok(neighbor) = src.row_bounds(x - n / 2, x + n / 2 + 1).and_then(|row| {row.col_bounds(y - n / 2, y + n / 2 + 1)}) {
                pixels = neighbor;
            } else {
                *(result.at_2d_mut::<Vec3b>(x, y).unwrap()) = *(src.at_2d::<Vec3b>(x, y).unwrap());
                continue;
            }

            *(result.at_2d_mut::<Vec3b>(x, y).unwrap()) = func(pixels, src.at_2d::<Vec3b>(x, y).unwrap());
        }
    }

    result
}

fn average_filter(src: Mat, _cur: &Vec3b) -> Vec3b {
    let tot = (src.rows() * src.cols()) as f64;
    let mut cnt = [0 as f64; 3];

    for (_, pixel) in src.iter::<Vec3b>().unwrap() {
        cnt[0] += pixel[0] as f64 / tot;
        cnt[1] += pixel[1] as f64 / tot;
        cnt[2] += pixel[2] as f64 / tot;
    }

    Vec3b::from(cnt.map(|x|{x as u8}))
}

fn median_filter(src: Mat, _cur: &Vec3b) -> Vec3b {
    let mut cnt: [Vec<u8>; 3] = Default::default();

    for (_, pixel) in src.iter::<Vec3b>().unwrap() {
        cnt[0].push(pixel[0]);
        cnt[1].push(pixel[1]);
        cnt[2].push(pixel[2]);
    }


    for x in &mut cnt {
        x.sort();
    }

    Vec3b::from(cnt.map(|x|{x[x.len() / 2]}))
}

fn unsharp_mask(src: Mat, cur: &Vec3b) -> Vec3b {
    let tot = (src.rows() * src.cols()) as f64;
    let mut cnt = [0 as f64; 3];

    for (_, pixel) in src.iter::<Vec3b>().unwrap() {
        cnt[0] -= pixel[0] as f64 / tot;
        cnt[1] -= pixel[1] as f64 / tot;
        cnt[2] -= pixel[2] as f64 / tot;
    }

    for idx in 0..cnt.len() {
        cnt[idx] = (cnt[idx] + cur[idx] as f64) * tot + cur[idx] as f64;
    }


    Vec3b::from(cnt.map(|x|{(x) as u8}))
}



fn main() {
    let img = imread("ccw.jpg", IMREAD_COLOR).unwrap();

    let avg_filter_img = filter(&img, average_filter, 3);
    let median_filter_img = filter(&img, median_filter, 3);
    let unsharp_img = filter(&img, unsharp_mask, 3);


    imshow("Origin", &img);
    imshow("Avg", &avg_filter_img).unwrap();
    imshow("Median", &median_filter_img).unwrap();
    imshow("Unsharp", &unsharp_img).unwrap();

    let param = Vector::new();

    imwrite("Origin.png", &img, &param).unwrap();
    imwrite("Avg.png", &avg_filter_img, &param).unwrap();
    imwrite("Median.png", &median_filter_img, &param).unwrap();
    imwrite("Unsharp.png", &unsharp_img, &param).unwrap();

    wait_key(0).unwrap();
    destroy_all_windows().unwrap();
}
