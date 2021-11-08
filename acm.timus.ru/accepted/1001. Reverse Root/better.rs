use std::io::prelude::*;
use std::io::{BufReader, BufWriter, Read, Write};

fn solve(input: &mut dyn Read, output: &mut dyn Write) {
    let reader = BufReader::new(input);
    let mut data: Vec<f64> = Vec::new();
    for line in reader.lines().map(|s| s.expect("input")) {
        for v in line
            .split(char::is_whitespace)
            .map(|s| s.trim())
            .filter(|s| s.len() > 0)
            .map(|s| s.parse::<i64>().expect("integer value") as f64)
        {
            data.push(v)
        }
    }
    let mut writer = BufWriter::new(output);
    for val in data.into_iter().rev() {
        writer.write_fmt(format_args!("{:0.4}\n", val.sqrt()));
    }
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn basic_test() {
        use crate::solve;
        let mut buf_in = TEST_INPUT.as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output, TEST_OUTPUT);
    }

    const TEST_INPUT: &str = r#" 1427  0   

    876652098643267843 
 5276538
   
    "#;
    const TEST_OUTPUT: &str = r#"2297.0716
936297014.1164
0.0000
37.7757
"#;
}
