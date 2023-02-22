use std::io::{self, BufReader, BufWriter, Read, Write};

const ECHO: bool = false;
const DEBUG: bool = true;

mod v1 {
    use core::fmt;
    use std::{
        collections::{HashMap, VecDeque},
        io::{BufRead, BufReader, BufWriter, Read, Write},
    };

    use crate::{DEBUG, ECHO};

    #[derive(Clone, Copy)]
    enum Dir {
        DL,
        DR,
        L,
        R,
        UL,
        UR,
    }

    #[derive(Clone, Copy)]
    struct Pos {
        x: i32,
        y: i32,
        keys: [bool; 4],
    }

    pub struct State {
        width: i32,
        height: i32,
        board: [[u8; 40]; 40],
        start: (i32, i32),
        exit: (i32, i32),
        solution: VecDeque<Dir>,
    }

    impl State {
        pub fn new() -> Self {
            Self {
                width: 0,
                height: 0,
                board: [[0; 40]; 40],
                start: (0, 0),
                exit: (0, 0),
                solution: VecDeque::new(),
            }
        }

        pub fn read(&mut self, reader: &mut BufReader<&mut dyn Read>) {
            let mut buf = String::new();
            reader.read_line(&mut buf).unwrap();
            if ECHO {
                eprintln!("{}", buf.trim());
            }
            // line1: width height
            let size: Vec<_> = buf
                .split(" ")
                .map(|s| s.trim().parse::<i32>().unwrap())
                .collect();
            self.width = size[0];
            self.height = size[1];
            for y in 0..self.height as usize {
                buf = String::new();
                reader.read_line(&mut buf).unwrap();
                if ECHO {
                    eprintln!("{}", buf.trim());
                }
                assert_eq!(buf.trim().len(), self.width as usize);
                let data = buf.as_bytes();
                for x in 0..self.width as usize {
                    self.board[y][x] = match data[x] {
                        b'S' => {
                            self.start = (x as i32, y as i32);
                            b'.'
                        }
                        b'E' => {
                            self.exit = (x as i32, y as i32);
                            b'.'
                        }
                        b => b,
                    };
                }
            }
        }

        pub fn write(&mut self, writer: &mut BufWriter<&mut dyn Write>) {
            let mut flag = false;
            for dir in &self.solution {
                if flag {
                    write!(writer, " ").unwrap();
                } else {
                    flag = true;
                }
                write!(writer, "{}", dir).unwrap();
            }
            writeln!(writer).unwrap();
        }

        fn get(&self, pos: &Pos) -> u8 {
            self.board[pos.y as usize][pos.x as usize]
        }

        fn apply(&self, from: &Pos, dir: Dir) -> Option<Pos> {
            let mut cur = *from;
            let mut sliding = true;
            'moving: loop {
                let mut next = cur.apply(dir);
                if next.x < 0 || next.x > 40 || next.y < 0 || next.y > 40 {
                    break 'moving;
                }
                let next_cell = self.get(&next);
                sliding &= next_cell == b'_';
                match next_cell {
                    b'#' => {
                        break 'moving;
                    }
                    b'A' | b'B' | b'C' | b'D' => {
                        let key_needed = next_cell - b'A';
                        if !from.keys[key_needed as usize] {
                            break 'moving;
                        }
                    }
                    b'a' | b'b' | b'c' | b'd' => {
                        let key_added = next_cell - b'a';
                        next.keys[key_added as usize] = true;
                    }
                    _ => (),
                }
                cur = next;

                if !sliding {
                    break;
                }
            }

            if cur.same_coords(from) {
                None
            } else {
                Some(cur)
            }
        }

