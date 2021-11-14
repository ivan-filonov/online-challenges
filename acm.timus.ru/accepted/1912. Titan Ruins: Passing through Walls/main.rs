//use std::io::prelude::*;

use std::io::{BufRead, BufReader, BufWriter, Read, Write};
use std::ops::{Shl, Shr};

use std::collections::{HashMap, VecDeque};

struct State {
    num_rows: usize,
    num_cols: usize,
    edges: [[[bool; 4]; 256]; 256],
    costs: [[[u8; 4]; 256]; 256],
    start_r: [i32; 2],
    start_c: [i32; 2],
    answer: u32,
}

const DIR_UP: usize = 0;
const DIR_LEFT: usize = 1;
const DIR_RIGHT: usize = 2;
const DIR_DOWN: usize = 3;

impl State {
    fn new(num_rows: usize, num_cols: usize) -> State {
        State {
            num_rows: num_rows,
            num_cols: num_cols,
            edges: [[[false; 4]; 256]; 256],
            costs: [[[255u8; 4]; 256]; 256],
            start_r: [0; 2],
            start_c: [0; 2],
            answer: u32::MAX,
        }
    }

    fn print_answer(&self, writer: &mut dyn Write) {
        let mut writer = BufWriter::new(writer);
        if self.answer < u32::MAX {
            writeln!(writer, "{}", self.answer);
        } else {
            writeln!(writer, "Death");
        }
    }

    fn run(&mut self) {
        let mut in_queue: HashMap<u32, bool> = HashMap::new();
        let mut dist: HashMap<u32, u32> = HashMap::new();
        let mut queue: VecDeque<u32> = VecDeque::new();
        let mut answer = u32::MAX;

        let start = pack4(
            self.start_r[0],
            self.start_c[0],
            self.start_r[1],
            self.start_c[1],
        );
        dist.insert(start, 0);
        queue.push_back(start);

        while !queue.is_empty() {
            let cur_packed = queue.pop_front().unwrap();
            in_queue.insert(start, false);
            let (cur_r0, cur_c0, cur_r1, cur_c1) = unpack4(cur_packed);
            let cur_dist: u32 = *dist.get(&cur_packed).unwrap();
            let mut neighbours: Vec<(i32, i32, i32, i32, u8)> = Vec::new();
            for dir in 0..4 {
                let (new_r0, new_c0) = apply_direction(cur_r0, cur_c0, dir);
                let (new_r1, new_c1) = apply_direction(cur_r1, cur_c1, dir);
                if self.edges[cur_r0 as usize][cur_c0 as usize][dir]
                    && is_state_valid(new_r0, new_c0, cur_r1, cur_c1)
                {
                    neighbours.push((
                        new_r0,
                        new_c0,
                        cur_r1,
                        cur_c1,
                        self.costs[cur_r0 as usize][cur_c0 as usize][dir],
                    ));
                }
                if self.edges[cur_r1 as usize][cur_c1 as usize][dir]
                    && is_state_valid(cur_r0, cur_c0, new_r1, new_c1)
                {
                    neighbours.push((
                        cur_r0,
                        cur_c0,
                        new_r1,
                        new_c1,
                        self.costs[cur_r1 as usize][cur_c1 as usize][dir],
                    ));
                }
            }
            for (r0, c0, r1, c1, edge_len) in neighbours {
                let new_packed = pack4(r0, c0, r1, c1);
                let new_dist: u32 = *dist.get(&new_packed).unwrap_or(&u32::MAX);
                if new_dist > cur_dist + edge_len as u32 {
                    let newer_dist = cur_dist + edge_len as u32;
                    if newer_dist > answer {
                        continue;
                    }
                    dist.insert(new_packed, newer_dist);
                    if self.half_win(r0, c0) && self.half_win(r1, c1) && newer_dist < answer {
                        answer = newer_dist;
                    }
                    match in_queue.get(&new_packed) {
                        Some(true) => {}
                        Some(false) => queue.push_front(new_packed),
                        None => queue.push_back(new_packed),
                    }
                    in_queue.insert(new_packed, true);
                }
            }
        }
        self.answer = answer;
    }

    fn half_win(&self, r: i32, c: i32) -> bool {
        (r == 0) || (c == 0) || (r == self.num_rows as i32 + 1) || (c == self.num_cols as i32 + 1)
    }
}

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let mut reader = BufReader::new(input);
    //let mut writer = BufWriter::new(writer);
    let mut buf = String::new();
    reader.read_line(&mut buf).expect("header");
    let config: Vec<_> = buf
        .split(char::is_whitespace)
        .filter_map(|s| s.parse::<usize>().ok())
        .collect();
    let mut state = State::new(config[0], config[1]);
    state.init();
    for (i, line) in reader
        .lines()
        .map(|r| r.unwrap())
        .map(|s| s.trim().to_string())
        .enumerate()
    {
        state.add_line(i, &line);
    }
    state.run();
    state.print_answer(writer);
}

