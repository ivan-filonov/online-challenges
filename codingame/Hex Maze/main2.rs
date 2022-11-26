use core::fmt;
use std::{
    collections::{HashMap, HashSet, VecDeque},
    io::{self, BufRead, BufReader, BufWriter, Read, Write},
};

// has to_string(), apply()
#[derive(Copy, Clone)]
enum Dir {
    L,
    R,
    UL,
    UR,
    DL,
    DR,
}

struct Solver<'a> {
    reader: BufReader<&'a mut dyn Read>,
    writer: BufWriter<&'a mut dyn Write>,
    width: usize,
    height: usize,
    board: Vec<u8>,
    start: (i32, i32),
    exit: (i32, i32),
}

const DEBUG: bool = false;

impl<'a> Solver<'a> {
    fn new(reader: &'a mut dyn Read, writer: &'a mut dyn Write) -> Solver<'a> {
        Self {
            reader: BufReader::new(reader),
            writer: BufWriter::new(writer),
            width: 0,
            height: 0,
            board: Vec::new(),
            start: (0, 0),
            exit: (0, 0),
        }
    }

    fn read(&mut self) {
        let mut line = String::new();
        self.reader.read_line(&mut line).expect("board size");
        let mut sizes = line.trim().split(" ").map(|s| s.parse().expect("number"));
        self.width = sizes.next().expect("width");
        self.height = sizes.next().expect("height");
        if DEBUG {
            eprintln!("width={} height={}", self.width, self.height);
        }
        self.board.resize(self.width * self.height, 0);
        // # : wall
        // . : free space
        // S : start
        // E : end
        // _ : sliding floor
        // Uppercase letters A, B, C, D : Closed doors
        // Lowercase letters a, b, c, d : Keys which open doors
        for y in 0..self.height {
            let mut line = String::new();
            self.reader.read_line(&mut line).expect("board size");
            let row = line.trim_matches('\n').as_bytes();
            assert_eq!(row.len(), self.width);
            if DEBUG {
                eprintln!("{}", line.trim_matches('\n'));
            }
            for x in 0..self.width {
                match row[x] {
                    b'S' => self.start = (x as i32, y as i32),
                    b'E' => self.exit = (x as i32, y as i32),
                    _ => (),
                }
            }
            self.board[y * self.width..(y + 1) * self.width].copy_from_slice(row);
        }
    }

    fn get(&self, x: i32, y: i32) -> u8 {
        assert!(x >= 0 && y >= 0 && x < 41 && y < 41);
        let x = x as usize;
        let base = self.width * y as usize;
        self.board[base + x]
    }

    fn apply(&self, keys: i32, x: i32, y: i32, dir: &Dir) -> Option<(i32, i32, i32)> {
        let mut next = dir.apply((x, y));
        let mut c = self.get(next.0, next.1);
        match c {
            b'#' => return None,
            b'A' | b'B' | b'C' | b'D' => {
                if keys & key_mask(c) == 0 {
                    return None;
                } else {
                    return Some((keys, next.0, next.1));
                }
            }
            b'.' | b'E' | b'S' => return Some((keys, next.0, next.1)),
            b'a' | b'b' | b'c' | b'd' => return Some((keys | key_mask(c), next.0, next.1)),
            _ => (),
        }
        loop {
            let prev = next;
            next = dir.apply(prev);
            c = self.get(next.0, next.1);
            match c {
                b'#' => return Some((keys, prev.0, prev.1)),
                b'A' | b'B' | b'C' | b'D' => {
                    if keys & key_mask(c) == 0 {
                        return Some((keys, prev.0, prev.1));
                    } else {
                        return Some((keys, next.0, next.1));
                    }
                }
                b'a' | b'b' | b'c' | b'd' => return Some((keys | key_mask(c), next.0, next.1)),
                b'.' | b'E' | b'S' => return Some((keys, next.0, next.1)),
                _ => (),
            }
        }
    }

    fn run(&mut self) {
        self.read();

        let mut queue = VecDeque::new();
        let mut visited = HashSet::new();
        let mut backtrack = HashMap::new();
        queue.push_back((pack_state(0, self.start.0, self.start.1), u16::MAX, Dir::L));
        visited.insert(queue.front().unwrap().0);
        let mut exit_state = 0;
        'bfs_loop: while let Some((packed_state, prev_state, prev_cmd)) = queue.pop_front() {
            let (cur_keys, cur_x, cur_y) = unpack_state(packed_state);
            if DEBUG {
                eprintln!(
                    "testing [{}] ({}, {}) with keys=0b{:0>4b} on cell '{}'",
                    packed_state,
                    cur_x,
                    cur_y,
                    cur_keys,
                    self.get(cur_x, cur_y) as char
                );
            }
            backtrack.insert(packed_state, (prev_state, prev_cmd));
            if self.exit == (cur_x, cur_y) {
                if DEBUG {
                    eprintln!("success!");
                }
                exit_state = packed_state;
                break 'bfs_loop;
            }
            for dir in Dir::VALUES {
                if let Some((keys, x, y)) = self.apply(cur_keys, cur_x, cur_y, &dir) {
                    let state = pack_state(keys, x, y);
                    if !visited.insert(state) {
                        continue;
                    }
                    queue.push_back((state, packed_state, dir));
                    if DEBUG {
                        eprintln!(
                            "{},{} (0b{:0>4b}) -{}-> {},{} (0b{:0>4b}) // {:?}",
                            cur_x, cur_y, cur_keys, dir, x, y, keys, &queue
                        );
                    }
                    backtrack.insert(state, (packed_state, dir));
                }
            }
        }
        if DEBUG {
            eprintln!("backtrack: {:?}", backtrack);
        }
        let mut commands = Vec::new();
        while let Some((state, dir)) = backtrack.get(&exit_state) {
            exit_state = *state;
            if exit_state == u16::MAX {
                break;
            }
            commands.push(dir);
            assert!(commands.len() < 20);
        }
        commands.reverse();
        if DEBUG {
            eprintln!("commands: {:?}", &commands);
        }
        let strings: Vec<_> = commands.iter().map(|&d| d.to_string()).collect();
        writeln!(self.writer, "{}", strings.join(" ")).unwrap();
        self.writer.flush().unwrap();
    }
}

