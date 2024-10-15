use crate::linear::Calculate;
use argmin::core::{CostFunction, Gradient};
use std::f32::consts::PI;

pub struct TrigonometricProblem {
    data: Vec<Vec<f64>>,
}

impl TrigonometricProblem {
    pub fn new(data: Vec<Vec<f64>>) -> Self {
        Self { data }
    }
}

impl CostFunction for TrigonometricProblem {
    type Param = Vec<f64>;
    type Output = f64;

    fn cost(&self, p: &Self::Param) -> Result<Self::Output, argmin::core::Error> {
        let mut sum = 0.0;

        for d in &self.data {
            let y_pred = Self::calc(p, d[0]);
            sum += (d[1] - y_pred) * (d[1] - y_pred);
        }

        Ok(sum)
    }
}

impl Gradient for TrigonometricProblem {
    type Param = Vec<f64>;
    type Gradient = Vec<f64>;

    fn gradient(&self, p: &Self::Param) -> Result<Self::Gradient, argmin::core::Error> {
        let mut grad = vec![0.0; p.len()];

        for i in 0..p.len() {
            for d in &self.data {
                grad[i] +=
                    -2.0 * (d[1] - Self::calc(p, d[0])) * (2.0 * PI as f64 * i as f64 * d[0]).cos();
            }
        }

        Ok(grad)
    }
}

impl Calculate for TrigonometricProblem {
    fn calc(param: &Vec<f64>, x: f64) -> f64 {
        let mut res = 0.0;
        let mut idx = 0;

        for p in param {
            res += p * (2.0 * PI as f64 * idx as f64 * x).cos();
            idx += 1;
        }

        res
    }
}
