pub struct Fenwick {
    values: Vec<u32>,
}

const MOD: u32 = 1_000_000_000;
pub fn add_mod(x: u32, y: u32) -> u32 {
    (x + y) % MOD
}
pub fn sub_mod(x: u32, y: u32) -> u32 {
    if x > y {
        (x - y) % MOD
    } else {
        (x + MOD - y) % MOD
    }
}

impl Fenwick {
    pub fn new(size: usize) -> Self {
        Self {
            values: vec![0; size + 1],
        }
    }

    pub fn inc(&mut self, i: u32, delta: u32) {
        let n = self.values.len();
        let t = &mut self.values;
        let mut i = i as usize;
        while i < n {
            t[i] = add_mod(t[i], delta);
            i = i | (i + 1);
        }
    }

    pub fn sum(&self, r: u32) -> u32 {
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

    pub fn sum_range(&self, l: u32, r: u32) -> u32 {
        sub_mod(self.sum(r), self.sum(l))
    }
}
