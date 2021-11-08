const TEST_INPUT: &str = r#"1 5"#;
const TEST_OUTPUT: &str = r#"6"#;

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
    let input: Vec<_> = input
        .into_iter()
        .fold(String::new(), fold_join_strings)
        .split(char::is_whitespace)
        .into_iter()
        .filter(|s| s.len() > 0)
        .map(|s| s.to_string())
        .map(|s| s.parse::<i32>().expect("number"))
        .collect();
    let mut output: Vec<_> = Vec::new();
    output.push(format!("{}", input[0] + input[1]));
    output
}

fn fold_join_strings(sum: String, s: String) -> String {
    sum + &s
}
fn fold_join_strings_eoln(sum: String, s: String) -> String {
    if sum.is_empty() {
        s
    }
    //else if s.is_empty() {sum}
    else {
        sum + "\n" + &s
    }
}

fn main() {
    let (input, _expected_output) = read_input();
    match _expected_output {
        None => run(input).into_iter().for_each(move |s| println!("{}", s)),
        Some(ref s) => {
            let output = run(input)
                .into_iter()
                .fold(String::new(), fold_join_strings_eoln);
            if output != *s {
                println!("got output:\n{:?}\nexpected output:\n{:?}", output, s);
            }
        }
    }
}
