//use std::io::prelude::*;
use std::io::{BufRead, BufReader, BufWriter, Read, Write};

const BASE: u64 = 1000_000_000u64;
struct Value {
    data: Vec<u32>,
}

impl Value {
    fn new(x: u64) -> Self {
        let mut data: Vec<u32> = Vec::new();
        data.push((x % BASE) as u32);
        if x > BASE {
            data.push((x / BASE) as u32);
        }
        Value { data: data }
    }

    fn to_string(&self) -> String {
        let mut parts: Vec<String> = self.data.iter().map(|x| x.to_string()).collect();
        parts.reverse();
        let mut s = String::with_capacity(9 * parts.len());
        for (n, p) in parts.into_iter().enumerate() {
            if n > 0 {
                for _ in p.len()..9 {
                    s.push('0');
                }
            }
            s.push_str(&p);
        }
        s.to_string()
    }

    fn clone(&self) -> Self {
        let data = self.data.clone();
        Value { data: data }
    }

    fn add(&mut self, other: Value) {
        let mut carry = 0u64;
        let mut v: Vec<u32> = Vec::new();
        let maxi = usize::max(self.data.len(), other.data.len());
        for i in 0..maxi {
            let mut sum = carry;
            if i < self.data.len() {
                sum += self.data[i] as u64;
            }
            if i < other.data.len() {
                sum += other.data[i] as u64;
            }
            v.push((sum % BASE) as u32);
            carry = sum / BASE;
        }
        if carry > 0 {
            v.push(carry as u32);
        }
        self.data = v;
    }

    fn mul_u32(self: Value, val: u32) -> Self {
        let mut s = self;
        let mut carry = 0u64;
        for x in s.data.iter_mut() {
            let t = *x as u64 * val as u64 + carry;
            *x = (t % BASE) as u32;
            carry = t / BASE;
        }
        if carry > 0 {
            s.data.push(carry as u32);
        }
        s
    }
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
        .filter(|s| s.len() > 0)
        .collect();
    assert_eq!(v.len(), 2);
    let length = v[0].parse::<u64>().expect("number");
    let base = v[1].parse::<u64>().expect("number");
    assert_eq!(base >= 2, true);
    assert_eq!(base <= 10, true);
    assert_eq!(length >= 2, true);
    assert_eq!(length + base <= 1800, true);

    let mut prev2 = Value::new(base - 1);
    let mut prev = Value::new(base * (base - 1));
    for _ in 2..length {
        let mut s = prev.clone();
        s.add(prev2);
        prev2 = prev;
        prev = s.mul_u32(base as u32 - 1);
    }

    writeln!(writer, "{}", prev.to_string());
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn bigint() {
        use crate::*;
        use std::str::FromStr;
        let mut v = Value::new(1);
        let mut s = String::from_str("1").unwrap();
        assert_eq!(Value::new(0).to_string(), "0");
        for _ in 0..30 {
            assert_eq!(v.to_string(), s);
            s.push('1');
            v = v.mul_u32(10);
            v.add(Value::new(1));
        }
    }

    #[test]
    fn test() {
        use crate::solve;
        let mut buf_in = r#"2
10"#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"90"#.trim());
    }

    #[test]
    fn test2() {
        use crate::solve;
        let mut buf_in = r#"170 10"#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"19140929114881653462476041684116627391559236845580909494492016732196087599513580596508681339397425705393057484060909466165965897483835868175679753976672747715432612675930"#.trim());
    }
}
