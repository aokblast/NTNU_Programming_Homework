use histo::Histogram;
use opencv::core::{CV_8UC1, CV_8UC3, Mat, Scalar, Vec3b, VecN, Vector};
use opencv::highgui::{destroy_all_windows, imshow, wait_key};
use opencv::imgcodecs::{imread, IMREAD_COLOR, imwrite};
use opencv::imgproc::{COLOR_BGR2GRAY, cvt_color};
use opencv::prelude::{MatTraitConstManual, MatTraitConst, MatTrait};

fn main() {
    let image = imread("image.png", IMREAD_COLOR).unwrap();
    let mut gray_scale_img = Mat::new_size_with_default(image.size().unwrap(), CV_8UC1, Scalar::all(0 as f64)).unwrap();
    let mut he_gray_scale_img = Mat::new_size_with_default(image.size().unwrap(), CV_8UC1, Scalar::all(0 as f64)).unwrap();
    let mut he_img = Mat::new_size_with_default(image.size().unwrap(), CV_8UC3, Scalar::all(0 as f64)).unwrap();

    cvt_color(&image, &mut gray_scale_img, COLOR_BGR2GRAY, 0).unwrap();

    let mut histogram  = Histogram::with_buckets(20);

    let mut color_map = [0 as f64; 256];
    let tot = image.rows() * image.cols();

    for x in 0..image.rows() {
        for y in 0..image.cols() {
            let pixel = gray_scale_img.at_2d::<VecN<u8, 1>>(x, y).unwrap();
            color_map[pixel[0] as usize] += 1 as f64;
            histogram.add(pixel[0] as u64);
        }
    }

    for cnt in &mut color_map {
        *cnt /= tot as f64;
    }

    for i in 1..color_map.len() {
        color_map[i] *= 255 as f64;
        color_map[i] += color_map[i - 1];
    }

    for x in 0..image.rows() {
        for y in 0..image.cols() {
            let orig_gray_scale_pixel = gray_scale_img.at_2d::<VecN<u8, 1>>(x, y).unwrap();
            let orig_pixel = image.at_2d::<Vec3b>(x, y).unwrap();
            let gray_scale_pixel = he_gray_scale_img.at_2d_mut::<VecN<u8, 1>>(x, y).unwrap();
            let pixel = he_img.at_2d_mut::<Vec3b>(x, y).unwrap();

            gray_scale_pixel[0] = color_map[orig_gray_scale_pixel[0] as usize].round() as u8;

            let amplify = gray_scale_pixel[0] as f64 / orig_gray_scale_pixel[0] as f64;
            pixel[0] = (orig_pixel[0] as f64 * amplify).round() as u8;
            pixel[1] = (orig_pixel[1] as f64 * amplify).round() as u8;
            pixel[2] = (orig_pixel[2] as f64 * amplify).round() as u8;
        }
    }

    println!("{}", histogram);

    imshow("Orig Image", &image).unwrap();
    imshow("Orig Gray Scale Image", &gray_scale_img).unwrap();
    imshow("HE Image", &he_img).unwrap();
    imshow("HE Gray scale Image", &he_gray_scale_img).unwrap();

    let param = Vector::<i32>::new();

    imwrite("orig_img.png", &image, &param).unwrap();
    imwrite("orig_gray_scale_img.png", &gray_scale_img, &param).unwrap();
    imwrite("he_img.png", &he_img, &param).unwrap();
    imwrite("he_gray_scale_img.png", &he_gray_scale_img, &param).unwrap();

    wait_key(0).unwrap();
    destroy_all_windows().unwrap();
}
