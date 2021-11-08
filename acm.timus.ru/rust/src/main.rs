const TEST_INPUT: &str = r#"test input"#;
const TEST_OUTPUT: &str = r#"test output"#;

use std::io::prelude::*;
use std::io::BufReader;
use std::vec::Vec;

fn read_input() -> (Vec<String>, Option<String>) {
    let args: Vec<_> = std::env::args().collect();
    if args.len() > 1 {
        // test
        let lines: Vec<_> = TEST_INPUT.split('\n').map(|s| s.to_string()).collect();
        (lines, Some(TEST_OUTPUT.to_string()))
    } else {
        // judge
        let mut input: Vec<_> = Vec::new();
        let reader = BufReader::new(std::io::stdin());
        for line in reader.lines() {
            match line {
                Ok(line) => input.push(line),
                Err(_) => {}
            }
        }
        (input, None)
    }
}

fn run(input: Vec<String>) -> Vec<String> {
    // TODO: do work here
    input
}

fn main() {
    let (input, _expected_output) = read_input();
    match _expected_output {
        None => run(input).into_iter().for_each(move |s| println!("{}", s)),
        Some(ref s) => {
            let output = run(input)
                .into_iter()
                .reduce(|sum, x| sum + "\n" + &x)
                .unwrap();
            if output != *s {
                println!("got output:\n{:?}\nexpected output:\n{:?}", output, s);
            }
        }
    }
}
