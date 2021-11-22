//use std::io::prelude::*;

use std::collections::{HashMap, VecDeque};
use std::io::{BufRead, BufReader, BufWriter, Read, Write};
use std::ops::{Shl, Shr};

use std::cmp::Ordering;
#[derive(Debug)]
struct BigInt {
    sign: i32,
    value: Vec<u32>,
}

impl BigInt {
    const BASE: u64 = 1_000_000_000_u64;

    fn new(init: i64) -> Self {
        let mut value = Vec::new();
        let sign = init.signum();
        let mut c = init.abs() as u64;
        while c > 0 {
            value.push((c % Self::BASE) as u32);
            c /= Self::BASE;
        }
        if value.is_empty() {
            value.push(0);
        }
        Self {
            sign: sign as i32,
            value: value,
        }
    }

    fn clone(&self) -> Self {
        Self {
            sign: self.sign,
            value: self.value.clone(),
        }
    }

    fn to_string(&self) -> String {
        let mut parts: Vec<String> = self.value.iter().map(|x| x.to_string()).collect();
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
        if self.sign >= 0 {
            s.to_string()
        } else {
            "-".to_string() + &s.to_string()
        }
    }

    fn add(&self, other: &Self) -> Self {
        assert_eq!(self.sign, other.sign);
        let mut value: Vec<u32> = Vec::new();
        let mut carry: u64 = 0;
        let maxi = usize::max(self.value.len(), other.value.len());
        for i in 0..maxi {
            let mut sum = carry;
            if i < self.value.len() {
                sum += self.value[i] as u64;
            }
            if i < other.value.len() {
                sum += other.value[i] as u64;
            }
            value.push((sum % Self::BASE) as u32);
            carry = sum / Self::BASE;
        }
        if carry > 0 {
            value.push(carry as u32);
        }
        while value.len() > 1 && *value.last().unwrap() == 0u32 {
            value.pop();
        }
        Self {
            sign: self.sign,
            value: value,
        }
    }

    fn neg(&self) -> Self {
        BigInt {
            sign: -self.sign,
            value: self.value.clone(),
        }
    }

    fn mul_i32(&self, k: i32) -> Self {
        let result = self.mul_u32(k.abs() as u32);
        if k > 0 {
            result
        } else {
            result.neg()
        }
    }

    fn mul_u32(&self, k: u32) -> Self {
        if k == 0 {
            return Self::new(0);
        }
        if k == 1 {
            return self.clone();
        }
        let k = k as u64;
        let mut value: Vec<u32> = Vec::new();
        let mut carry = 0u64;
        for &x in self.value.iter() {
            let t = (x as u64) * k + carry;
            value.push((t % Self::BASE) as u32);
            carry = t / Self::BASE;
        }
        if carry > 0 {
            value.push(carry as u32);
        }
        while value.len() > 1 && *value.last().unwrap() == 0u32 {
            value.pop();
        }
        Self {
            sign: self.sign,
            value: value,
        }
    }

    fn is_zero(&self) -> bool {
        self.value.len() == 1 && self.value[0] == 0
    }

    fn is_one(&self) -> bool {
        self.value.len() == 1 && self.value[0] == 1
    }

    fn mul(&self, other: &Self) -> Self {
        if self.is_zero() || other.is_zero() {
            return Self::new(0);
        }
        if self.is_one() {
            return other.clone();
        }
        if other.is_one() {
            return self.clone();
        }
        let mut value: Vec<u32> = Vec::new();
        for _ in 0..=self.value.len() + other.value.len() {
            value.push(0);
        }
        for (sx, &x) in self.value.iter().enumerate() {
            for (sy, &y) in other.value.iter().enumerate() {
                let mut ofs = sx + sy;
                let mut carry = (x as u64) * (y as u64);
                while carry > 0 {
                    let t = value[ofs] as u64 + carry;
                    value[ofs] = (t % Self::BASE) as u32;
                    carry = t / Self::BASE;
                    ofs += 1;
                }
            }
        }
        while value.len() > 1 && *value.last().unwrap() == 0u32 {
            value.pop();
        }
        Self {
            sign: self.sign * other.sign,
            value: value,
        }
    }

    fn pow_u32(&self, p: u32) -> Self {
        let mut result = Self::new(1);
        let mut p2 = self.clone();
        for shift in 0..32 {
            let bit = 1u32.shl(shift);
            if bit > p {
                break;
            }
            if shift > 0 {
                p2 = p2.mul(&p2);
            }
            if (bit & p) > 0u32 {
                result = result.mul(&p2);
            }
        }
        result
    }

