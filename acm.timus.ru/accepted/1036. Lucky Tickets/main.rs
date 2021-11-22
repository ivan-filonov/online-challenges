//use std::io::prelude::*;

use std::collections::{HashMap, VecDeque};
use std::io::{BufRead, BufReader, BufWriter, Read, Write};
use std::ops::{Shl, Shr};

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

    fn sqr(&self) -> Self {
        let mut res : Value = Value::new(0);
        for (shift, digit) in self.data.iter().enumerate() {
            let mut temp = self.clone();
            for _ in 0..shift {
                temp.data.insert(0, 0);
            }
            res.add(temp.mul_u32(*digit));
        }
        res
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
    let mut reader = BufReader::new(input);
    //let mut writer = BufWriter::new(writer);
    let mut buf = String::new();
    reader.read_line(&mut buf).expect("need header");
    let args: Vec<_> = buf
        .trim()
        .split(char::is_whitespace)
        .map(|s| s.parse::<usize>().expect("need integer value"))
        .collect();
    let size = args[0];
    if args[1] % 2 == 1 {
        writeln!(writer, "0");
        return;
    }
    let sum = args[1] / 2;
    let mut d: Vec<Value> = Vec::with_capacity(size * (sum + 1));
    for _ in 0..size * (sum + 1) {
        d.push(Value::new(0));
    }
    for k in 0..sum + 1 {
        if k < 10 {
            d[k] = Value::new(1);
        }
    }
    for n in 1..size {
        for k in 0..sum + 1 {
            for v in 0..10 {
                if v <= k {
                    let b = d[(n - 1) * (sum + 1) + k - v].clone();
                    d[n * (sum + 1) + k].add(b);
                }
            }
        }
    }
    writeln!(writer, "{}", d.last().unwrap().sqr().to_string());
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn test() {
        use crate::solve;
        let mut buf_in = r#"2 2"#.as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"4"#.trim());
    }

    #[test]
    fn test2() {
        use crate::solve;
        let mut buf_in = r#"2 4"#.as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"9"#.trim());
    }

    #[test]
    fn test3() {
        use crate::solve;
        let mut buf_in = r#"4 8"#.as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"1225"#.trim());
    }

    #[test]
    fn test4() {
        use crate::solve;
        let mut buf_in = r#"50 500"#.as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"854559745684320697549060368131279814466643179689928095831053239604130293492672614469791533133321"#.trim());
    }

    #[test]
    fn test5() {
        use crate::solve;
        let mut buf_in = r#"20 100"#.as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"475696100826892995150152084506564"#.trim());
    }

    #[test]
    fn test6() {
        use crate::solve;
        let mut buf_in = r#"50 300"#.as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"3660144008320861404705392514284143308670326732628084621145897204106027006076468826991924409"#.trim());
    }
}