        pub fn solve(&mut self) {
            let mut backtrack = HashMap::new();
            let mut backtrack_dir = HashMap::new();
            let mut queue = VecDeque::new();
            let start_pos = Pos::new(self.start.0, self.start.1);
            let exit_pos = Pos::new(self.exit.0, self.exit.1);

            let mut exit_state = None;
            let packed_start = start_pos.pack();
            backtrack.insert(packed_start, -1);
            queue.push_back(packed_start);
            while let Some(packed_cur) = queue.pop_front() {
                let cur = Pos::unpack(packed_cur);
                if cur.same_coords(&exit_pos) {
                    exit_state = Some(packed_cur);
                    break;
                }
                for dir in Dir::VALUES {
                    if let Some(next) = self.apply(&cur, dir) {
                        let packed_next = next.pack();
                        if !backtrack.contains_key(&packed_next) {
                            backtrack.insert(packed_next, packed_cur);
                            backtrack_dir.insert(packed_next, dir);
                            queue.push_back(packed_next);
                        }
                    }
                }
            }
            if DEBUG {
                eprintln!(
                    "search done, visited {} positions, result: {:?}",
                    backtrack.len(),
                    exit_state
                );
            }
            if let Some(mut state) = exit_state {
                let mut pos_track = VecDeque::new();
                let mut cmd_track = VecDeque::new();
                while state != -1 {
                    if let Some(&dir) = backtrack_dir.get(&state) {
                        cmd_track.push_front(dir);
                        pos_track.push_front(Pos::unpack(state));
                    } else {
                        break;
                    }
                    state = *backtrack.get(&state).expect("backtrack should be valid!");
                }
                if DEBUG {
                    eprintln!("collected pos backtrack: {:?}", pos_track);
                    eprintln!("collected cmd backtrack: {:?}", cmd_track);
                }
                for dir in cmd_track.into_iter() {
                    self.solution.push_back(dir);
                }
            }
        }
    }

    impl Pos {
        fn same_coords(&self, other: &Self) -> bool {
            self.x == other.x && self.y == other.y
        }

        fn pack(&self) -> i32 {
            assert!(self.x >= 0 && self.x <= 40, "invalid pos.x: {}", self.x);
            assert!(self.y >= 0 && self.y <= 40, "invalid pos.y: {}", self.y);
            let key_mask = (0..4)
                .map(|i| if self.keys[i] { 1 << i } else { 0 })
                .fold(0, |a, b| a | b);
            (key_mask * 41 + self.y) * 41 + self.x
        }

        fn unpack(v: i32) -> Self {
            let (x, a) = (v % 41, v / 41);
            let (y, k) = (a % 41, a / 41);
            let mut keys = [false; 4];
            for i in 0..4 {
                if k & (1 << i) != 0 {
                    keys[i] = true;
                }
            }
            Self { x, y, keys }
        }

        fn new(x: i32, y: i32) -> Self {
            Self {
                x: x,
                y: y,
                keys: [false; 4],
            }
        }

        fn apply(&self, dir: Dir) -> Self {
            let (x, y) = match dir {
                Dir::DL => (self.x - 1 + self.y.abs() % 2, self.y + 1),
                Dir::UL => (self.x - 1 + self.y.abs() % 2, self.y - 1),

                Dir::L => (self.x - 1, self.y),
                Dir::R => (self.x + 1, self.y),

                Dir::DR => (self.x + self.y.abs() % 2, self.y + 1),
                Dir::UR => (self.x + self.y.abs() % 2, self.y - 1),
            };
            Self {
                x,
                y,
                keys: self.keys,
            }
        }
    }

    impl Dir {
        const VALUES: [Dir; 6] = [Dir::DL, Dir::DR, Dir::L, Dir::R, Dir::UL, Dir::UR];
    }

    impl fmt::Display for Dir {
        fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
            match self {
                Self::DL => write!(f, "DL"),
                Self::DR => write!(f, "DR"),
                Self::L => write!(f, "L"),
                Self::R => write!(f, "R"),
                Self::UL => write!(f, "UL"),
                Self::UR => write!(f, "UR"),
            }
        }
    }

    impl fmt::Display for Pos {
        fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
            write!(
                f,
                "Pos(x={}, y={}, keys=[{},{},{},{}]",
                self.x,
                self.y,
                if self.keys[0] { '+' } else { '-' },
                if self.keys[1] { '+' } else { '-' },
                if self.keys[2] { '+' } else { '-' },
                if self.keys[3] { '+' } else { '-' }
            )
        }
    }

    impl fmt::Debug for Pos {
        fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
            fmt::Display::fmt(&self, f)
        }
    }

    impl fmt::Debug for Dir {
        fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
            fmt::Display::fmt(&self, f)
        }
    }
}

fn solve(reader: &mut dyn Read, writer: &mut dyn Write) {
    let mut reader = BufReader::new(reader);
    let mut writer = BufWriter::new(writer);

    use v1::State;
    let state = &mut State::new();
    state.read(&mut reader);
    state.solve();
    state.write(&mut writer);
}

fn main() {
    let mut streams = (io::stdin(), io::stdout());

    solve(&mut streams.0, &mut streams.1);
}

mod test {
    #[test]
    fn test1() {
        let mut t_in = r#"10 4
##########
#S.___.###
#######.E#
##########
"#
        .as_bytes();
        let t_out = "R R DR R";
        let mut out = Vec::new();
        crate::solve(&mut t_in, &mut out);
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out.trim());
    }

    #[test]
    fn test2() {
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
        crate::solve(&mut t_in, &mut out);
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out.trim());
    }
}
