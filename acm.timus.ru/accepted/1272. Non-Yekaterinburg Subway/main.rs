use std::collections::{HashMap, HashSet};
use std::io::{BufRead, BufReader, BufWriter, Read, Write};

struct Solution1272 {
    // "1272. Non-Yekaterinburg Subway"
    num_islands: usize,
    num_tonnels: usize,
    num_bridges: usize,
    tonnels: HashMap<usize, HashSet<usize>>,
}

impl Solution1272 {
    fn new() -> Self {
        Self {
            num_islands: 0,
            num_tonnels: 0,
            num_bridges: 0,
            tonnels: HashMap::new(),
        }
    }
    fn setup(&mut self, num_islands: usize, num_tonnels: usize, num_bridges: usize) {
        self.num_islands = num_islands;
        self.num_tonnels = num_tonnels;
        self.num_bridges = num_bridges;
        for v in 0..num_islands {
            self.tonnels.insert(v, HashSet::new());
        }
    }
    fn add_tonnel(&mut self, a: usize, b: usize) {
        if let Some(set) = self.tonnels.get_mut(&a) {
            set.insert(b);
        }
        if let Some(set) = self.tonnels.get_mut(&b) {
            set.insert(a);
        }
    }
    fn add_bridge(&mut self, _a: usize, _b: usize) {}
    fn solve(&self) -> usize {
        let mut stack: Vec<usize> = Vec::new();
        let mut visited = vec![false; self.num_islands];
        let mut components: usize = 0;
        for start in 0..self.num_islands {
            if visited[start] {
                continue;
            }
            components += 1;
            stack.push(start);
            while let Some(v) = stack.pop() {
                visited[v] = true;
                for &other in self.tonnels.get(&v).unwrap() {
                    if !visited[other] {
                        stack.push(other);
                    }
                }
            }
        }

        components - 1
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
    let mut s = Solution1272::new();
    let num_islands = values_iter.next().unwrap();
    let num_tonnels = values_iter.next().unwrap();
    let num_bridges = values_iter.next().unwrap();
    s.setup(
        num_islands as usize,
        num_tonnels as usize,
        num_bridges as usize,
    );
    for _ in 0..num_tonnels {
        let a = (values_iter.next().unwrap() - 1) as usize;
        let b = (values_iter.next().unwrap() - 1) as usize;
        s.add_tonnel(a, b);
    }
    for _ in 0..num_bridges {
        let a = (values_iter.next().unwrap() - 1) as usize;
        let b = (values_iter.next().unwrap() - 1) as usize;
        s.add_bridge(a, b);
    }

    writeln!(writer, "{}", s.solve());
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn test() {
        use crate::solve;
        let mut buf_in = r#"6 3 4
1 2
2 3
4 5
1 3
3 4
4 6
5 6"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"2"#.trim());
    }
    #[test]
    fn test2() {
        use crate::solve;
        let mut buf_in = r#"4 4 0
3 4
1 2
3 1
2 4"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"0"#.trim());
    }
    #[test]
    fn test3() {
        use crate::solve;
        let mut buf_in = r#"7 7 0
1 2
1 3
2 4
2 5
3 6
3 7
4 7
"#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"0"#.trim());
    }
}
