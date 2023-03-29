use opencv::core::{CV_8UC3, Scalar, Vec3b, Vector};
use opencv::highgui::{destroy_all_windows, imshow, wait_key};
use opencv::imgcodecs::{imread, IMREAD_COLOR, imwrite};
use opencv::prelude::*;
use quick_select::quick_select;

const K: f64 = 0.5;

fn filter(src: &Mat, func: fn(Mat, &Vec3b) -> Vec3b, n: i32) -> Mat {
    let mut result = Mat::new_size_with_default(src.size().unwrap(), CV_8UC3, Scalar::all(0 as f64)).unwrap();

    for (pos, pixel) in src.iter::<Vec3b>().unwrap() {
        let x = pos.y;
        let y = pos.x;
        if let Ok(neighbor) = src.row_bounds(x - n / 2, x + n / 2 + 1).and_then(|row| {row.col_bounds(y - n / 2, y + n / 2 + 1)}) {
            *(result.at_2d_mut::<Vec3b>(x, y).unwrap()) = func(neighbor, &pixel.clone());
        } else {
            *(result.at_2d_mut::<Vec3b>(x, y).unwrap()) = pixel.clone();
        }

    }

    result
}

fn add_weight(src1: &Mat, alpha: f64, src2: &Mat, beta: f64) -> Mat {
    let mut result = Mat::new_size_with_default(src1.size().unwrap(), CV_8UC3, Scalar::all(0 as f64)).unwrap();

    for x in 0..src1.rows() {
        for y in 0..src1.cols() {
            let res = result.at_2d_mut::<Vec3b>(x, y).unwrap();
            let a = src1.at_2d::<Vec3b>(x, y).unwrap();
            let b = src2.at_2d::<Vec3b>(x, y).unwrap();

            res[0] = (a[0] as f64 * alpha + b[0] as f64 * beta) as u8;
            res[1] = (a[1] as f64 * alpha + b[1] as f64 * beta) as u8;
            res[2] = (a[2] as f64 * alpha + b[2] as f64 * beta) as u8;
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

    Vec3b::from(cnt.map(|mut x|{
        let l = x.len();
        *quick_select(&mut x, l / 2)
    }))
}

fn unsharp_mask(src: Mat, cur: &Vec3b) -> Vec3b {
    let tot = (src.rows() * src.cols()) as f64;
    let mut cnt = [0 as i32; 3];

    for (_, pixel) in src.iter::<Vec3b>().unwrap() {
        cnt[0] += pixel[0] as i32;
        cnt[1] += pixel[1] as i32;
        cnt[2] += pixel[2] as i32;
    }

    for idx in 0..cnt.len() {
        cnt[idx] = ((1.0 + K) * cur[idx] as f64 - (cnt[idx] as f64 / tot) * K) as i32;
    }


    Vec3b::from(cnt.map(|x|{x as u8}))
}

fn unsharp_median(src: Mat, cur: &Vec3b) -> Vec3b {
    let mut cnt: [Vec<u8>; 3] = Default::default();
    let mut res = [0 as i32; 3];

    for (_, pixel) in src.iter::<Vec3b>().unwrap() {
        cnt[0].push(pixel[0]);
        cnt[1].push(pixel[1]);
        cnt[2].push(pixel[2]);
    }

    for idx in 0..cur.len() {
        let l = cnt[idx].len();
        res[idx] = ((1.0 + K) * cur[idx] as f64 - (*quick_select(&mut cnt[idx], l / 2)) as f64 * K) as i32;
    }

    Vec3b::from(res.map(|x|{x as u8}))
}


fn main() {
    let img = imread("image.png", IMREAD_COLOR).unwrap();

    let avg_filter_img = filter(&img, average_filter, 5);
    let median_filter_img = filter(&img, median_filter, 5);
    let unsharp_img = add_weight(&img, (1.0 + K), &avg_filter_img, -K);
    let unsharp_median_img = add_weight(&img, (1.0 + K),&median_filter_img, -K);

    imshow("Origin", &img).unwrap();
    imshow("Avg", &avg_filter_img).unwrap();
    imshow("Median", &median_filter_img).unwrap();
    imshow("Unsharp", &unsharp_img).unwrap();
    imshow("Unsharp_median", &unsharp_median_img).unwrap();

    let param = Vector::new();

    imwrite("Origin.png", &img, &param).unwrap();
    imwrite("Avg.png", &avg_filter_img, &param).unwrap();
    imwrite("Median.png", &median_filter_img, &param).unwrap();
    imwrite("Unsharp.png", &unsharp_img, &param).unwrap();
    imwrite("Unsharp_median.png",&unsharp_median_img, &param ).unwrap();

    wait_key(0).unwrap();
    destroy_all_windows().unwrap();
}
