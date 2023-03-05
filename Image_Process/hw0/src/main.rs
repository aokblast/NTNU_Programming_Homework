use opencv::highgui::{destroy_all_windows, imshow, wait_key};
use opencv::imgcodecs::{imread, IMREAD_COLOR};

fn main() {
    let img = imread("ccw.jpg", IMREAD_COLOR).unwrap();


    imshow("CCW", &img).unwrap();

    wait_key(0).unwrap();
    destroy_all_windows().unwrap();
}
