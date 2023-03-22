use ndhistogram::{ndhistogram, axis::Uniform, Histogram};
use plotters::prelude::*;
use std::{env, io::{BufReader, BufRead}, fs::File};

const BINS: usize = 128;
const GIGA: f64 = 1e6;

fn to_cdf(data: &Vec<u64>) -> Vec<(f64, f64)> {
    let mut hist = ndhistogram!(Uniform::new(BINS, *data.iter().min().unwrap() as f64 , (*data.iter().max().unwrap() + 1) as f64));
    let mut res = vec![];

    for d in data {
        hist.fill(&(*d as f64));
    }

    let cnt = data.len();
    let mut cur = 0 as f64;


    for item in hist.iter() {
        let st;
        let ed;

        if let Some(start) = item.bin.start() {
            st = start;
        } else {
            continue;
        }

        if let Some(end) = item.bin.end() {
            ed = end;
        } else {
            continue;
        }

        cur += *item.value;

        res.push(((ed + st) / 2.0 / GIGA, cur / cnt as f64));
    }

    res
}


fn main() -> Result<(), Box<dyn std::error::Error>> {
    let args: Vec<String> = env::args().collect();

    let input_file = &args[1];
    let output_file = &args[2];

    // Collecting Data
    let file_reader = BufReader::new(File::open(input_file).unwrap());

    let mut data: Vec<u64> = vec![];

    for line in file_reader.lines() {
        if let Ok(line) = line {
            if let Ok(num) = line.parse() {
                data.push(num);
            }
        }
    }

    let cdf = to_cdf(&data);

    // Draw Graph
    let root = BitMapBackend::new(output_file, (1024, 768)).into_drawing_area();

    root.fill(&WHITE)?;

    let mut chart = ChartBuilder::on(&root)
        .set_label_area_size(LabelAreaPosition::Left, 60)
        .set_label_area_size(LabelAreaPosition::Bottom, 60)
        .caption("P1_Result", ("sans-serif", 40))
        .build_cartesian_2d((*data.iter().min().unwrap() as f64 / GIGA)..(*data.iter().max().unwrap() as f64 / GIGA), 0.0..1.0)?;

    chart.configure_mesh()
        .disable_x_mesh()
        .disable_y_mesh()
        .x_desc("Response Time (ms)")
        .y_desc("Cumulative Possibility")
        .axis_desc_style(("sans-serif", 25))
        .draw()?;

    chart.draw_series(
        AreaSeries::new(
            cdf.iter().map(|item| { (item.0, item.1) }),
            0.0,
            &RED.mix(0.2),
        ).border_style(&RED),
    )?;

    root.present().unwrap();
    println!("Result has been saved to {}", output_file);

    Ok(())
}