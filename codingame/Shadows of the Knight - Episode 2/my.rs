use std::{
    fmt::Display,
    io,
    ops::{Add, Div, Mul, Sub},
};

use rand::{rngs::ThreadRng, Rng};

macro_rules! parse_input {
    ($x:expr, $t:ident) => {
        $x.trim().parse::<$t>().unwrap()
    };
}

#[derive(Clone, Copy, Debug, PartialEq)]
struct Vec2(i32, i32);

impl Vec2 {
    fn d2(&self) -> i32 {
        self.0 * self.0 + self.1 * self.1
    }
    fn tuple(&self) -> (i32, i32) {
        (self.0, self.1)
    }
}

#[derive(Clone, Copy, Debug)]
struct Line {
    y: i32,
    left: i32,
    right: i32,
}

impl Line {
    fn new(y: i32, left: i32, right: i32) -> Self {
        Self { y, left, right }
    }
}

struct State {
    maxx: i32,
    maxy: i32,
    prev: Vec2,
    cur: Vec2,
    lines: Vec<Line>,
    rng: Box<ThreadRng>,
}

fn split_line_coeffs(near: Vec2, far: Vec2) -> (f64, f64) {
    assert_ne!(near.0, far.0);
    let mid_x = (near.0 + far.0) as f64 / 2_f64;
    let mid_y = (near.1 + far.1) as f64 / 2_f64;
    let dir_x = (far.0 - near.0) as f64;
    let dir_y = (far.1 - near.1) as f64;
    // dir_x * x + dir_y * y + c = 0
    // dir_x * mid_x + dir_y * mid_y = - c
    let dyx = dir_y / dir_x;
    let c = -(mid_x + dyx * mid_y);
    // x = - (c + dir_y * y) / dir_x
    (-dyx, -c)
}

impl State {
    fn new(width: i32, height: i32, _n: i32, x0: i32, y0: i32) -> Self {
        Self {
            maxx: width - 1,
            maxy: height - 1,
            prev: Vec2(-1, -1),
            cur: Vec2(x0, y0),
            lines: (0..height).map(|y| Line::new(y, 0, width - 1)).collect(),
            rng: Box::new(rand::thread_rng()),
        }
    }

    fn first(&self) -> Vec2 {
        Vec2(self.maxx - self.cur.0, self.maxy - self.cur.1)
    }

    fn valid(&self, pt: Vec2) -> bool {
        pt.0 >= 0 && pt.1 >= 0 && pt.0 <= self.maxx && pt.1 <= self.maxy
    }

    fn next_move(&mut self) -> Vec2 {
        let mut pt = self.next_move_v2();
        while pt == self.prev || pt == self.cur {
            let line = self.lines[self.rng.gen_range(0..self.lines.len())];
            let x = if self.rng.gen() {
                line.left
            } else {
                line.right
            };
            pt = Vec2(x, line.y)
        }
        pt
    }

    fn center_v1(&self) -> Vec2 {
        if self.lines.len() == 1 {
            let line = self.lines[0];
            return Vec2((line.left + line.right) / 2, line.y);
        }

        let mut sx = 0i32;
        let mut sy = 0i32;
        let mut s = 0i32;

        for line in &self.lines {
            let c = line.right - line.left + 1;
            s += c;
            sy += c * line.y;
            sx += (line.right - line.left + 1) * (line.right + line.left) / 2;
        }
        eprintln!("lines.len() = {}, s = {}", self.lines.len(), s);

        Vec2(sx / s, sy / s)
    }

    fn next_move_v2(&mut self) -> Vec2 {
        let center = self.center_v1();
        let cur = self.cur;
        eprintln!("center: {}, cur: {}", center, cur);

        let s: i32 = self.lines.iter().map(|l| l.right - l.left + 1).sum();
        let mut e = s as u32;
        let mut pt = cur;
        for _ in 0..1000 {
            let p = Vec2(
                self.rng.gen_range(0..=self.maxx),
                self.rng.gen_range(0..=self.maxy),
            );
            let l2 = self.split_lines(p, cur);
            let s2: i32 = l2.iter().map(|l| l.right - l.left + 1).sum();
            if s.abs_diff(s2 * 2) < e {
                e = s.abs_diff(s2 * 2);
                pt = p;
            }
            if e * 10 < s as u32 {
                break;
            }
        }
        eprintln!("s={}, e={}", s, e);
        pt
    }

    fn next_move_v1(&self) -> Vec2 {
        let center = self.center_v1();
        let cur = self.cur;
        eprintln!("center: {}, cur: {}", center, cur);

        if center.0 == cur.0 {
            Vec2(center.0, (center.1 * 2 - cur.1).max(0).min(self.maxy))
        } else if center.1 == cur.1 {
            Vec2((center.0 * 2 - cur.0).max(0).min(self.maxx), center.1)
        } else {
            let mut a = self.cur;
            let mut b = center * 2 - a;
            while !(a == b) {
                let mid = (a + b) / 2;
                if mid == a {
                    break;
                }
                if self.valid(mid) {
                    a = mid
                } else {
                    b = mid
                }
            }
            a
        }
    }

