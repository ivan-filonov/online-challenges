use std::cmp::Ordering;
use std::ops::Shl;

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

mod tests {
    #[test]
    fn simple() {
        use crate::bigint::*;
        let big3 = BigInt::from("3");
        let big5 = BigInt::from("5");
        assert_eq!(
            big3.pow_u32(77),
            BigInt::from("5474401089420219382077155933569751763")
        );
        assert_eq!(
            (big3.pow_u32(121) + big5.pow_u32(61)) * 11,
            BigInt::from("59301339897176277648530523736012062639061933986184389245608")
        );
    }
}