    fn cmp(&self, other: &Self) -> Ordering {
        if self.sign < other.sign {
            return Ordering::Less;
        } else if self.sign > other.sign {
            return Ordering::Greater;
        }
        let c1 = self.value.len().cmp(&other.value.len());
        if c1 == Ordering::Equal {
            for i in 0..self.value.len() {
                let j = self.value.len() - i - 1;
                let l = self.value[j];
                let r = other.value[j];
                if l > r {
                    return Ordering::Greater;
                } else if l < r {
                    return Ordering::Less;
                }
            }
        }

        if self.sign > 0 && other.sign > 0 {
            c1
        } else {
            match c1 {
                Ordering::Equal => c1,
                Ordering::Less => Ordering::Greater,
                Ordering::Greater => Ordering::Less,
            }
        }
    }
}

impl std::ops::Add for BigInt {
    type Output = BigInt;
    fn add(self, rhs: Self) -> Self {
        BigInt::add(&self, &rhs)
    }
}

impl std::ops::Mul<BigInt> for BigInt {
    type Output = BigInt;
    fn mul(self, other: BigInt) -> BigInt {
        BigInt::mul(&self, &other)
    }
}

impl std::ops::Mul<u32> for BigInt {
    type Output = BigInt;
    fn mul(self, other: u32) -> BigInt {
        BigInt::mul_u32(&self, other)
    }
}

impl std::ops::Mul<i32> for BigInt {
    type Output = BigInt;
    fn mul(self, other: i32) -> BigInt {
        BigInt::mul_i32(&self, other)
    }
}

impl From<BigInt> for String {
    fn from(value: BigInt) -> String {
        value.to_string()
    }
}

impl From<String> for BigInt {
    fn from(value: String) -> BigInt {
        if value.as_bytes()[0] == '-' as u8 {
            return BigInt {
                sign: -1,
                value: BigInt::from(value[1..].to_string()).value,
            };
        }
        let mut v: Vec<u32> = Vec::new();
        let count = value.len() / 9 + 1;
        v.resize(count, 0);
        for i in 0..count {
            let end = value.len() - i * 9;
            let start = if end >= 9 { end - 9 } else { 0 };
            v[i] = value[start..end].parse().unwrap();
        }
        BigInt { sign: 1, value: v }
    }
}

impl From<&str> for BigInt {
    fn from(value: &str) -> BigInt {
        BigInt::from(String::from(value))
    }
}

impl PartialEq for BigInt {
    fn eq(&self, other: &BigInt) -> bool {
        self.value == other.value
    }
}

impl PartialOrd for BigInt {
    fn partial_cmp(&self, other: &BigInt) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Eq for BigInt {}

impl Ord for BigInt {
    fn cmp(&self, other: &Self) -> Ordering {
        self.partial_cmp(other).unwrap()
    }
}

fn mul_slice(part: &[i32]) -> BigInt {
    let mut p = vec![0u32; 4];
    let mut sign = 1;
    for &i in part {
        p[i.abs() as usize] += 1;
        if i < 0 {
            sign = -sign;
        }
    }
    BigInt::new(2).pow_u32(p[2]) * BigInt::new(3).pow_u32(p[3]) * BigInt::new(sign)
}

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
    let values: Vec<i32> = reader
        .lines()
        .map(|s| s.unwrap())
        .map(|s| {
            s.split(char::is_whitespace)
                .map(|s2| s2.to_string())
                .collect::<Vec<String>>()
        })
        .flatten()
        .map(|s: String| s.parse::<i32>().unwrap())
        .skip(1)
        .collect();
    let mut answers: Vec<BigInt> = Vec::new();
    answers.push(BigInt::new(*values.iter().max().unwrap() as i64));

    for part in values.split(|&x| x == 0).filter(|s| s.len() > 0) {
        let num_neg = part.iter().filter(|x| **x < 0).count();
        if num_neg % 2 == 0 {
            answers.push(mul_slice(part));
        } else if part.len() > 1 {
            let mut first_neg: usize = 0;
            let mut last_neg: usize = 0;
            let mut have_first = false;
            for (pos, &val) in part.iter().enumerate() {
                if val >= 0 {
                    continue;
                }
                last_neg = pos;
                if !have_first {
                    first_neg = pos;
                    have_first = true;
                }
            }
            answers.push(mul_slice(&part[first_neg + 1..]));
            answers.push(mul_slice(&part[..last_neg]));
        }
    }

