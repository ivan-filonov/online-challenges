use std::{
    collections::{HashMap, HashSet, VecDeque},
    hash::Hash,
    io::{self, BufRead, BufReader, BufWriter, Read, Write},
};

struct Solver<'a> {
    reader: BufReader<&'a mut dyn Read>,
    writer: BufWriter<&'a mut dyn Write>,
    bots: [u8; 6],
}

const DEBUG: bool = true;

#[derive(Debug, Clone, Copy)]
enum Dir {
    D,
    L,
    R,
    U,
}

impl<'a> Solver<'a> {
    fn new(reader: &'a mut dyn Read, writer: &'a mut dyn Write) -> Solver<'a> {
        Self {
            reader: BufReader::new(reader),
            writer: BufWriter::new(writer),
            bots: [0xff; 6],
        }
    }

    fn read(&mut self) {
        for y in 0..5 {
            let mut line = String::new();
            self.reader.read_line(&mut line).unwrap();
            if DEBUG {
                eprintln!("{}", line.trim_matches('\n'));
            }
            let line = line.trim_matches('\n').as_bytes();
            assert_eq!(line.len(), 5);
            for x in 0..5 {
                let c = line[x];
                match c {
                    b'.' => (),
                    b'A' | b'B' | b'C' | b'D' | b'E' => {
                        self.bots[(c - b'A') as usize] = (y * 5 + x) as u8
                    }
                    b'X' => self.bots[5] = (y * 5 + x) as u8,
                    _ => panic!("dunno that to do with '{}'", c as char),
                }
            }
        }
    }

    fn cast(x: i32, y: i32, dir: Dir) -> u32 {
        let mut result = 0u32;
        let (dx, dy) = (dir.dx(), dir.dy());
        let (mut x, mut y) = (x + dx, y + dy);
        while x >= 0 && x < 5 && y >= 0 && y < 5 {
            result |= 1 << (x + y * 5);
            x += dx;
            y += dy;
        }
        result
    }

    fn mask_of(state: &[u8; 6]) -> u32 {
        let mut result = 0;
        for &c in state {
            if c < 25 {
                result |= 1 << c;
            }
        }
        result
    }

    const BOTS: &'static [u8] = "ABCDEX".as_bytes();
    fn bot_char(bot_id: usize) -> char {
        Self::BOTS[bot_id] as char
    }

    fn search(&mut self) -> Option<(String, [u8; 6])> {
        let mut visited = HashSet::new();
        let mut queue = VecDeque::new();
        let mut from: HashMap<[u8; 6], ([u8; 6], usize, Dir)> = HashMap::new();

        visited.insert(self.bots);
        queue.push_back(self.bots);

        while let Some(state) = queue.pop_front() {
            let vcoords = state
                .iter()
                .map(|&s| ((s % 5) as i32, (s / 5) as i32))
                .collect::<Vec<_>>();
            let mut coords: [(i32, i32); 6] = [(0, 0); 6];
            coords.copy_from_slice(vcoords.as_slice());
            let mask = Self::mask_of(&state);
            if DEBUG {
                eprintln!("test {:?}\n            mask 0b{:0>25b}", &coords, &mask);
            }
            if coords[5] == (2, 2) {
                if DEBUG {
                    eprintln!("victory condition detected!");
                }
                let mut key = state;
                let mut commands = Vec::new();
                while let Some((key_prev, bot_id, dir)) = from.get(&key) {
                    commands.push(std::format!("{}{:?}", Self::bot_char(*bot_id), dir));
                    key.copy_from_slice(key_prev);
                }
                commands.reverse();
                let result_commands = commands.join(" ");
                if DEBUG {
                    eprintln!("commands: {}", result_commands);
                    eprintln!("final state: {:?}", state);
                }
                return Some((result_commands, state));
            }
            for bot_id in 0..6 {
                if state[bot_id] > 25 {
                    continue;
                }
                //let mask = if bot_id < 5 { mask } else { mask | (1 << 18) };
                for dir in Dir::ALL {
                    let ray = Self::cast(coords[bot_id].0, coords[bot_id].1, dir);
                    if ray & mask == 0 {
                        continue;
                    }
                    if DEBUG {
                        eprintln!(
                            "can move {} {:?} ray=0b{:0>25b}",
                            Self::bot_char(bot_id),
                            &dir,
                            ray
                        );
                    }
                    let (mut x, mut y) = coords[bot_id];
                    let (dx, dy) = (dir.dx(), dir.dy());
                    let win_mask = if bot_id < 5 { 0 } else { 1u32 << 13 };
                    loop {
                        let next_bit = 1u32 << ((y + dy) * 5 + (x + dx));
                        if mask & next_bit != 0 {
                            break;
                        }
                        x += dx;
                        y += dy;
                        if win_mask & next_bit != 0 {
                            break;
                        }
                    }
                    let mut new_state = state;
                    new_state[bot_id] = (y * 5 + x) as u8;

                    if visited.contains(&new_state) {
                        continue;
                    }
                    from.insert(new_state, (state, bot_id, dir.clone()));
                    visited.insert(new_state.clone());
                    queue.push_back(new_state);
                }
            }
        }
        None
    }

    fn run(&mut self) {
        self.read();
        let result = self.search();
        let (commands, state) = result.unwrap();

        writeln!(self.writer, "{}\n", commands).unwrap();
        let mut grid = [b'.'; 25];
        for i in 0..6 {
            let pos = state[i];
            if pos == 255 {
                continue;
            }
            grid[pos as usize] = Self::bot_char(i) as u8;
        }
        for y in 0..5 {
            let line: String = grid[y * 5..(y + 1) * 5]
                .iter()
                .map(|&x| x as char)
                .collect();
            writeln!(self.writer, "{}", line).unwrap();
        }
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
        let mut t_in = r#".....
A.D..
.C...
..X..
B....
"#
        .as_bytes();
        let t_out = "XU

.....
A.D..
.CX..
.....
B....";
        let mut out = Vec::new();
        Solver::new(&mut t_in, &mut out).run();
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out);
    }

    #[test]
    fn test_2() {
        use crate::Solver;
        let mut t_in = r#".....
A.D..
B....
....E
C..X.
....."#
            .as_bytes();
        let t_out = "AR XL AD AR CU CR CU BR XU XR

.....
A.D..
.CX..
.....
B....";
        let mut out = Vec::new();
        Solver::new(&mut t_in, &mut out).run();
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out);
    }
}

impl Dir {
    const ALL: [Dir; 4] = [Dir::D, Dir::L, Dir::R, Dir::U];

    fn dx(&self) -> i32 {
        match self {
            Dir::L => -1,
            Dir::R => 1,
            _ => 0,
        }
    }

    fn dy(&self) -> i32 {
        match self {
            Dir::U => -1,
            Dir::D => 1,
            _ => 0,
        }
    }
}