    fn cut_even(&mut self) {
        if self.prev.0 == self.cur.0 {
            let yy = (self.prev.1 + self.cur.1) / 2;
            let new_lines = self
                .lines
                .iter()
                .map(|&x| x)
                .filter(|line| line.y == yy)
                .collect();
            self.lines = new_lines;
        } else {
            let (cy, c) = split_line_coeffs(self.prev, self.cur);
            let new_lines = self
                .lines
                .iter()
                .map(|&line| {
                    let xf = cy * line.y as f64 + c;
                    let (xl, xr) = (xf.floor() as i32, xf.ceil() as i32);
                    if xr < line.left || xl > line.right {
                        Line::new(-1, 0, 0)
                    } else {
                        Line::new(line.y, i32::max(xl, line.left), i32::min(xr, line.right))
                    }
                })
                .filter(|line| line.y >= 0)
                .collect();
            self.lines = new_lines;
        }
    }

    fn split_lines(&self, near: Vec2, far: Vec2) -> Vec<Line> {
        if far.0 == near.0 {
            self.lines
                .iter()
                .filter(|line| (line.y - near.1).abs() < (line.y - far.1).abs())
                .map(|x| *x)
                .collect()
        } else if near.0 < far.0 {
            let (cy, c) = split_line_coeffs(near, far);
            self.lines
                .iter()
                .map(|&line| {
                    let cutxf = cy * line.y as f64 + c;
                    let cutx = cutxf.floor() as i32;
                    if cutx < line.left {
                        Line::new(-1, 0, 0)
                    } else {
                        Line::new(line.y, line.left, cutx.min(line.right))
                    }
                })
                .filter(|line| line.y >= 0)
                .collect()
        } else {
            let (cy, c) = split_line_coeffs(near, far);
            self.lines
                .iter()
                .map(|&line| {
                    let cutxf = cy * line.y as f64 + c;
                    let cutx = cutxf.ceil() as i32;
                    if cutx > line.right {
                        Line::new(-1, 0, 0)
                    } else {
                        Line::new(line.y, cutx.max(line.left), line.right)
                    }
                })
                .filter(|line| line.y >= 0)
                .collect()
        }
    }

    fn cut_uneven(&mut self, near: Vec2, far: Vec2) {
        self.lines = self.split_lines(near, far);
    }

    fn step(&mut self, dir: String) -> (i32, i32) {
        eprintln!("dir: {}", dir);
        let result = match dir.as_str() {
            "UNKNOWN" => self.first(),
            "SAME" => {
                self.cut_even();
                self.next_move()
            }
            "WARMER" => {
                self.cut_uneven(self.cur, self.prev);
                self.next_move()
            }
            "COLDER" => {
                self.cut_uneven(self.prev, self.cur);
                self.next_move()
            }
            _ => {
                assert!(false);
                Vec2(-1, -1)
            }
        };

        assert!(result.0 >= 0 && result.1 >= 0);
        (self.prev, self.cur) = (self.cur, result);
        eprintln!("answer: {}", result);
        (result.0, result.1)
    }
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
fn main() {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let inputs = input_line.split(" ").collect::<Vec<_>>();
    let w = parse_input!(inputs[0], i32); // width of the building.
    let h = parse_input!(inputs[1], i32); // height of the building.
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let n = parse_input!(input_line, i32); // maximum number of turns before game over.
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let inputs = input_line.split(" ").collect::<Vec<_>>();
    let x0 = parse_input!(inputs[0], i32);
    let y0 = parse_input!(inputs[1], i32);

    let mut state = State::new(w, h, n, x0, y0);
    // game loop
    loop {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let bomb_dir = input_line.trim().to_string(); // Current distance to the bomb compared to previous distance (COLDER, WARMER, SAME or UNKNOWN)

        let (x, y) = state.step(bomb_dir);
        println!("{} {}", x, y);
    }
}

mod test {
    #[test]
    fn test() {
        use crate::*;
        let n = 20;
        let mut prev = (3, 5);
        let mut state = State::new(100, 100, n, prev.0, prev.1);
        let mut cur = state.step("UNKNOWN".to_string());
        let dst = (44, 71);
        let mut success = false;
        for _ in 0..n {
            if cur == dst {
                success = true;
                break;
            }
            let dprev = (dst.0 - prev.0) * (dst.0 - prev.0) + (dst.1 - prev.1) * (dst.1 - prev.1);
            let dcur = (dst.0 - cur.0) * (dst.0 - cur.0) + (dst.1 - cur.1) * (dst.1 - cur.1);
            prev = cur;
            cur = state.step(if dprev == dcur {
                "SAME".to_string()
            } else if dprev < dcur {
                "COLDER".to_string()
            } else {
                "WARMER".to_string()
            })
        }
        assert!(success);
    }
}

impl Mul<Vec2> for Vec2 {
    type Output = i32;
    fn mul(self, rhs: Self) -> Self::Output {
        self.0 * rhs.0 + self.1 * rhs.1
    }
}

impl Mul<i32> for Vec2 {
    type Output = Vec2;
    fn mul(self, rhs: i32) -> Self::Output {
        Vec2(self.0 * rhs, self.1 * rhs)
    }
}

impl Add<Vec2> for Vec2 {
    type Output = Vec2;
    fn add(self, rhs: Vec2) -> Self::Output {
        Vec2(self.0 + rhs.0, self.1 + rhs.1)
    }
}

impl Sub<Vec2> for Vec2 {
    type Output = Vec2;
    fn sub(self, rhs: Vec2) -> Self::Output {
        Vec2(self.0 - rhs.0, self.1 - rhs.1)
    }
}

impl Div<i32> for Vec2 {
    type Output = Vec2;
    fn div(self, rhs: i32) -> Self::Output {
        Vec2(self.0 / rhs, self.1 / rhs)
    }
}

impl Display for Vec2 {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "Vec2({}, {})", self.0, self.1)
    }
}
