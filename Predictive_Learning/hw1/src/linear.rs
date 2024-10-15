mod lin;
mod tri;

use crate::linear::lin::LinearProblem;
use crate::linear::tri::TrigonometricProblem;
use argmin::core::{CostFunction, Executor, Gradient, State};
use argmin::solver::gradientdescent::SteepestDescent;
use argmin::solver::linesearch::MoreThuenteLineSearch;

trait ProblemFactory {
    type Problem: CostFunction<Param = Vec<f64>, Output = f64>
        + Gradient<Param = Vec<f64>, Gradient = Vec<f64>>
        + Calculate;
    fn generate(&self) -> Self::Problem;

    fn get_data(&self) -> &Vec<Vec<f64>>;
}

trait Calculate {
    fn calc(param: &Vec<f64>, x: f64) -> f64;
}

struct TrigonometricProblemFactory<'a>(&'a Vec<Vec<f64>>);

impl ProblemFactory for TrigonometricProblemFactory<'_> {
    type Problem = TrigonometricProblem;
    fn generate(&self) -> Self::Problem {
        TrigonometricProblem::new(self.0.clone())
    }

    fn get_data(&self) -> &Vec<Vec<f64>> {
        &self.0
    }
}

struct LinearProblemFactory<'a>(&'a Vec<Vec<f64>>);

impl ProblemFactory for LinearProblemFactory<'_> {
    type Problem = LinearProblem;
    fn generate(&self) -> Self::Problem {
        LinearProblem::new(self.0.clone())
    }

    fn get_data(&self) -> &Vec<Vec<f64>> {
        &self.0
    }
}

fn do_schwartz_internal<T>(generator: T, test_data: &Vec<Vec<f64>>, log: bool) -> (i32, f64, f64)
where
    T: ProblemFactory,
{
    let len = generator.get_data().len();
    let mut k_res = 0;
    let mut rme_res = f64::MAX;
    let mut mse_res = 0.0;

    let calc_score = |k: usize| {
        if k == len {
            return 241213421123123.1;
        }
        let p = k as f64 / len as f64;
        1.0 + p / (1.0 - p) * (len as f64).ln()
    };

    if log {
        println!("Schwartz:");
        println!("m, Loss, Schwartz, R")
    }

    for k in 1..len {
        let problem = generator.generate();
        let mut init_param = vec![];
        let line_search = MoreThuenteLineSearch::new();
        let solver = SteepestDescent::new(line_search);

        for _ in 0..k {
            init_param.push(1.0);
        }

        let runner = Executor::new(problem, solver)
            .configure(|state| state.param(init_param).max_iters(1000).target_cost(0.0))
            .run()
            .unwrap();

        let param = runner.state.get_best_param().unwrap();
        let mut mse = 0.0;
        let score = calc_score(k);

        for d in test_data {
            let y_pred = T::Problem::calc(&param, d[0]);
            mse += (y_pred - d[1]).powi(2);
        }

        let rme = mse * score;

        if rme_res > rme {
            rme_res = rme;
            k_res = k as i32;
            mse_res = mse;
        }

        if log {
            println!("{}, {}, {}, {}", k, mse, score, rme);
        }
    }

    (k_res, mse_res, rme_res)
}

pub fn do_schwartz(train_data: &Vec<Vec<f64>>) {
    let test_data = train_data.clone();
    println!("Trigonometric:");
    do_schwartz_internal(TrigonometricProblemFactory(train_data), &test_data, true);
    println!("Linear:");
    do_schwartz_internal(LinearProblemFactory(train_data), &test_data, true);
}

fn do_k_fold_internal<T>(
    data: &Vec<Vec<f64>>,
    k: usize,
    _: T,
    do_train: fn(Vec<Vec<f64>>, test_data: Vec<Vec<f64>>) -> (i32, f64, f64),
) where
    T: ProblemFactory,
{
    let len = data.len();
    let step = len / k;
    println!("knn:");
    println!("k, mse, rme:");

    for i in (0..len).step_by(step) {
        let mut train_data = vec![];
        let mut test_data = vec![];

        for j in 0..len {
            if j >= i && j < i + step {
                test_data.push(data[j].clone());
            } else {
                train_data.push(data[j].clone());
            }
        }

        let (k, mse, rme) = do_train(train_data, test_data);
        println!("{k}, {mse}, {rme}");
    }
}

pub fn do_k_fold(train_data: &Vec<Vec<f64>>, k: usize) {
    let data = vec![];
    println!("Trigonometric:");
    do_k_fold_internal(
        train_data,
        k,
        TrigonometricProblemFactory(&data),
        |train_data, test_data: Vec<Vec<f64>>| {
            do_schwartz_internal(TrigonometricProblemFactory(&train_data), &test_data, false)
        },
    );
    println!("Linear:");
    do_k_fold_internal(
        train_data,
        k,
        LinearProblemFactory(&data),
        |train_data, test_data: Vec<Vec<f64>>| {
            do_schwartz_internal(LinearProblemFactory(&train_data), &test_data, false)
        },
    );
}
