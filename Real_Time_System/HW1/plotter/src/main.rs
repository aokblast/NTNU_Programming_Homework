use ndhistogram::{ndhistogram, axis::Uniform, Histogram};
use plotters::prelude::*;
use std::{env, io::{BufReader, BufRead}, fs::File};
use std::cmp::{max, min};

const BINS: usize = 512;
const GIGA: f64 = 1e6;

const COLORS: [RGBColor; 7] = [BLUE, RED, MAGENTA, CYAN, GREEN, BLACK, YELLOW];

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

    let output_file = &args[1];
    let input_files = &args[2..min(args.len(), COLORS.len())];

    let mut data = vec![];
    let mut boundary = (u64::MAX, u64::MIN);

    for file in input_files {
        // Collecting Data
        let mut d: Vec<u64> = vec![];
        let file_reader = BufReader::new(File::open(file).unwrap());

        for line in file_reader.lines() {
            if let Ok(line) = line {
                if let Ok(num) = line.parse() {
                    d.push(num);
                    boundary.0 = min(boundary.0, num);
                    boundary.1 = max(boundary.1, num);
                }
            }
        }

        data.push(d);
    }

    // Draw Graph
    let root = BitMapBackend::new(output_file, (1024, 768)).into_drawing_area();

    root.fill(&WHITE)?;

    let mut chart = ChartBuilder::on(&root)
        .set_label_area_size(LabelAreaPosition::Left, 60)
        .set_label_area_size(LabelAreaPosition::Bottom, 60)
        .caption("Result", ("sans-serif", 40))
        .build_cartesian_2d((boundary.0 as f64 / GIGA)..((boundary.1 + 1) as f64 / GIGA), 0.0..1.0)?;

    chart.configure_mesh()
        .disable_x_mesh()
        .disable_y_mesh()
        .x_desc("Response Time (ms)")
        .y_desc("Cumulative Probability")
        .axis_desc_style(("sans-serif", 25))
        .draw()?;

    let mut idx = 0;

    for d in data {
        let cdf = to_cdf(&d);
        let cur = idx;
        chart.draw_series(
            LineSeries::new(
                cdf.iter().map(|item| { (item.0, item.1) }),
                &COLORS[idx],
            ))?
            .label(format!("P{}", idx + 1)).legend(move |(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], &COLORS[cur]));
        idx += 1;
    }

    chart.configure_series_labels().border_style(&BLACK).draw()?;

    root.present().unwrap();
    println!("Result has been saved to {}", output_file);

    Ok(())
}