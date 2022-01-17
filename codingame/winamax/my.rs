use std::{io, iter};
use std::collections::{HashSet, HashMap};
use std::iter::FromIterator;
use std::ops::RangeFull;

use paths_builder::Path;

macro_rules! parse_input {
    ($x:expr, $t:ident) => ($x.trim().parse::<$t>().unwrap())
}

#[derive(Debug, Hash, PartialEq, Eq, Clone, Copy, PartialOrd, Ord)]
pub struct Pt {
    x: i32,
    y: i32,
}

mod paths_builder {
    pub use crate::paths_builder_impl::build_paths;
    use crate::*;

    #[derive(Debug, Clone)]
    pub struct Path {
        pub moves: Vec<(i8, i8)>, // (dx, dy)
        pub nowater: Vec<Pt>,     // (x, y) sorted
        pub noobstacles: Vec<Pt>, // (x, y) sorted
        pub xmin: i32,
        pub xmax: i32,
        pub ymin: i32,
        pub ymax: i32,
    }

    #[derive(Debug)]
    pub struct Paths {
        pub paths: HashMap<usize, HashMap<(i32, i32), Vec<Path>>>,
    }
}

struct BallPaths {
    count: usize,
    ball: usize,
    paths: Vec<(paths_builder::Path,usize)>
}

struct Main {
    width : usize,
    height: usize,
    field : Vec<u8>,
    holes : Vec<Pt>,
    balls : Vec<(Pt, u8)>,
    water : HashSet<Pt>,
    obstacles : HashSet<Pt>,
    ball_paths : Vec<BallPaths>,
}

impl Main {
    fn new(width: i32, height: i32) -> Self {
        eprintln!("field size: {} x {}", width, height);
        Self {
            width: width as usize,
            height: height as usize,
            field: vec![0u8; (width * height) as usize],
            balls: Vec::new(),
            holes: Vec::new(),
            water: HashSet::new(),
            obstacles: HashSet::new(),
            ball_paths: Vec::new(),
        }
    }

    fn set_row(&mut self, row : usize, data : &[u8]) {
        assert!(row < self.height);
        assert!(data.len() == self.width);
        let base = self.width * row;
        for (col, &c) in data.iter().enumerate() {
            self.field[base + col] = match c {
                b'.' => 1,
                b'X' => {//water
                    let coords = Pt{ x: col as i32, y: row as i32};
                    self.water.insert(coords);
                    0
                },
                b'H' => {//hole
                    let coords = Pt{ x: col as i32, y: row as i32};
                    self.holes.push(coords);
                    self.obstacles.insert(coords);
                    0
                }
                b'0'..=b'9' => {//ball
                    let coords = Pt{ x: col as i32, y: row as i32};
                    self.balls.push((coords, c - b'0'));
                    0
                }
                _=>panic!()
            }
        }
    }

    fn build_paths_for_brute_force(&mut self) -> u32 {
        let max_ball = *self.balls.iter().map(|(_,sc)|sc).max().unwrap();
        let paths = paths_builder::build_paths(max_ball as usize);

        let mut ball_paths : Vec<BallPaths> = Vec::new();
        for (ball_idx, ball) in self.balls.iter().enumerate() {
            let mut v = BallPaths{ count: 0, ball: ball_idx, paths: Vec::new() };
            let paths_by_shot_count = &paths.paths[&(ball.1 as usize)];
            for (hole_id, hole) in self.holes.iter().enumerate() {
                let dx = hole.x - ball.0.x;
                let dy = hole.y - ball.0.y;
                if let Some(paths_to_hole) = paths_by_shot_count.get(&(dx, dy)) {
                    for path in paths_to_hole.iter() {
                        if let Some(p) = path.from_point(ball.0.x, ball.0.y, self.width, self.height) {
                            if p.noobstacles.iter().any(|pt|self.obstacles.contains(&pt)) {
                                continue;
                            }
                            if p.nowater.iter().any(|pt|self.water.contains(&pt)) {
                                continue;
                            }
                            v.paths.push((p, hole_id));
                            v.count += 1;
                        }
                    }
                }
            }
            ball_paths.push(v);
        }
        ball_paths.sort_unstable_by_key(|bp|bp.count);
        let bc = ball_paths.iter().fold(1u32, |acc, bp|acc * (bp.count as u32));
        self.ball_paths = ball_paths;
        bc
    }

