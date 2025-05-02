use std::io;

const DIRECTIONS: [(i32, i32); 4] = [(0, -1), (-1, 0), (1, 0), (0, 1)];

struct Maze {
    width: i32,
    height: i32,
    maze: Vec<Vec<u8>>,
}

impl Maze {
    fn new(width: i32, height: i32) -> Maze {
        let maze = vec![vec![b'.'; width as usize]; height as usize];
        Maze {
            width,
            height,
            maze,
        }
    }
    fn set(&mut self, x: i32, y: i32, c: u8) {
        self.maze[y as usize][x as usize] = c;
    }
    fn get(&self, x: i32, y: i32) -> u8 {
        self.maze[y as usize][x as usize]
    }
}

trait Environment {
    fn read(&mut self) -> Maze;
    fn write(&mut self, maze: Maze);
}

mod cg_env {
    use std::io::{BufRead, BufReader, BufWriter, Read, Write};

    use crate::{Environment, Maze};

    pub struct StreamReader<'a> {
        reader: BufReader<&'a mut dyn Read>,
        writer: BufWriter<&'a mut dyn Write>,
    }

    impl<'a> StreamReader<'a> {
        pub fn new(reader: &'a mut dyn Read, writer: &'a mut dyn Write) -> Self {
            Self {
                reader: BufReader::new(reader),
                writer: BufWriter::new(writer),
            }
        }
    }

    impl<'a> Environment for StreamReader<'a> {
        fn read(&mut self) -> Maze {
            let mut line = String::new();
            self.reader.read_line(&mut line).unwrap();
            let mut iter = line.trim().split(" ").map(|s| s.parse::<i32>().unwrap());
            let width = iter.next().unwrap();
            let height = iter.next().unwrap();
            let mut maze = Maze::new(width, height);
            for y in 0..height {
                line.clear();
                self.reader.read_line(&mut line).unwrap();
                for x in 0..width {
                    maze.set(x, y, line.as_bytes()[x as usize]);
                }
            }
            maze
        }
        fn write(&mut self, maze: Maze) {
            for line in maze.maze {
                for c in line {
                    write!(self.writer, "{}", c as char).unwrap();
                }
                writeln!(self.writer).unwrap();
            }
            self.writer.flush().unwrap();
        }
    }
}

trait Solver {
    fn solve(&mut self, maze: &mut Maze);
}

mod solver_main {
    use std::collections::VecDeque;

    use super::*;
    pub struct SolverImpl;
    pub fn create_solver() -> SolverImpl {
        SolverImpl
    }

    struct Pt {
        x: i32,
        y: i32,
        d: i32,
    }

    fn get_start(maze: &Maze) -> Pt {
        for y in 0..maze.height {
            for x in 0..maze.width {
                if maze.get(x, y) == b'S' {
                    return Pt { x, y, d: 0 };
                }
            }
        }
        panic!("no start point found");
    }

    impl Solver for SolverImpl {
        fn solve(&mut self, maze: &mut Maze) {
            let mut queue = VecDeque::new();
            let mut visited = vec![vec![false; maze.width as usize]; maze.height as usize];

            queue.push_back(get_start(maze));
            while !queue.is_empty() {
                let pt = queue.pop_front().unwrap();
                if visited[pt.y as usize][pt.x as usize] {
                    continue;
                }
                visited[pt.y as usize][pt.x as usize] = true;
                maze.set(
                    pt.x,
                    pt.y,
                    if pt.d < 10 {
                        b'0' + pt.d as u8
                    } else {
                        b'A' + (pt.d - 10) as u8
                    },
                );
                for &(dx, dy) in DIRECTIONS.iter() {
                    let x = (pt.x + maze.width + dx) % maze.width;
                    let y = (pt.y + maze.height + dy) % maze.height;
                    if !visited[y as usize][x as usize] && maze.get(x, y) != b'#' {
                        queue.push_back(Pt { x, y, d: pt.d + 1 });
                    }
                }
            }
        }
    }
}

fn main() {
    let (mut r#in, mut out) = (io::stdin(), io::stdout());
    let mut env = cg_env::StreamReader::new(&mut r#in, &mut out);
    let mut solver = solver_main::create_solver();
    let mut maze = env.read();
    solver.solve(&mut maze);
    env.write(maze);
}

mod test {
    use super::*;

    fn make_test_maze(maze_data: &str) -> Maze {
        let lines: Vec<_> = maze_data.split("\n").collect();
        let width = lines[0].len();
        let height = lines.len();
        let mut maze = Maze::new(width as i32, height as i32);
        for (y, line) in lines.iter().enumerate() {
            for (x, c) in line.chars().enumerate() {
                maze.set(x as i32, y as i32, c as u8);
            }
        }

        maze
    }

    fn test(maze_data: &str, expected: &str) {
        let mut maze = make_test_maze(maze_data);
        let mut solver = solver_main::create_solver();
        solver.solve(&mut maze);
        let expected: Vec<&str> = expected.split("\n").collect();
        for y in 0..maze.height {
            for x in 0..maze.width {
                let maze_byte = maze.get(x, y);
                let expected_byte = expected[y as usize].as_bytes()[x as usize];
                assert_eq!(
                    maze_byte as char, expected_byte as char,
                    "x: {}, y: {}",
                    x, y
                );
            }
        }
    }

    #[test]
    fn test1() {
        test(
            r"##########
#S.......#
##.#####.#
##.#.....#
##########",
            r"##########
#01234567#
##2#####8#
##3#DCBA9#
##########",
        );
    }
}
