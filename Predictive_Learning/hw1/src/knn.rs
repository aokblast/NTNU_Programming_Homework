use crate::{DataPoint, Party};
use knn::PointCloud;

pub fn do_knn(train_data: &Vec<DataPoint>, validate_data: &Vec<DataPoint>) {
    let dist = |a: &DataPoint, b: &DataPoint| {
        return (a.x - b.x).abs();
    };

    let get_result_by_predict = |points: &[&DataPoint]| -> Party {
        let mut statistic = [0; 2];
        for p in points {
            statistic[p.y as usize] += 1;
        }

        if statistic[0] > statistic[1] {
            Party::try_from(0).unwrap()
        } else {
            Party::try_from(1).unwrap()
        }
    };

    let get_score_with_k = |k| {
        let mut true_count = 0;

        for p in train_data {
            let mut pc = PointCloud::new(dist);

            for p2 in train_data {
                if p != p2 {
                    pc.add_point(p2);
                }
            }

            let points = pc.get_nearest_k(p, k);
            let ps: Vec<&DataPoint> = points.iter().map(|p| p.1).collect();
            let result = get_result_by_predict(&ps);

            true_count += (result == p.y) as i32;
        }

        return true_count as f64 / train_data.len() as f64;
    };

    let get_correctness_with_k = |k| {
        let mut true_count = 0;
        let mut pc = PointCloud::new(dist);

        for p in train_data {
            pc.add_point(p);
        }

        for p in validate_data {
            let points = pc.get_nearest_k(p, k);
            let ps: Vec<&DataPoint> = points.iter().map(|p| p.1).collect();
            let result = get_result_by_predict(&ps);
            true_count += (result == p.y) as i32;
        }

        return true_count as f64 / validate_data.len() as f64;
    };

    println!("k, validation_error, test_error:");

    // Get all score by LOO of knn by LOO
    for k in 1..train_data.len() {
        let scores = get_score_with_k(k);
        let train_correctness = get_correctness_with_k(k);
        println!("{}, {:?}, {:?}", k, 1.0 - scores, 1.0 - train_correctness);
    }
}
