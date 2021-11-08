//use std::io::prelude::*;
use std::collections::{HashMap, HashSet};
use std::convert::TryInto;
use std::io::{BufRead, BufReader, BufWriter, Read, Write};
use std::ops::{Shl, Shr};

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
    let input_line = reader
        .lines()
        .map(|r| r.unwrap())
        .fold(String::new(), |acc, s| acc + &s);
    let parts: Vec<&str> = input_line.split(char::is_whitespace).collect();
    assert_eq!(parts.len(), 8);

    let (start_x, start_y) = coords_from_string(parts[0]);
    let (finish_x, finish_y) = coords_from_string(parts[1]);
    let weights: Vec<_> = parts
        .into_iter()
        .skip(2)
        .map(|s| s.parse::<i32>().unwrap())
        .collect::<Vec<i32>>()
        .try_into()
        .unwrap();
    assert_eq!(weights.len(), 6);
    //dbg!(&weights);
    //dbg!(pack_point(start_x, start_y, Dice::new().pack()));

    let mut dist: HashMap<u32, i32> = HashMap::new();
    let mut visited: HashSet<u32> = HashSet::new();
    let mut frontier: HashSet<u32> = HashSet::new();
    let mut prev: HashMap<u32, u32> = HashMap::new();

    {
        let start = pack_point(start_x, start_y, Dice::new().pack());
        dist.insert(start, weights[BOTTOM]);
        frontier.insert(start);
    }

    while !frontier.is_empty() {
        let mut cur_pt = 0u32;
        let mut cur_dist = i32::MAX;
        for pt in frontier.iter() {
            let od = dist.get(pt);
            if od.is_none() {
                continue;
            }
            let d = *od.unwrap();
            if d < cur_dist {
                cur_dist = d;
                cur_pt = *pt;
            }
        }
        let cur_dist = cur_dist;
        frontier.remove(&cur_pt);
        visited.insert(cur_pt);

        let (cur_x, cur_y, packed_dice) = unpack_point(cur_pt);
        let dice = Dice::unpack(packed_dice);

        if cur_x > 0 {
            let (nx, ny, ndice) = (cur_x - 1, cur_y, dice.roll(Roll::Left));
            dijkstra_step(
                cur_pt,
                nx,
                ny,
                &ndice,
                cur_dist,
                &weights,
                &mut dist,
                &mut frontier,
                &visited,
                &mut prev,
            );
        }
        if cur_x < 7 {
            let (nx, ny, ndice) = (cur_x + 1, cur_y, dice.roll(Roll::Right));
            dijkstra_step(
                cur_pt,
                nx,
                ny,
                &ndice,
                cur_dist,
                &weights,
                &mut dist,
                &mut frontier,
                &visited,
                &mut prev,
            );
        }
        if cur_y > 0 {
            let (nx, ny, ndice) = (cur_x, cur_y - 1, dice.roll(Roll::Back));
            dijkstra_step(
                cur_pt,
                nx,
                ny,
                &ndice,
                cur_dist,
                &weights,
                &mut dist,
                &mut frontier,
                &visited,
                &mut prev,
            );
        }
        if cur_y < 7 {
            let (nx, ny, ndice) = (cur_x, cur_y + 1, dice.roll(Roll::Forward));
            dijkstra_step(
                cur_pt,
                nx,
                ny,
                &ndice,
                cur_dist,
                &weights,
                &mut dist,
                &mut frontier,
                &visited,
                &mut prev,
            );
        }
    }

    let mut best_finish = 0u32;
    let mut best_dist = i32::MAX;
    for (pt, d) in dist {
        let (x, y, packed_dice) = unpack_point(pt);
        if x == finish_x && y == finish_y {
            if d < best_dist {
                best_finish = pt;
                best_dist = d;
            }
        }
    }

    let mut path: Vec<String> = Vec::new();
    let mut pt = best_finish;
    while pt != 0 {
        let (x, y, _) = unpack_point(pt);
        path.push(coords_to_string(x, y));
        pt = *prev.get(&pt).unwrap_or(&0);
    }
    path.reverse();

    write!(writer, "{}", best_dist);
    for s in path {
        write!(writer, " {}", s);
    }
    writeln!(writer);
}

fn dijkstra_step(
    code: u32,
    nx: i32,
    ny: i32,
    ndice: &Dice,
    cur_dist: i32,
    weights: &Vec<i32>,
    dist: &mut HashMap<u32, i32>,
    frontier: &mut HashSet<u32>,
    visited: &HashSet<u32>,
    prev: &mut HashMap<u32, u32>,
) {
    let ncode = pack_point(nx, ny, ndice.pack());
    if visited.contains(&ncode) {
        return;
    }
    let ndist = cur_dist + weights[ndice.id as usize];
    if !dist.contains_key(&ncode) {
        dist.insert(ncode, ndist);
        prev.insert(ncode, code);
    } else if dist[&ncode] > ndist {
        dist.insert(ncode, ndist);
        prev.insert(ncode, code);
    }
    frontier.insert(ncode);
}

fn pack_point(x: i32, y: i32, dice: u32) -> u32 {
    x as u32 | y.shl(3) as u32 | dice.shl(6)
}

