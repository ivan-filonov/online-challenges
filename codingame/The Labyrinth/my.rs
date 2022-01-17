use std::{io, collections::VecDeque};

macro_rules! parse_input {
    ($x:expr, $t:ident) => ($x.trim().parse::<$t>().unwrap())
}

#[derive(Clone, Copy)]
struct Coords {
    row: i32,
    col: i32
}

impl Coords {
    fn new(index:usize, num_cols:i32) -> Self {
        Self {
            col: index as i32 % num_cols,
            row: index as i32 / num_cols
        }
    }

    fn index(&self, num_cols:i32)->usize {
        (self.row * num_cols + self.col) as usize
    }

    fn neighbours(&self, num_rows:i32,num_cols:i32) -> std::vec::IntoIter<Coords> {
        let mut v : Vec<Coords> = Vec::with_capacity(4);
        if self.row > 0 {
            v.push(Self{row:self.row - 1, col: self.col});
        }
        if self.col > 0 {
            v.push(Self{col:self.col - 1, row: self.row});
        }
        if self.row + 1 < num_rows {
            v.push(Self{row:self.row+1, col: self.col});
        }
        if self.col + 1 < num_cols {
            v.push(Self{col:self.col+1, row: self.row});
        }
        v.into_iter()
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
    let num_rows = parse_input!(inputs[0], i32); // number of rows.
    let num_cols = parse_input!(inputs[1], i32); // number of columns.
    let _a = parse_input!(inputs[2], i32); // number of rounds between the time the alarm countdown is activated and the time the alarm goes off.

    // game loop
    let mut escape = false;
    let mut explore = true;
    loop {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let inputs = input_line.split(" ").collect::<Vec<_>>();
        let kr = parse_input!(inputs[0], i32); // row where Rick is located.
        let kc = parse_input!(inputs[1], i32); // column where Rick is located.
        let current_pos = Coords { row: kr, col:kc };
        let current_index = current_pos.index(num_cols);
        let mut maze = vec![0u8; (num_rows*num_cols)as usize];

        let mut exit_point = Coords{row:-1, col:-1};
        let mut exit_seen = false;
        let mut control_point = Coords{row:-1, col:-1};
        let mut control_seen = false;
        for i in 0..num_rows as usize {
            let mut input_line = String::new();
            io::stdin().read_line(&mut input_line).unwrap();
            let row = input_line.trim().to_string(); // C of the characters in '#.TC?' (i.e. one line of the ASCII maze).
            for (j, c) in row.as_bytes().iter().enumerate() {
                match *c {
                    b'T' => {exit_point = Coords{row:i as i32,col:j as i32}; exit_seen = true; },
                    b'C' => {control_point = Coords{row:i as i32,col:j as i32}; control_seen = true; },
                    _=> {}
                }
            }
            maze[i*num_cols as usize..((i+1)*num_cols as usize)].copy_from_slice(row.as_bytes());
        }

        if maze[current_index] == b'C' {
            escape = true;
        }
        eprintln!("explore: {}, escape: {}", explore, escape);

        let mut nearest_question_index : Option<usize> = None;
        let mut bfs_bag = VecDeque::<Coords>::new();
        let mut bfs_from = vec![usize::MAX; maze.len()];
        let mut bfs_d = vec![u32::MAX; maze.len()];
        bfs_bag.push_back(current_pos);
        bfs_from[current_index] = current_index;
        bfs_d[current_index] = 0;
        while !bfs_bag.is_empty() {
            let from = bfs_bag.pop_front().unwrap();
            let from_index = from.index(num_cols);
            if maze[from_index] == b'?' {
                continue;
            }
            for to in from.neighbours(num_rows, num_cols) {
                let to_index = to.index(num_cols);
                if bfs_from[to_index] != usize::MAX {
                    continue;
                }
                if maze[to_index] == b'#' {
                    continue;
                }
                bfs_from[to_index] = from_index;
                bfs_d[to_index] = bfs_d[from_index] + 1;
                if maze[to_index] == b'?' {
                    let nearest_question_d = match nearest_question_index {
                        Some(index) => bfs_d[index],
                        None=>u32::MAX
                    };
                    if bfs_d[to_index] < nearest_question_d {
                        nearest_question_index = Some(to_index);
                    }
                }
                bfs_bag.push_back(to);
            }
        }

        if explore && exit_seen && control_seen &&
           bfs_d[control_point.index(num_cols)] != u32::MAX && bfs_d[exit_point.index(num_cols)] != u32::MAX {
                explore = false;
        }

        let destination_index = if explore {
            nearest_question_index.unwrap()
        } else {
            if escape {
                exit_point.index(num_cols)
            } else {
                control_point.index(num_cols)
            }
        };

        let mut move_index = destination_index;
        while bfs_from[move_index] != current_index {
            move_index = bfs_from[move_index];
        }
        let move_pos = Coords::new(move_index, num_cols);

        //eprintln!("move: ({}, {}) -> ({}, {})", current_pos.row, current_pos.col, move_pos.row, move_pos.col);
        if move_pos.row > current_pos.row {
            println!("DOWN");
        } else if move_pos.row < current_pos.row {
            println!("UP");
        } else
        if move_pos.col > current_pos.col {
            println!("RIGHT");
        } else {
            println!("LEFT");
        }
    }
}
