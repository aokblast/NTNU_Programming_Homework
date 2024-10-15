mod knn;
mod linear;

use crate::knn::do_knn;
use crate::linear::{do_k_fold, do_schwartz};
use csv::ReaderBuilder;
use std::env::args;
use std::fs::File;
use std::io::Read;

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
enum Party {
    REPUBLIC = 0,
    DEMOCRATIC = 1,
}

impl TryFrom<i32> for Party {
    type Error = String;
    fn try_from(value: i32) -> Result<Self, Self::Error> {
        match value {
            0 => Ok(Party::REPUBLIC),
            1 => Ok(Party::DEMOCRATIC),
            _ => Err(Self::Error::from("Invalid party")),
        }
    }
}

#[derive(Debug, Copy, Clone, PartialEq)]
struct DataPoint {
    x: f64,
    y: Party,
}

fn get_data_from_file(mut file: File) -> Vec<DataPoint> {
    let mut res = vec![];
    let mut raw_data = vec![];
    file.read_to_end(&mut raw_data).unwrap();

    let mut reader = ReaderBuilder::new()
        .has_headers(false)
        .delimiter(b',')
        .from_reader(raw_data.as_slice());

    while let Some(entry) = reader.records().next() {
        let record = entry.unwrap();
        let x = (&record[1][..record[1].len() - 1]).parse::<f64>().unwrap();
        let y = match &record[2] {
            "D" => Party::DEMOCRATIC,
            "R" => Party::REPUBLIC,
            _ => panic!("Invalid party"),
        };

        res.push(DataPoint { x, y });
    }

    res
}
fn get_linear_data_from_file(mut file: File) -> Vec<Vec<f64>> {
    let mut res = vec![];
    let mut raw_data = vec![];
    file.read_to_end(&mut raw_data).unwrap();

    let mut reader = ReaderBuilder::new()
        .has_headers(false)
        .delimiter(b',')
        .from_reader(raw_data.as_slice());

    while let Some(entry) = reader.records().next() {
        let record = entry.unwrap();
        let x = record[0].parse::<f64>().unwrap();
        let y = record[1].parse::<f64>().unwrap();

        res.push(vec![x, y]);
    }

    res
}

fn main() {
    let args = args().collect::<Vec<_>>();
    // Read one from args[0], another from args[1]
    let knn_train_data = File::open(&args[1]).unwrap();
    let knn_validate_data = File::open(&args[2]).unwrap();
    let linear_train_data = File::open(&args[3]).unwrap();

    let knn_train_data = get_data_from_file(knn_train_data);
    let knn_validate_data = get_data_from_file(knn_validate_data);
    let linear_train_data = get_linear_data_from_file(linear_train_data);

    // do_knn(&knn_train_data, &knn_validate_data);
    // do_knn(&knn_validate_data, &knn_train_data);
    // do_schwartz(&linear_train_data);
    do_k_fold(&linear_train_data, 5);
}