fn unpack_point(pt: u32) -> (i32, i32, u32) {
    (pt as i32 % 8, pt.shr(3) as i32 % 8, pt.shr(6))
}

// NEAR, FAR, TOP, RIGHT, BOTTOM, LEFT
const NEAR: usize = 0;
const FAR: usize = 1;
const TOP: usize = 2;
const RIGHT: usize = 3;
const BOTTOM: usize = 4;
const LEFT: usize = 5;
const T_LEFT: [(usize, usize); 6] = [
    (NEAR, NEAR),
    (FAR, FAR),
    (TOP, LEFT),
    (BOTTOM, RIGHT),
    (LEFT, BOTTOM),
    (RIGHT, TOP),
];
const T_FORWARD: [(usize, usize); 6] = [
    (LEFT, LEFT),
    (RIGHT, RIGHT),
    (TOP, FAR),
    (BOTTOM, NEAR),
    (NEAR, TOP),
    (FAR, BOTTOM),
];

fn coords_to_string(x: i32, y: i32) -> String {
    assert!(x >= 0 && x < 8 && y >= 0 && y < 8);
    let mut b = [0u8; 2];
    b[0] = x as u8 + 'a' as u8;
    b[1] = y as u8 + '1' as u8;
    std::str::from_utf8(&b).unwrap().to_string()
}

fn coords_from_string(s: &str) -> (i32, i32) {
    assert_eq!(s.len(), 2);
    let b = s.as_bytes();
    ((b[0] - 'a' as u8) as i32, (b[1] - '1' as u8) as i32)
}

#[derive(Debug)]
struct Dice {
    ids: [u8; 6],
    id: u8,
}

impl PartialEq for Dice {
    fn eq(&self, other: &Dice) -> bool {
        self.ids == other.ids && self.id == other.id
    }
}

enum Roll {
    Forward,
    Back,
    Left,
    Right,
}

impl Dice {
    fn pack(&self) -> u32 {
        self.ids
            .iter()
            .enumerate()
            .map(|(n, id)| (*id as u32).shl(n * 3))
            .fold(0u32, |acc, x| acc | x)
    }
    fn unpack(packed: u32) -> Self {
        let mut ids = [0u8; 6];
        for n in 0..6 {
            ids[n] = (packed.shr(n * 3) % 8) as u8;
        }
        Self {
            id: ids[BOTTOM],
            ids: ids,
        }
    }
    fn new() -> Self {
        Self {
            ids: [
                NEAR as u8,
                FAR as u8,
                TOP as u8,
                RIGHT as u8,
                BOTTOM as u8,
                LEFT as u8,
            ],
            id: BOTTOM as u8,
        }
    }
    fn roll(&self, roll: Roll) -> Self {
        let (table, dir) = match roll {
            Roll::Forward => (&T_FORWARD, false),
            Roll::Back => (&T_FORWARD, true),
            Roll::Left => (&T_LEFT, false),
            Roll::Right => (&T_LEFT, true),
        };
        let mut ni = [0u8; 6];
        if dir {
            for &(to, from) in table {
                ni[to] = self.ids[from];
            }
        } else {
            for &(from, to) in table {
                ni[to] = self.ids[from];
            }
        }
        Self {
            id: ni[BOTTOM] as u8,
            ids: ni,
        }
    }
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn test_coords() {
        use crate::{coords_from_string, coords_to_string};
        for c in ["a1", "e3", "d6", "h8"] {
            let cc = coords_from_string(&c);
            assert_eq!(c, coords_to_string(cc.0, cc.1));
        }
    }

    #[test]
    fn test_dice() {
        use crate::*;
        let d = Dice::new();
        assert_eq!(d.id, BOTTOM as u8);
        assert_eq!(d, Dice::unpack(d.pack()));
        let dl = d.roll(Roll::Left);
        assert_eq!(dl.id, LEFT as u8);
        assert_eq!(dl, Dice::unpack(dl.pack()));
        let dl2 = dl.roll(Roll::Left);
        assert_eq!(dl2.id, TOP as u8);
        assert_eq!(dl2, Dice::unpack(dl2.pack()));
        let dr = d.roll(Roll::Right);
        assert_eq!(dr.id, RIGHT as u8);
        let dr2 = dr.roll(Roll::Right);
        assert_eq!(dr2.id, TOP as u8);
        let dr3 = dr2.roll(Roll::Right);
        assert_eq!(dr3.id, LEFT as u8);
        assert_eq!(dr3, Dice::unpack(dr3.pack()));
        let df = d.roll(Roll::Forward);
        assert_eq!(df.id, FAR as u8);
        let df2 = df.roll(Roll::Forward);
        assert_eq!(df2.id, TOP as u8);
        let df3 = df2.roll(Roll::Forward);
        assert_eq!(df3.id, NEAR as u8);
        let d2 = df3.roll(Roll::Back).roll(Roll::Back).roll(Roll::Back);
        assert_eq!(d2.id, BOTTOM as u8);
        let dlb = d.roll(Roll::Left).roll(Roll::Back);
        assert_eq!(dlb.id, NEAR as u8);
    }

    #[test]
    fn test() {
        use crate::solve;
        let mut buf_in = r#"e2 e3 0 8 1 2 1 1"#.as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"5 e2 d2 d1 e1 e2 e3"#.trim());
    }
}
