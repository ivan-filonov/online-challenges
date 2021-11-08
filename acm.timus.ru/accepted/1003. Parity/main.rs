use std::collections::HashMap;
use std::io::prelude::*;
use std::io::{BufReader, BufWriter, Read, Write};

struct State {
    first_to_last: HashMap<i32, i32>,
    last_to_first: HashMap<i32, i32>,
    first_to_odd: HashMap<i32, bool>,
}

impl State {
    fn new() -> Self {
        State {
            first_to_last: HashMap::new(),
            last_to_first: HashMap::new(),
            first_to_odd: HashMap::new(),
        }
    }
    fn add(&mut self, first: i32, last: i32, odd: bool) -> bool {
        let have_first = self.first_to_last.contains_key(&first);

        // exact interval match
        if have_first && self.first_to_last[&first] == last {
            return self.first_to_odd[&first] == odd;
        }

        // no match at all
        if !have_first && !self.last_to_first.contains_key(&last) {
            self.first_to_last.insert(first, last);
            self.last_to_first.insert(last, first);
            self.first_to_odd.insert(first, odd);
            return true;
        }

        if have_first {
            // left match - both intervals share first point
            let old_last = self.first_to_last[&first];
            let old_odd = self.first_to_odd[&first] ^ odd;
            if old_last > last {
                // first .. last .. old_last
                // split
                self.first_to_last.remove(&first);
                self.last_to_first.remove(&old_last);
                self.first_to_odd.remove(&first);
                self.add(first, last, odd) && self.add(last + 1, old_last, old_odd)
            } else {
                // first .. old_last .. last
                // append
                self.add(old_last + 1, last, old_odd)
            }
        } else {
            // right match - both intervals share last point
            let old_first = self.last_to_first[&last];
            let old_odd = self.first_to_odd[&old_first] ^ odd;
            if old_first < first {
                // old_first .. first .. last
                // split
                self.add(old_first, first - 1, old_odd) && self.add(first, last, odd)
            } else {
                // first .. old_first .. last
                // prepend
                self.add(first, old_first - 1, old_odd)
            }
        }
    }
}

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
    let mut lines = reader.lines().map(|s| s.expect("correct input"));
    loop {
        let bit_count: i32 = lines
            .next()
            .expect("bit string length")
            .parse()
            .expect("corrent integer");
        if bit_count < 0 {
            break;
        }
        let mut state = State::new();
        let mut result: i32 = 0;
        let mut done = false;
        let num_questions: i32 = lines
            .next()
            .expect("number of questions")
            .parse()
            .expect("correct integer");
        for _ in 0..num_questions {
            let line_opt = &lines.next();
            if done {
                continue;
            }
            let parts: Vec<_> = line_opt.as_ref().unwrap().split(' ').collect();
            let first: i32 = parts[0].parse().unwrap();
            let last: i32 = parts[1].parse().unwrap();
            let odd = if parts[2] == "odd" {
                true
            } else if parts[2] == "even" {
                false
            } else {
                panic!()
            };
            if first < 0 || last > bit_count {
                done = true;
            } else if state.add(first, last, odd) {
                result += 1;
            } else {
                done = true;
            }
        }
        writeln!(writer, "{}", result);
    }
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn basic_test() {
        use crate::solve;
        let mut buf_in = r#"10
5
1 2 even
3 4 odd
5 6 even
1 6 even
7 10 odd
1000000000
10
1 100000000 even
100000001 200000000 even
200000001 300000000 even
300000001 400000000 even
400000001 500000000 even
1 500000000 even
1 1000000000 odd
100010001 200200000 odd
200000001 404000000 even
1 404000000 odd
-1
"#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(
            output,
            r#"3
9
"#
        );
    }
}
