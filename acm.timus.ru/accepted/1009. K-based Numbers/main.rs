//use std::io::prelude::*;
use std::io::{BufRead, BufReader, BufWriter, Read, Write};
use std::ops::Shl;

fn exp(x: u64, n: u64) -> u64 {
    match n {
        0 => 1,
        1 => x,
        i if i % 2 == 0 => exp(x * x, n / 2),
        _ => x * exp(x * x, (n - 1) / 2),
    }
}

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let mut reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
    let v: Vec<u64> = reader
        .lines()
        .map(|s| s.expect("ok").parse::<u64>().unwrap())
        .collect();
    assert_eq!(v.len(), 2);
    let length = v[0];
    let base = v[1];

    let mut result: u64 = 0;
    let max_i = 1u64.shl(length - 1);
    for i in 0..max_i {
        let mut num_set: u64 = 0;
        let mut prev: u64 = 1;
        let mut fail = false;
        let mut bit: u64 = 1;
        while bit < max_i && !fail {
            let cur = bit & i;
            if cur == 0 && prev == 0 {
                fail = true;
            }
            if cur != 0 {
                num_set += 1;
            }
            prev = cur;
            bit = bit.shl(1);
        }
        if !fail {
            result += exp(base - 1, num_set);
        }
    }
    result *= base - 1;

    writeln!(writer, "{}", result);
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn test1() {
        use crate::solve;
        let mut buf_in = r#"2
10"#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"90"#.trim());
    }
}
