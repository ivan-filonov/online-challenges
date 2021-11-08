use std::io::prelude::*;
use std::io::{BufReader, BufWriter, Read, Write};
use std::ops::Shl;

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
    let mut iter = reader
        .lines()
        .map(|s| s.unwrap())
        .map(|s| {
            s.split(char::is_whitespace)
                .map(|a| a.to_string())
                .collect::<Vec<String>>()
        })
        .flatten()
        .map(|s| s.parse::<i32>().unwrap());
    let num_weights = iter.next().unwrap();
    let weights: Vec<_> = iter.collect();
    let mut best = weights.iter().fold(0, |a, b| a + b);
    for i in 0..1_i32.shl(num_weights) {
        let mut s = 0;
        for (n, w) in weights.iter().enumerate() {
            let bit = 1_i32.shl(n);
            s = if (i & bit) != 0 { s + w } else { s - w };
        }
        s = s.abs();
        if s < best {
            best = s;
        }
        if 0 == best {
            break;
        }
    }
    writeln!(writer, "{}", best);
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn basic_test() {
        use crate::solve;
        let mut buf_in = r#"5
5 8 13 27 14
"#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(
            output,
            r#"3
"#
        );
    }
}
