use crate::linear::Calculate;
use argmin::core::{CostFunction, Gradient, Operator};

pub struct LinearProblem {
    data: Vec<Vec<f64>>,
}

impl LinearProblem {
    pub fn new(data: Vec<Vec<f64>>) -> Self {
        Self { data }
    }
}

impl CostFunction for LinearProblem {
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

impl Gradient for LinearProblem {
    type Param = Vec<f64>;
    type Gradient = Vec<f64>;

    fn gradient(&self, p: &Self::Param) -> Result<Self::Gradient, argmin::core::Error> {
        let mut grad = vec![0.0; p.len()];

        for d in &self.data {
            let mut cur = 1.0;
            for i in 0..p.len() {
                grad[i] += -2.0 * (d[1] - Self::calc(p, d[0])) * cur;
                cur *= d[0];
            }
        }

        Ok(grad)
    }
}

impl Calculate for LinearProblem {
    fn calc(param: &Vec<f64>, x: f64) -> f64 {
        let mut cur = 1.0;
        let mut res = 0.0;

        for p in param {
            res += p * cur;
            cur *= x;
        }

        res
    }
}