    writeln!(
        writer,
        "{}",
        String::from(answers.into_iter().max().unwrap())
    );
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn test_bigint() {
        use crate::*;
        assert_eq!(
            String::from(BigInt::new(2).pow_u32(83)),
            "9671406556917033397649408"
        );
        assert_eq!(
            String::from(BigInt::new(3).pow_u32(55)),
            "174449211009120179071170507"
        );
        assert_eq!(
            BigInt::new(3).pow_u32(35) * BigInt::new(11).pow_u32(17),
            BigInt::from("25288295801517806497456928035925097")
        );
        assert_eq!(
            String::from(BigInt::new(3).pow_u32(35) * BigInt::new(11).pow_u32(17)),
            "25288295801517806497456928035925097"
        );
        assert_eq!(
            String::from(BigInt::new(3).pow_u32(35) * BigInt::new(11).pow_u32(17) * 31u32),
            "783937169847052001421164769113678007"
        );
        assert_eq!(BigInt::from("123"), BigInt::from("123"));
        assert_eq!(
            BigInt::from("25288295801517806497456928035925097"),
            BigInt::from("25288295801517806497456928035925097")
        );
        assert_eq!(BigInt::from("123") < BigInt::from("124"), true);
        assert_eq!(
            BigInt::from("25288295801517806497456928035925097")
                < BigInt::from("25288295802517806497456928035925097"),
            true
        );
        assert_eq!(BigInt::from("133") > BigInt::from("123"), true);
        assert_eq!(
            BigInt::from("25288395801517806497456928035925097")
                > BigInt::from("25288295801517806497456928035925097"),
            true
        );
        assert_eq!(BigInt::from("133") > BigInt::from("-123"), true);
        assert_eq!(BigInt::from("-133") < BigInt::from("-123"), false);
    }

    #[test]
    fn test_bigint_pow() {
        use crate::*;
        let pow = 500;
        let base = 3;
        let as_pow = BigInt::new(base).pow_u32(pow);
        let mut as_mul = BigInt::new(1);
        for _ in 0..pow {
            as_mul = as_mul * BigInt::new(base);
        }
        assert_eq!(as_pow.to_string(), as_mul.to_string());
        assert_eq!(as_pow, as_mul);
    }

    #[test]
    fn test() {
        use crate::solve;
        let mut buf_in = r#"3
1 2 -1"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"2"#.trim());
    }
    #[test]
    fn test_single_neg() {
        use crate::solve;
        let mut buf_in = r#"1
-1"#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"-1"#.trim());
    }
    #[test]
    fn test_zero() {
        use crate::solve;
        let mut buf_in = r#"1
0"#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"0"#.trim());
    }
    #[test]
    fn test_2_neg() {
        use crate::solve;
        let mut buf_in = r#"2
-2 -1"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"2"#.trim());
    }
    #[test]
    fn test_odd_neg() {
        use crate::solve;
        let mut buf_in = r#"5
2 -1 2 -1 1 -1 3"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"6"#.trim());
    }
    #[test]
    fn test_n_neg() {
        use crate::solve;
        let mut buf_in = r#"5
-1 -2 -2 -2 -1"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"8"#.trim());
    }
    #[test]
    fn test_1_neg() {
        use crate::solve;
        let mut buf_in = r#"5
1 2 2 -2 1"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"4"#.trim());
    }
    #[test]
    fn test_even_neg() {
        use crate::solve;
        let mut buf_in = r#"5
2 -1 3 3 -1 3"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"54"#.trim());
    }
    #[test]
    fn test_zeros() {
        use crate::solve;
        let mut buf_in = r#"15
0 2 -1 3 3 -1 3 0 0 -1 -2 -2 -2 -1 0"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"54"#.trim());
    }
    #[test]
    fn test_f1() {
        use crate::solve;
        let mut buf_in = r#"15
1 -1 0 2 -2 0 3 -1 0 0 -3 -3 2 0 1"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"18"#.trim());
    }
    #[test]
    fn test_f2() {
        use crate::solve;
        let mut buf_in = r#"11
-1 0 -1 0 -3 0 -1 0 0 0 -2"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"0"#.trim());
    }
}