fn main() {
    let streams = (&mut io::stdin(), &mut io::stdout());
    let mut solver = Solver::new(streams.0, streams.1);
    solver.run();
}

mod tests {
    #[test]
    fn test_1() {
        use crate::Solver;
        let mut t_in = r#"10 4
##########
#S.___.###
#######.E#
##########
"#
        .as_bytes();
        let t_out = "R R DR R";
        let mut out = Vec::new();
        Solver::new(&mut t_in, &mut out).run();
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out);
    }

    #[test]
    fn test_2() {
        use crate::Solver;
        let mut t_in = r#"10 10
##########
#S.#######
###.######
###.######
###..#####
##.#.#####
##a##A####
##.##.####
###..B..E#
##########
"#
        .as_bytes();
        let t_out = "R DR DR DL DL DL UR UR R DR DR DR DR R R";
        let mut out = Vec::new();
        Solver::new(&mut t_in, &mut out).run();
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out);
    }

    #[test]
    fn test_dir() {
        use crate::Dir;
        assert_eq!(Dir::R.apply((0, 0)), (1, 0));
        assert_eq!(Dir::DR.apply((0, 0)), (0, 1));
        assert_eq!(Dir::R.apply((0, 1)), (1, 1));
        assert_eq!(Dir::DR.apply((0, 1)), (1, 2));
        assert_eq!(Dir::UR.apply((0, 1)), (1, 0));
        assert_eq!(Dir::DR.apply((0, 2)), (0, 3));
        assert_eq!(Dir::UR.apply((0, 2)), (0, 1));
        for y in -3..=3 {
            for x in -3..3 {
                let pt = (x, y);
                assert_eq!(Dir::L.apply(Dir::R.apply(pt)), pt, "L(R(pt))");
                assert_eq!(Dir::UL.apply(Dir::DR.apply(pt)), pt, "UL(DR(pt))");
                assert_eq!(Dir::DL.apply(Dir::UR.apply(pt)), pt, "DL(UR(pt))");
            }
        }
    }

    #[test]
    fn test_state() {
        for x in 0..=40 {
            for y in 0..=40 {
                for keys in 0..=0xf {
                    let packed = crate::pack_state(keys, x, y);
                    let (ukeys, ux, uy) = crate::unpack_state(packed);
                    assert_eq!(keys, ukeys);
                    assert_eq!(x, ux);
                    assert_eq!(y, uy);
                }
            }
        }
    }
}

impl Dir {
    fn apply(&self, coords: (i32, i32)) -> (i32, i32) {
        match self {
            Dir::L => (coords.0 - 1, coords.1),
            Dir::R => (coords.0 + 1, coords.1),

            Dir::UR => (coords.0 + coords.1.abs() % 2, coords.1 - 1),
            Dir::DR => (coords.0 + coords.1.abs() % 2, coords.1 + 1),

            Dir::UL => (coords.0 - 1 + coords.1.abs() % 2, coords.1 - 1),
            Dir::DL => (coords.0 - 1 + coords.1.abs() % 2, coords.1 + 1),
        }
    }
}

impl Dir {
    const VALUES: [Dir; 6] = [Dir::L, Dir::UL, Dir::UR, Dir::R, Dir::DR, Dir::DL];
}

impl fmt::Display for Dir {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::L => write!(f, "L"),
            Self::UL => write!(f, "UL"),
            Self::DL => write!(f, "DL"),
            Self::R => write!(f, "R"),
            Self::UR => write!(f, "UR"),
            Self::DR => write!(f, "DR"),
        }
    }
}

impl fmt::Debug for Dir {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        fmt::Display::fmt(&self, f)
    }
}

fn pack_state(keys: i32, x: i32, y: i32) -> u16 {
    (((keys & 0xF) * 41 + (x & 0x3F)) * 41 + (y & 0x3F)) as u16
}

fn unpack_state(state: u16) -> (i32, i32, i32) {
    let y = state % 41;
    let x = (state / 41) % 41;
    let keys = state / (41 * 41);
    (keys as i32, x as i32, y as i32)
}

fn key_mask(c: u8) -> i32 {
    match c {
        b'a' | b'A' => 1,
        b'b' | b'B' => 2,
        b'c' | b'C' => 4,
        b'd' | b'D' => 8,
        _ => 0,
    }
}
