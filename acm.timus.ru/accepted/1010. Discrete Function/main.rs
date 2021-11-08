//use std::io::prelude::*;
use std::io::{BufRead, BufReader, BufWriter, Read, Write};
use std::ops::Shl;

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let mut reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
    let v: Vec<i64> = reader
        .lines()
        .map(|s| s.unwrap())
        .map(|s| s.parse().unwrap())
        .collect();
    let mut a = v[2];
    let mut d = (a - v[1]).abs();
    let mut ri: usize = 1;
    for (ai, &b) in v.iter().enumerate().skip(3) {
        let d2 = (a - b).abs();
        if d2 > d {
            ri = ai - 1;
            d = d2;
        }
        a = b;
    }
    writeln!(writer, "{} {}", ri, ri + 1);
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn test1() {
        use crate::solve;
        let mut buf_in = r#"3
2
6
4"#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"1 2"#.trim());
    }
    #[test]
    fn test2() {
        use crate::solve;
        let mut buf_in = r#"5
1
1
1
1
1"#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"1 2"#.trim());
    }

    #[test]
    fn test3() {
        use crate::solve;
        let mut buf_in = r#"3
1
-2000000000
2000000000"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"2 3"#.trim());
    }
}