    fn r (&self, used_points : &HashSet<Pt>, used_holes: &HashSet<usize>, pos : usize) -> Option<Vec<(usize, Path)>> {
        let is_leaf = pos + 1 == self.balls.len();
        let paths = &self.ball_paths[pos];
        'outer: for (path, hole) in paths.paths.iter() {
            if used_holes.contains(&hole) {
                continue;
            }
            for pt in path.noobstacles.iter().chain(path.nowater.iter()) {
                if used_points.contains(pt) {
                    continue 'outer;
                }
            }
            if is_leaf {
                let mut s : Vec<(usize, Path)> = Vec::new();
                s.push((paths.ball, path.clone()));
                return Some(s);
            }

            let mut uh = used_holes.clone();
            uh.insert(*hole);
            let mut up = used_points.clone();
            for &pt in path.noobstacles.iter().chain(path.nowater.iter()) {
                up.insert(pt);
            }
            if let Some(mut s) = self.r(&up, &uh, pos + 1) {
                s.push((paths.ball, path.clone()));
                return Some(s);
            }
        }
        None
    }

    fn solve(&self, brute_force_k: u32) -> Vec<(usize, Path)> {
        if brute_force_k == 1
        {
            self.ball_paths.iter().map(|bp| (bp.ball, bp.paths[0].0.clone())).collect()
        } else {
            self.r(&HashSet::new(), &HashSet::new(), 0).unwrap()
        }
    }

    fn run(&mut self) {
        let bc = self.build_paths_for_brute_force();
        eprintln!("balls: {}, combinations to brute force: {}", self.balls.len(), bc);
        let solution = self.solve(bc);
        // finally render paths:
        let mut field = (0..self.height).map(|_|vec!['.'; self.width]).collect::<Vec<_>>();

        for (ball, path) in solution.into_iter() {
            let (mut p, mut d) = self.balls[ball];
            for (mdx, mdy) in path.moves {
                let c = if mdx < 0 {'<'} else if mdx > 0 {'>'} else if mdy < 0 {'^'} else {'v'};
                for _ in 0..d {
                    field[p.y as usize][p.x as usize] = c;
                    p.x += mdx as i32;
                    p.y += mdy as i32;
                }
                d -= 1;
            }
        }
        for row in field {
            println!("{}", row.into_iter().collect::<String>());
        }
    }
}

fn main() {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let inputs = input_line.split(" ").collect::<Vec<_>>();
    let width = parse_input!(inputs[0], i32);
    let height = parse_input!(inputs[1], i32);
    let mut main = Main::new(width, height);
    for i in 0..height as usize {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let row = input_line.trim();//.to_string();
        main.set_row(i, row.as_bytes());
    }
    //eprintln!("field size {} x {}, {} balls, {} holes, {} shots max", main.width, main.height, main.balls.len(), main.holes.len(),
    //    main.balls.iter().map(|(_,_,n)| *n as u32).sum::<u32>());
    main.run();
}

mod paths_builder_impl {
    use crate::paths_builder::*;
    use crate::*;

    impl paths_builder::Path {
        pub fn from_point(
            &self,
            x: i32,
            y: i32,
            field_width: usize,
            field_height: usize,
        ) -> Option<Path> {
            if x + self.xmin < 0
                || x + self.xmax >= (field_width as i32)
                || y + self.ymin < 0
                || y + self.ymax >= (field_height as i32)
            {
                return None;
            }
            Some(Path {
                moves: self.moves.clone(),
                nowater: self
                    .nowater
                    .iter()
                    .map(|&pt| Pt {
                        x: pt.x + x,
                        y: pt.y + y,
                    })
                    .collect(),
                noobstacles: self
                    .noobstacles
                    .iter()
                    .map(|&pt| Pt {
                        x: pt.x + x,
                        y: pt.y + y,
                    })
                    .collect(),
                xmin: self.xmin + x,
                xmax: self.xmax + x,
                ymin: self.ymin + y,
                ymax: self.ymax + y,
            })
        }
    }

