//use std::io::prelude::*;
use std::io::{BufRead, BufReader, BufWriter, Read, Write};
use std::ops::Shl;

fn parse_percent(s: String) -> i32 {
    let v: Vec<_> = s.split('.').collect();
    let integer: i32 = v[0].parse().unwrap();
    let fractional: i32 = if v.len() == 1 {
        0
    } else {
        let fs = if v[1].len() > 1 {
            v[1].to_string()
        } else {
            v[1].to_string() + "0"
        };
        fs.parse::<i32>().unwrap()
    };
    integer * 100 + fractional
}

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
    let v: Vec<String> = reader
        .lines()
        .map(|s| s.unwrap())
        .map(|s| {
            s.split(char::is_whitespace)
                .map(|s| s.to_string())
                .collect::<Vec<String>>()
        })
        .flatten()
        .collect();
    let p = parse_percent(v[0].clone());
    let q = parse_percent(v[1].clone());
    assert_eq!(p < q, true);
    let mut result = 0;
    for x in 1..20000 {
        let min_conductors = x * p;
        let max_conductors = x * q;
        let min_int_conductors = min_conductors
            + (if min_conductors % 10000 > 0 {
                10000 - min_conductors % 10000
            } else {
                0
            });
        let max_int_conductors = max_conductors - max_conductors % 10000;
        if min_int_conductors > max_int_conductors {
            continue;
        }
        result = x;
        if min_conductors < min_int_conductors && min_int_conductors < max_conductors {
            break;
        }
        if max_int_conductors - min_int_conductors > 10000 {
            break;
        }
    }
    writeln!(writer, "{}", result);
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn test_pars() {
        use crate::parse_percent;
        assert_eq!(parse_percent("0.01".to_string()), 1);
        assert_eq!(parse_percent("0.1".to_string()), 10);
        assert_eq!(parse_percent("1".to_string()), 100);
        assert_eq!(parse_percent("10".to_string()), 1000);
        assert_eq!(parse_percent("13".to_string()), 1300);
        assert_eq!(parse_percent("14.1".to_string()), 1410);
        assert_eq!(parse_percent("14.01".to_string()), 1401);
    }

    #[test]
    fn test1() {
        use crate::solve;
        let mut buf_in = r#"13
14.1"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"15"#.trim());
    }

    #[test]
    fn test2() {
        use crate::solve;
        let mut buf_in = r#"13 14.1"#.as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"15"#.trim());
    }
}
