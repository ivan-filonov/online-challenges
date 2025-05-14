use std::{
    cell::RefCell,
    collections::{HashMap, HashSet, VecDeque},
    rc::Rc,
};

#[rustfmt::skip]
mod unformatted {
/*
types:
0    1    2    3    4    5    6    7    8    9    10   11   12   13
XXX  X|X  XXX  X|X  X/X  X\X  X|X  X|X  XXX  X|X  X/X  X\X  XXX  XXX
XXX  -+-  ---  X|X  /X/  \X\  ---  X|-  -+-  -+X  /XX  XX\  XX/  \XX
XXX  X|X  XXX  X|X  X/X  X\X  XXX  X|X  X|X  X|X  XXX  XXX  X/X  X\X
rotation groups:
0, 1, (2, 3), (4, 5), (6, 7, 8, 9), (10, 11, 12, 13)
*/}
fn num_rotations(tile: i32) -> i32 {
    match tile {
        0..=1 => 1,
        2..=5 => 2,
        6..=13 => 4,
        _ => 1,
    }
}
fn rotate_left(tile: i32, amount: i32) -> i32 {
    match tile {
        2 | 3 => 2 + (tile - 2 + 2 - amount % 2) % 2,
        4 | 5 => 4 + (tile - 4 + 2 - amount % 2) % 2,
        6..=9 => 6 + (tile - 6 + 4 - amount % 4) % 4,
        10..=13 => 10 + (tile - 10 + 4 - amount % 4) % 4,
        _ => tile,
    }
}
fn rotate_right(tile: i32, amount: i32) -> i32 {
    match tile {
        2 | 3 => 2 + (tile - 2 + amount % 2) % 2,
        4 | 5 => 4 + (tile - 4 + amount % 2) % 2,
        6..=9 => 6 + (tile - 6 + amount % 4) % 4,
        10..=13 => 10 + (tile - 10 + amount % 4) % 4,
        _ => tile,
    }
}
#[derive(Debug, Copy, Clone, PartialEq, Eq, Hash)]
enum Dir {
    TOP,
    LEFT,
    RIGHT,
}
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
struct Pos {
    dir: Dir,
    x: u32,
    y: u32,
}
struct Maze {
    width: u32,
    height: u32,
    maze: Vec<i32>,
}
impl Maze {
    fn indexxy(&self, x: u32, y: u32) -> usize {
        assert!(
            x < self.width && y < self.height,
            "coords x={} y={} out of bounds ({}, {})",
            x,
            y,
            self.width,
            self.height
        );
        (y * self.width + x) as usize
    }
    fn getxy(&self, x: u32, y: u32) -> i32 {
        self.maze[self.indexxy(x, y)]
    }
    fn setxy(&mut self, x: u32, y: u32, tile: i32) {
        let index = self.indexxy(x, y);
        self.maze[index] = tile;
    }
    fn get(&self, pos: Pos) -> i32 {
        self.getxy(pos.x, pos.y)
    }
    fn set(&mut self, pos: Pos, tile: i32) {
        self.setxy(pos.x, pos.y, tile);
    }
    fn read() -> Self {
        let mut maze = Vec::new();
        let mut line = String::new();
        std::io::stdin().read_line(&mut line).unwrap();
        let mut iter = line.trim().split_whitespace().map(|s| s.parse().unwrap());
        let width = iter.next().unwrap();
        let height = iter.next().unwrap();
        drop(iter);
        for _ in 0..height {
            line.clear();
            std::io::stdin().read_line(&mut line).unwrap();
            maze.extend(
                line.trim()
                    .split_whitespace()
                    .map(|s| s.parse::<i32>().unwrap()),
            );
        }
        Self {
            width,
            height,
            maze,
        }
    }
    fn clone(&self) -> Self {
        Self {
            width: self.width,
            height: self.height,
            maze: self.maze.clone(),
        }
    }
    fn apply(&mut self, cmd: Command) {
        match cmd {
            Command::LEFT(x, y) => {
                let index = (y * self.width + x) as usize;
                let tile = self.maze[index];
                let rotated = rotate_left(tile, 1);
                self.maze[index] = rotated;
            }
            Command::RIGHT(x, y) => {
                let index = (y * self.width + x) as usize;
                let tile = self.maze[index];
                let rotated = rotate_right(tile, 1);
                self.maze[index] = rotated;
            }
            Command::WAIT => {}
        }
    }
}
impl Pos {
    fn add(&self, dx: i32, dy: i32) -> Self {
        Self {
            dir: match (dx, dy) {
                (0, 1) => Dir::TOP,
                (1, 0) => Dir::LEFT,
                (-1, 0) => Dir::RIGHT,
                _ => panic!("invalid increment"),
            },
            x: (self.x as i32 + dx) as u32,
            y: (self.y as i32 + dy) as u32,
        }
    }
    fn move_over(&self, tile: i32) -> Option<Self> {
        match (tile.abs(), &self.dir) {
            (1, _) => Some(self.add(0, 1)),
            (2, Dir::LEFT) => Some(self.add(1, 0)),
            (2, Dir::RIGHT) => Some(self.add(-1, 0)),
            (3, Dir::TOP) => Some(self.add(0, 1)),
            (4, Dir::TOP) => Some(self.add(-1, 0)),
            (4, Dir::RIGHT) => Some(self.add(0, 1)),
            (5, Dir::TOP) => Some(self.add(1, 0)),
            (5, Dir::LEFT) => Some(self.add(0, 1)),
            (6, Dir::LEFT) => Some(self.add(1, 0)),
            (6, Dir::RIGHT) => Some(self.add(-1, 0)),
            (7, Dir::TOP) => Some(self.add(0, 1)),
            (7, Dir::RIGHT) => Some(self.add(0, 1)),
            (8, Dir::LEFT) => Some(self.add(0, 1)),
            (8, Dir::RIGHT) => Some(self.add(0, 1)),
            (9, Dir::TOP) => Some(self.add(0, 1)),
            (9, Dir::LEFT) => Some(self.add(0, 1)),
            (10, Dir::TOP) => Some(self.add(-1, 0)),
            (11, Dir::TOP) => Some(self.add(1, 0)),
            (12, Dir::RIGHT) => Some(self.add(0, 1)),
            (13, Dir::LEFT) => Some(self.add(0, 1)),
            (_, _) => None,
        }
    }
    fn read() -> Self {
        let mut line = String::new();
        std::io::stdin().read_line(&mut line).unwrap();
        let mut iter = line.trim().split_whitespace();
        let x = iter.next().unwrap().parse::<u32>().unwrap();
        let y = iter.next().unwrap().parse::<u32>().unwrap();
        let dir = match iter.next().unwrap() {
            "TOP" => Dir::TOP,
            "LEFT" => Dir::LEFT,
            "RIGHT" => Dir::RIGHT,
            _ => panic!(),
        };
        Self { dir, x, y }
    }
}
#[derive(Debug, Clone, Copy)]
enum Command {
    WAIT,
    LEFT(u32, u32),
    RIGHT(u32, u32),
}
impl Command {
    fn to_string(&self) -> String {
        match self {
            Self::WAIT => "WAIT".to_string(),
            Self::LEFT(x, y) => format!("{} {} LEFT", x, y),
            Self::RIGHT(x, y) => format!("{} {} RIGHT", x, y),
        }
    }
}
struct Solver {
    maze: Rc<RefCell<Maze>>,
    indie: Pos,
    rocks: Vec<Pos>,
    exit_x: u32,
    trail: HashSet<Pos>,
}
impl Solver {
    fn new(maze: &Rc<RefCell<Maze>>, indie: Pos, rocks: Vec<Pos>, exit_x: u32) -> Self {
        Self {
            maze: Rc::clone(maze),
            indie,
            rocks,
            exit_x,
            trail: HashSet::new(),
        }
    }
    fn cleanup_rocks(&self, rocks: &mut Vec<Pos>) {
        let mut map: HashMap<Pos, u32> = HashMap::new();
        for rock in rocks.iter() {
            if map.contains_key(rock) {
                map.insert(*rock, map[rock] + 1);
            } else {
                map.insert(*rock, 1);
            }
        }
        let mut index = 0;
        while index < rocks.len() {
            if map[&rocks[index]] > 1
                || rocks[index].y >= self.maze.borrow().height
                || self.trail.contains(&rocks[index])
            {
                rocks.remove(index);
            } else {
                index += 1;
            }
        }
    }
    fn dfs(
        &self,
        maze: Rc<RefCell<Maze>>,
        indie: Option<Pos>,
        rocks: Vec<Pos>,
        out: &mut Vec<Command>,
        count: &mut usize,
    ) -> bool {
        if indie == None {
            return false;
        }
        *count += 1;
        let indie = indie.unwrap();
        if indie.y == maze.borrow().height {
            if indie.x == self.exit_x {
                // eprintln!("success!");
            }
            return indie.x == self.exit_x;
        }
        // eprintln!("dfs from {:?}", indie);
        for rock in rocks.iter() {
            if indie == *rock {
                return false;
            }
            if indie.y == rock.y {
                if indie.x == rock.x + 1 && indie.dir == Dir::LEFT && rock.dir == Dir::RIGHT {
                    return false;
                }
                if indie.x == rock.x - 1 && indie.dir == Dir::RIGHT && rock.dir == Dir::LEFT {
                    return false;
                }
                if indie.x == rock.x {
                    match maze.borrow().get(indie) {
                        1 | 6 => return false,
                        _ => {}
                    }
                }
            }
        }
        let mut choices = Vec::new();
        if maze.borrow().get(indie) > 1 {
            choices.push(indie);
        }
        for rock in rocks.iter() {
            if maze.borrow().get(*rock) > 1 && !self.trail.contains(rock) {
                choices.push(*rock);
            }
        }
        if choices.is_empty() {
            let mut mrocks = rocks
                .iter()
                .filter_map(|rock| rock.move_over(maze.borrow().get(*rock)))
                .collect();
            self.cleanup_rocks(&mut mrocks);
            let indie = indie.move_over(maze.borrow().get(indie));
            self.dfs(maze, indie, mrocks, out, count)
        } else {
            let num_choices = choices
                .iter()
                .map(|pos| maze.borrow().get(*pos))
                .map(|tile| num_rotations(tile))
                .fold(1, |a, c| a * c);
            let mut choice_values: Vec<i32> = vec![0; choices.len()];
            for _ in 0..num_choices {
                // explore
                let maze2 = Rc::new(RefCell::new(maze.borrow().clone()));
                for i in 0..choices.len() {
                    // eprintln!("choice: {:?}", choices[i]);
                    let pos = choices[i];
                    let tile = maze.borrow().get(pos);
                    let rotated = rotate_right(tile, choice_values[i]);
                    maze2.borrow_mut().set(pos, rotated);
                    let mut commands = Vec::new();
                    match choice_values[i] {
                        1 => commands.push(Command::RIGHT(pos.x, pos.y)),
                        2 => {
                            commands.push(Command::RIGHT(pos.x, pos.y));
                            commands.push(Command::RIGHT(pos.x, pos.y))
                        }
                        3 => commands.push(Command::LEFT(pos.x, pos.y)),
                        _ => {}
                    }

                    let mut mrocks = rocks
                        .iter()
                        .filter_map(|rock| rock.move_over(maze2.borrow().get(*rock)))
                        .collect();
                    self.cleanup_rocks(&mut mrocks);
                    if self.dfs(
                        Rc::clone(&maze2),
                        indie.move_over(maze2.borrow().get(indie)),
                        mrocks,
                        out,
                        count,
                    ) {
                        if !commands.is_empty() {
                            out.clear();
                            out.extend(commands.iter());
                        }
                        return true;
                    }
                }
                // increment
                for i in 0..choice_values.len() {
                    let d = num_rotations(maze.borrow().get(choices[i]));
                    choice_values[i] = (choice_values[i] + 1) % d;
                    if choice_values[i] != 0 {
                        break;
                    }
                }
            }
            false
        }
    }
    fn solve(&mut self) -> Vec<Command> {
        let mut result = Vec::new();
        let mut count = 0;
        self.trail.insert(self.indie);
        if self.dfs(
            Rc::clone(&self.maze),
            Some(self.indie),
            self.rocks.clone(),
            &mut result,
            &mut count,
        ) {
            if !result.is_empty() {
                eprintln!("generated commands: {:?}", result);
            }
            dbg!(count);
            result
        } else {
            dbg!(count);
            Vec::new()
        }
    }
}
fn main() {
    let mut line = String::new();
    let maze = Rc::new(RefCell::new(Maze::read()));
    std::io::stdin().read_line(&mut line).unwrap();
    let exit_x = line.trim().parse::<u32>().unwrap();
    let mut commands = VecDeque::new();
    loop {
        let indie = Pos::read();
        line.clear();
        std::io::stdin().read_line(&mut line).unwrap();
        let num_rocks = line.trim().parse::<usize>().unwrap();
        let mut rocks = Vec::new();
        for _ in 0..num_rocks {
            let rock = Pos::read();
            rocks.push(rock);
        }
        if commands.is_empty() {
            let mut solver = Solver::new(&maze, indie, rocks.clone(), exit_x);
            commands.extend(solver.solve());
        }

        let mut cmd = commands.pop_front().unwrap_or(Command::WAIT);
        cmd = match cmd {
            Command::LEFT(x, y) | Command::RIGHT(x, y) => {
                if (indie.x == x && indie.y == y) || rocks.iter().any(|r| r.x == x && r.y == y) {
                    Command::WAIT
                } else {
                    maze.borrow_mut().apply(cmd);
                    cmd
                }
            }
            _ => cmd,
        };

        println!("{}", cmd.to_string());
    }
}
mod test {
    use crate::*;
}
