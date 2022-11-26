use std::io::{self, BufRead, BufReader, BufWriter, Read, Write};

struct Viewer {
    dir: u8,
    x: i32,
    y: i32,
}

struct Solver<'a> {
    reader: BufReader<&'a mut dyn Read>,
    writer: BufWriter<&'a mut dyn Write>,
    width: usize,
    height: usize,
    board_text: Vec<u8>,
    viewers: Vec<Viewer>,
    target: (i32, i32),
}

impl Viewer {
    fn new(dir: u8, x: i32, y: i32) -> Self {
        Self { dir, x, y }
    }
}

const DEBUG: bool = true;

fn vsmul(a: (i32, i32), b: (i32, i32)) -> i32 {
    a.0 * b.0 + a.1 * b.1
}

impl<'a> Solver<'a> {
    fn new(reader: &'a mut dyn Read, writer: &'a mut dyn Write) -> Solver<'a> {
        Self {
            reader: BufReader::new(reader),
            writer: BufWriter::new(writer),
            width: 0,
            height: 0,
            board_text: Vec::new(),
            viewers: Vec::new(),
            target: (0, 0),
        }
    }

    fn read(&mut self) {
        let mut line = String::new();
        self.reader.read_line(&mut line).expect("height");
        self.height = line.trim().parse().expect("number");

        let mut line = String::new();
        self.reader.read_line(&mut line).expect("width");
        self.width = line.trim().parse().expect("number");

        self.board_text.resize(self.width * self.height, '.' as u8);
        self.viewers.clear();

        if DEBUG {
            eprintln!("width={} height={}", self.width, self.height);
        }
        let mut have_target = false;
        for y in 0..self.height {
            let mut line = String::new();
            self.reader.read_line(&mut line).unwrap();
            if DEBUG {
                eprint!("{}", &line);
            }
            let bytes = line.trim().as_bytes();
            assert_eq!(bytes.len(), self.width);
            for x in 0..self.width {
                match bytes[x].into() {
                    '<' | '>' | 'v' | '^' => {
                        self.viewers.push(Viewer::new(bytes[x], x as i32, y as i32))
                    }
                    'Y' => {
                        assert!(!have_target);
                        self.target.0 = x as i32;
                        self.target.1 = y as i32;
                        have_target = true;
                    }
                    _ => (),
                }
            }
            self.board_text[y * self.width..(y + 1) * self.width].clone_from_slice(bytes);
        }
        if DEBUG {
            eprintln!("{} viewer(s)", self.viewers.len());
        }
    }

    fn get(&self, pt: (i32, i32)) -> char {
        assert!(pt.0 >= 0);
        assert!(pt.1 >= 0);
        assert!(pt.0 < self.width as i32);
        assert!(pt.1 < self.height as i32);

        let base = self.width * pt.1 as usize;
        let x = pt.0 as usize;
        self.board_text[base + x] as char
    }

    fn test_look(&self, from: (i32, i32), dir: (i32, i32)) -> bool {
        let dir_cw = (-dir.1, dir.0);
        let dir_ccw = (dir.1, -dir.0);
        let vft = (self.target.0 - from.0, self.target.1 - from.1);
        let a = vsmul(vft, dir);
        if a <= 0 {
            return false;
        }
        let m = vsmul(vft, dir_cw);
        if m == 0 {
            let mut pt = from;
            loop {
                pt.0 += dir.0;
                pt.1 += dir.1;
                if pt == self.target {
                    return true;
                }
                if self.get(pt) != '.' {
                    return false;
                }
            }
        } else {
            let b = m.abs();
            if b > a {
                return false;
            }
            let ov = if m > 0 { dir_cw } else { dir_ccw };
            for u in 1..a {
                let pt = (from.0 + u * dir.0, from.1 + u * dir.1);
                for v in 1..=b {
                    if u < v || a - u < b - v {
                        continue;
                    }
                    let pt = (pt.0 + v * ov.0, pt.1 + v * ov.1);
                    if self.get(pt) != '.' {
                        return false;
                    }
                }
            }
            true
        }
    }

    fn test_look_down(&self, from_x: i32, from_y: i32) -> bool {
        self.test_look((from_x, from_y), (0, 1))
    }

    fn test_look_up(&self, from_x: i32, from_y: i32) -> bool {
        self.test_look((from_x, from_y), (0, -1))
    }

    fn test_look_right(&self, from_x: i32, from_y: i32) -> bool {
        self.test_look((from_x, from_y), (1, 0))
    }

    fn test_look_left(&self, from_x: i32, from_y: i32) -> bool {
        self.test_look((from_x, from_y), (-1, 0))
    }

    fn solve(&mut self) -> u32 {
        let mut result = 0;
        for &Viewer { dir, x, y } in &self.viewers {
            if match dir.into() {
                '<' => self.test_look_left(x, y),
                '>' => self.test_look_right(x, y),
                '^' => self.test_look_up(x, y),
                'v' => self.test_look_down(x, y),
                _ => false,
            } {
                if DEBUG {
                    eprintln!("+ {} at x={}, y={}", dir as char, x, y);
                }
                result += 1;
            }
        }
        result
    }

    fn run(&mut self) {
        self.read();
        let result = self.solve();
        writeln!(self.writer, "{}", result).unwrap();
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
        let mut t_in = r#"4
6
...v..
......
..Y.#<
...^..
"#
        .as_bytes();
        let t_out = "2";
        let mut out = Vec::new();
        Solver::new(&mut t_in, &mut out).run();
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out);
    }

    #[test]
    fn test_2() {
        use crate::Solver;
        let mut t_in = r#"5
5
.....
..Y..
..v..
.....
.....
"#
        .as_bytes();
        let t_out = "0";
        let mut out = Vec::new();
        Solver::new(&mut t_in, &mut out).run();
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out);
    }

    #[test]
    fn test_3() {
        use crate::Solver;
        let mut t_in = r#"10
10
.........<
>.........
>........>
v...v....>
>........<
>#..##...<
>#..Y....<
>#........
.#..^....<
>#.......<
"#
        .as_bytes();
        let t_out = "6";
        let mut out = Vec::new();
        Solver::new(&mut t_in, &mut out).run();
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out);
    }

    #[test]
    fn test_4() {
        use crate::Solver;
        let mut t_in = r#"5
5
>...v
..#..
.#Y#.
..#..
^...<
"#
        .as_bytes();
        let t_out = "4";
        let mut out = Vec::new();
        Solver::new(&mut t_in, &mut out).run();
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out);
    }

    #[test]
    fn test_5() {
        use crate::Solver;
        let mut t_in = r#"32
64
................................................................
................................................................
................................................................
................................................................
.....|----------====------------------------====----------|.....
.....|.........................||.........................|.....
.....|......____________.......||.........................|.....
.....|.....|............|......||.........................|v....
.....|....(|............|)................................|.....
.....|.....|............|.................................|.....
.....|....>|............|).....||.........................|v....
.....|.....|____________|......||.........................|.....
.....|.........................||.........................|v....
.....|.....................O--------O.....................|.....
.....|.....................|(_)[][_]|.....__________......|v....
.....|=========...=========|[].[_][]|....[__________].....|.....
.....|)...///..............|...[_][]|....[]!======![].....|.....
.....|...///...............|-..-----|.....................|v....
.....|..///......_.........|....p===|.....................|v....
.....|].|||.....|.|........|....|===|.....................|v....
.....|].|||....(|.|).......|....p===|.....................|v....
.....|..|||.....|.|........|....|===|......\==^^==/.......|.....
.....|].|0|....(|.|).......|....p===|.....................|v....
.....|]>|0|.....|_|........O....|===O.....................|v....
.....|].|0|...............................................|v....
.....|..|||...............................................|v....
.....|.....................|........|.....................|v....
.....|----------====----------....----------====----------|v....
.......................................Y.........<....<..<<<....
................................................................
................................................................
................................................................
"#
        .as_bytes();
        let t_out = "1";
        let mut out = Vec::new();
        Solver::new(&mut t_in, &mut out).run();
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out);
    }
}
