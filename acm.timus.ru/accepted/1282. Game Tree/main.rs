use std::cmp::{max, min};
use std::io::{BufRead, BufReader, BufWriter, Read, Write};

struct State {
    tree_size: usize,
    parent: Vec<usize>,     //< предок текущего узла
    leaf: Vec<u8>,          //< если лист, то бит с результатом
    child: Vec<usize>,      //< первый потомок узла
    prev_child: Vec<usize>, //< предыдущий потомок общего предка
}

impl State {
    fn new(size: usize) -> Self {
        let size = size + 1;
        Self {
            tree_size: size,
            parent: vec![0; size],
            leaf: vec![0u8; size],
            child: vec![0; size],
            prev_child: vec![0; size],
        }
    }

    fn add_node(&mut self, node: usize, parent: usize) {
        self.parent[node] = parent;
        self.prev_child[node] = self.child[parent];
        self.child[parent] = node;
    }

    fn add_leaf(&mut self, node: usize, parent: usize, outcome_bits: u8) {
        self.add_node(node, parent);
        self.leaf[node] = outcome_bits;
    }

    fn minimax(&mut self, node: usize, first_player: bool) -> u8 {
        if self.leaf[node] != 0 {
            return self.leaf[node];
        }
        let mut value: u8;
        if first_player {
            value = 1;
            let mut child = self.child[node];
            while child != 0 {
                value = max(value, self.minimax(child, false));
                child = self.prev_child[child];
            }
        } else {
            value = 4;
            let mut child = self.child[node];
            while child != 0 {
                value = min(value, self.minimax(child, true));
                child = self.prev_child[child];
            }
        }
        value
    }

    fn solve(&mut self) -> u8 {
        self.minimax(1, true)
    }
}

// "1282. Game Tree"
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
        .flatten();
    let mut state = State::new(values_iter.next().unwrap().parse().unwrap());

    for node in 2..state.tree_size {
        let node_type = values_iter.next().unwrap();
        let node_parent: usize = values_iter.next().unwrap().parse().unwrap();
        match node_type.as_ref() {
            "L" => {
                let outcome = match values_iter.next().unwrap().as_ref() {
                    "-1" => 1,
                    "0" => 2,
                    "+1" => 4,
                    _ => panic!(),
                };
                state.add_leaf(node, node_parent, outcome);
            }
            "N" => {
                state.add_node(node, node_parent);
            }
            _ => panic!(),
        }
    }

    writeln!(
        writer,
        "{}",
        match state.solve() {
            1 => "-1",
            2 => "0",
            4 => "+1",
            _ => panic!(),
        }
    );
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn test() {
        use crate::solve;
        let mut buf_in = r#"7
N 1
N 1
L 2 -1
L 2 +1
L 3 +1
L 3 +1"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"+1"#.trim());
    }
    #[test]
    fn test2() {
        use crate::solve;
        let mut buf_in = r#"7
N 1
N 1
L 2 -1
L 2 +1
L 3 +1
L 3 0"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"0"#.trim());
    }
    #[test]
    fn test3() {
        use crate::solve;
        let mut buf_in = r#"18
N 1
N 1
N 2
L 2 +1
N 3
L 3 +1
L 3 +1
L 4 -1
L 4 +1
N 4
N 6
L 6 -1
L 6 -1
L 11 -1
L 11 +1
L 12 +1
L 12 -1"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"+1"#.trim());
    }
}
