use std::io::{BufRead, BufReader, BufWriter, Read, Write};

struct Fenwick {
    values: Vec<u32>,
}

const MOD: u32 = 1_000_000_000;
fn add_mod(x: u32, y: u32) -> u32 {
    (x + y) % MOD
}
fn sub_mod(x: u32, y: u32) -> u32 {
    if x > y {
        (x - y) % MOD
    } else {
        (x + MOD - y) % MOD
    }
}

impl Fenwick {
    fn new(size: usize) -> Self {
        Self {
            values: vec![0; size + 1],
        }
    }

    fn inc(&mut self, i: u32, delta: u32) {
        let n = self.values.len();
        let t = &mut self.values;
        let mut i = i as usize;
        while i < n {
            t[i] = add_mod(t[i], delta);
            i = i | (i + 1);
        }
    }

    fn sum(&self, r: u32) -> u32 {
        let mut result: u32 = 0;
        let t = &self.values;
        let n = self.values.len();
        let mut r = r as i64;
        while r >= 0 {
            result = add_mod(result, t[r as usize]);
            r = (r & (r + 1)) - 1;
        }
        result
    }
    fn sum_range(&self, l: u32, r: u32) -> u32 {
        sub_mod(self.sum(r), self.sum(l))
    }
}

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
    let mut values_iter = reader
        .lines()
        .map(|s| s.unwrap())
        .map(|s| {
            s.split(char::is_whitespace)
                .map(|s2| s2.to_string())
                .collect::<Vec<String>>()
        })
        .flatten()
        .map(|s: String| s.parse::<i32>().unwrap());
    let num_values = values_iter.next().unwrap() as usize;
    let inv_size = values_iter.next().unwrap() as usize;
    let values: Vec<_> = values_iter.collect();

    let mut cc = vec![vec![1u32; num_values], vec![0u32; num_values]];
    let mut ci = 1;
    for _ in 1..inv_size {
        let mut t = Fenwick::new(num_values);
        for (idx, &v) in values.iter().enumerate().rev() {
            let v = v as u32;
            cc[ci][idx] = t.sum_range(0, v);
            t.inc(v, cc[1 - ci][idx]);
        }
        ci = 1 - ci;
    }

    writeln!(
        writer,
        "{}",
        cc[1 - ci].iter().fold(0u32, |x, &y| add_mod(x, y))
    );
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn test1() {
        use crate::solve;
        let mut buf_in = r#"3 2
3 1 2"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"2"#.trim());
    }
    #[test]
    fn test2() {
        use crate::solve;
        let mut buf_in = r#"5 3
5 4 3 2 1"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"10"#.trim());
    }
    #[test]
    fn test3() {
        use crate::solve;
        let mut buf_in = r#"40 10
40 39 38 37 36 35 34 33 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"847660528"#.trim());
    }
    #[test]
    fn test4() {
        use crate::solve;
        let mut buf_in = r#"6 3
6 2 3 1 5 4"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"3"#.trim());
    }
}