fn pack4(r: i32, c: i32, r1: i32, c1: i32) -> u32 {
    let r = r as u32;
    let c = c as u32;
    let r1 = r1 as u32;
    let c1 = c1 as u32;
    (r & 255) | (c & 255).shl(8) | (r1 & 255).shl(16) | (c1 & 255).shl(24)
}

fn unpack4(p: u32) -> (i32, i32, i32, i32) {
    let r = p.shr(0) & 255u32;
    let c = p.shr(8) & 255u32;
    let r1 = p.shr(16) & 255u32;
    let c1 = p.shr(24) & 255u32;
    (r as i32, c as i32, r1 as i32, c1 as i32)
}

fn is_state_valid(r0: i32, c0: i32, r1: i32, c1: i32) -> bool {
    (r0 != r1 || c0 != c1) && (r0 - r1).abs() <= 1 && (c0 - c1).abs() <= 1
}

fn direction_index(dr: i32, dc: i32) -> usize {
    match (dr, dc) {
        (-1, 0) => DIR_UP,
        (1, 0) => DIR_DOWN,
        (0, -1) => DIR_LEFT,
        (0, 1) => DIR_RIGHT,
        _ => panic!(),
    }
}

fn apply_direction(r: i32, c: i32, dir: usize) -> (i32, i32) {
    match dir {
        DIR_UP => (r - 1, c),
        DIR_DOWN => (r + 1, c),
        DIR_LEFT => (r, c - 1),
        DIR_RIGHT => (r, c + 1),
        _ => panic!(),
    }
}

impl State {
    fn init(&mut self) {
        self.init_borders();
    }

    fn set_start(&mut self, id: char, r: i32, c: i32) {
        match id {
            '1' => {
                self.start_r[0] = r;
                self.start_c[0] = c;
            }
            '2' => {
                self.start_r[1] = r;
                self.start_c[1] = c;
            }
            _ => {
                panic!()
            }
        }
    }

    fn add_edge_directed(&mut self, r: i32, c: i32, dir: usize, cost: u8) {
        self.edges[r as usize][c as usize][dir] = true;
        self.costs[r as usize][c as usize][dir] = cost;
    }
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn test() {
        use crate::solve;
        let mut buf_in = r#"3 4
+.+-+-+-+
| . .1. |
+-+-+-+.+
| .2. . |
+.+.+.+-+
| . | | |
+.+.+-+-+"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"13"#.trim());
    }
    #[test]
    fn test2() {
        use crate::solve;
        let mut buf_in = r#"2 2
+-+-+
.1|2.
+-+-+
| | |
+-+-+"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"Death"#.trim());
    }

    #[test]
    fn test3() {
        use crate::solve;
        let mut buf_in = r#"3 3
+.+-+.+
|1|2| |
+-+.+.+
| | . |
+-+.+.+
| | | |
+-+.+.+"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"8"#.trim());
    }
}

impl State {
    fn add_edge(&mut self, r: i32, c: i32, r1: i32, c1: i32, cost: u8) {
        assert!(r == r1 || c == c1);
        assert!(r != r1 || c != c1);
        assert!((r - r1).abs() <= 1);
        assert!((c - c1).abs() <= 1);
        let dir = direction_index(r1 - r, c1 - c);
        self.add_edge_directed(r, c, dir, cost);
        self.add_edge_directed(r1, c1, 3 - dir, cost);
    }

    fn init_borders(&mut self) {
        let bottom = self.num_rows as i32 + 1;
        for c in 0..self.num_cols as i32 + 1 {
            self.add_edge(0, c, 0, c + 1, 0);
            self.add_edge(bottom, c, bottom, c + 1, 0);
        }
        let right = self.num_cols as i32 + 1;
        for r in 0..self.num_rows as i32 + 1 {
            self.add_edge(r, 0, r + 1, 0, 0);
            self.add_edge(r, right, r + 1, right, 0);
        }
    }

    fn add_walls(&mut self, i: usize, line: &String) {
        let r = i as i32 / 2;
        let r1 = r + 1;
        for (col, ch) in line.char_indices().skip(1).step_by(2) {
            let c = col as i32 / 2 + 1;
            if '.' == ch {
                self.add_edge(r, c, r1, c, 1);
            }
        }
    }

    fn add_rooms(&mut self, i: usize, line: &String) {
        let r = i as i32 / 2 + 1;
        for (j, ch) in line.char_indices() {
            let c = j as i32 / 2;
            let c1 = c + 1;
            match ch {
                '.' => self.add_edge(r, c, r, c1, 1),
                '1' => self.set_start(ch, r, c1),
                '2' => self.set_start(ch, r, c1),
                _ => {}
            }
        }
    }

    fn add_line(&mut self, i: usize, line: &String) {
        if i % 2 == 0 {
            self.add_walls(i, line);
        } else {
            self.add_rooms(i, line);
        }
    }
}