    pub fn build_paths(max_dist: usize) -> Paths {
        let mut state = State {
            paths: HashMap::new(),
        };
        for dist in 1..=max_dist {
            ff(&mut state, &Context::new(), dist);
        }

        let mut paths: HashMap<usize, HashMap<(i32, i32), Vec<Path>>> = HashMap::new();
        for (coords, contexts) in state.paths.into_iter() {
            for ctx in contexts.into_iter() {
                let k0 = ctx.moves[0].1;
                paths
                    .entry(k0)
                    .or_insert(HashMap::new())
                    .entry(coords)
                    .or_insert(Vec::new())
                    .push(context_to_path(ctx));
            }
        }
        Paths { paths }
    }

    fn context_to_path(ctx: Context) -> Path {
        let moves = ctx
            .moves
            .into_iter()
            .map(|(dir, _)| (DIRS[dir].0 as i8, DIRS[dir].1 as i8))
            .collect();
        let cx = ctx.x;
        let cy = ctx.y;
        let mut nowater: Vec<Pt> = ctx
            .nowater
            .into_iter()
            .filter_map(|pt| {
                if pt.x == cx && pt.y == cy {
                    None
                } else {
                    Some(pt)
                }
            })
            .collect();
        nowater.sort_unstable();
        let mut noobstacles: Vec<Pt> = ctx
            .used
            .difference(&HashSet::from_iter(nowater.iter().cloned()))
            .map(|&x| x)
            .filter(|&pt| (pt.x != 0 || pt.y != 0) && (pt.x != cx || pt.y != cy))
            .collect();
        let mut xmin = ctx.x;
        let mut xmax = ctx.x;
        let mut ymin = ctx.y;
        let mut ymax = ctx.y;
        for &pt in nowater.iter().chain(noobstacles.iter()) {
            xmin = xmin.min(pt.x);
            xmax = xmax.max(pt.x);
            ymin = ymin.min(pt.y);
            ymax = ymax.max(pt.y);
        }
        noobstacles.sort_unstable();
        Path {
            moves,
            nowater,
            noobstacles,
            xmin,
            xmax,
            ymin,
            ymax,
        }
    }

    fn ff(state: &mut State, ctx: &Context, dist: usize) {
        let c = (ctx.x, ctx.y);
        if ctx.x != 0 || ctx.y != 0 {
            state.paths.entry(c).or_insert(Vec::new()).push(ctx.clone());
        }
        if dist == 0 {
            return;
        }
        for d in 0..4 {
            if let Some(c) = ctx.apply_move(d, dist) {
                ff(state, &c, dist - 1);
            }
        }
    }

    struct State {
        paths: HashMap<(i32, i32), Vec<Context>>,
    }

    #[derive(Clone)]
    struct Context {
        x: i32,
        y: i32,
        used: HashSet<Pt>,          // (x, y)
        moves: Vec<(usize, usize)>, // (dir, dist)
        nowater: Vec<Pt>,           // (x, y)
    }

    impl Context {
        fn new() -> Self {
            let mut used = HashSet::new();
            used.insert(Pt { x: 0, y: 0 });
            Self {
                x: 0,
                y: 0,
                used,
                moves: Vec::new(),
                nowater: Vec::new(),
            }
        }

        fn apply_move(&self, dir: usize, dist: usize) -> Option<Self> {
            let mut x = self.x;
            let mut y = self.y;
            let dx = DIRS[dir].0;
            let dy = DIRS[dir].1;
            let mut used = self.used.clone();
            if dist == 0 {
                return None;
            }
            for _ in 0..dist {
                x += dx;
                y += dy;
                if used.contains(&Pt { x, y }) {
                    return None;
                }
                used.insert(Pt { x, y });
            }
            let mut moves = self.moves.clone();
            moves.push((dir, dist));
            let mut nowater = self.nowater.clone();
            nowater.push(Pt { x, y });
            Some(Self {
                x,
                y,
                used,
                moves,
                nowater,
            })
        }
    }

    const DIRS: &[(i32, i32)] = &[(-1, 0), (1, 0), (0, 1), (0, -1)];
}
