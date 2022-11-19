use std::{
    collections::HashSet,
    io::{self, BufRead, BufReader, BufWriter, Read, Write},
    iter::FromIterator,
};

macro_rules! parse_input {
    ($x:expr, $t:ident) => {
        $x.trim().parse::<$t>().unwrap()
    };
}

fn full_mask(size: usize) -> u8 {
    match size {
        1 => 0b10,
        2 => 0b110,
        3 => 0b1110,
        4 => 0b11110,
        5 => 0b111110,
        6 => 0b1111110,
        _ => panic!("size must be in [0..6]"),
    }
}

struct Solver {
    width: i32,
    height: i32,

    group_letters: Vec<u8>,
    initial_digits: Vec<u8>,

    group_cells: Vec<Vec<usize>>,
    neigbours: Vec<Vec<usize>>,

    digits: Vec<u8>,
    masks: Vec<u8>,

    search_called: u32,
    answer: Option<Vec<u8>>,
}

impl Solver {
    fn new() -> Self {
        Self {
            width: 0,
            height: 0,

            group_letters: Vec::new(),
            initial_digits: Vec::new(),

            group_cells: Vec::new(),
            neigbours: Vec::new(),

            digits: Vec::new(),
            masks: Vec::new(),

            search_called: 0,
            answer: None,
        }
    }

    fn init_board(&mut self, width: i32, height: i32) {
        self.width = width;
        self.height = height;
        let size = (width * height) as usize;
        self.group_letters.resize(size, 0);
        self.initial_digits.resize(size, 0);
        self.neigbours.resize_with(size, || Vec::new());
    }

    fn init_cell(&mut self, x: usize, y: usize, init: &[u8]) {
        assert_eq!(init.len(), 2);
        let offset = y * self.width as usize + x;
        self.group_letters[offset] = init[0];
        self.initial_digits[offset] = if init[1].is_ascii_digit() {
            init[1] - '0' as u8
        } else {
            0
        };
    }

    fn read(&mut self, reader: &mut dyn Read) {
        let mut reader = BufReader::new(reader);

        let mut input_line = String::new();
        reader.read_line(&mut input_line).unwrap();
        let inputs = input_line.split(" ").collect::<Vec<_>>();
        eprintln!("{}", input_line.trim_end());

        self.init_board(parse_input!(inputs[0], i32), parse_input!(inputs[1], i32));

        for y in 0..self.height as usize {
            let mut input_line = String::new();
            reader.read_line(&mut input_line).unwrap();
            eprintln!("{}", input_line.trim_end());

            let line: Vec<&str> = input_line.trim_matches('\n').split(" ").collect();

            assert_eq!(line.len(), self.width as usize);

            for x in 0..self.width as usize {
                self.init_cell(x, y, line[x].as_bytes());
            }
        }
    }

    fn mark_group(&mut self, x: i32, y: i32, marker: u8, id: usize, visited: &mut Vec<bool>) {
        if x < 0 || y < 0 || x >= self.width || y >= self.height {
            return;
        }
        let offset = (y * self.width + x) as usize;
        if visited[offset] || self.group_letters[offset] != marker {
            return;
        }

        visited[offset] = true;
        self.group_letters[offset] = 0;
        self.group_cells[id].push(offset);

        self.mark_group(x, y - 1, marker, id, visited);
        self.mark_group(x - 1, y, marker, id, visited);
        self.mark_group(x + 1, y, marker, id, visited);
        self.mark_group(x, y + 1, marker, id, visited);
    }

    fn mark_all_groups(&mut self) {
        let mut visited: Vec<bool> = Vec::new();
        visited.resize((self.width * self.height) as usize, false);
        let mut group_id = 0;
        for y in 0..self.height {
            for x in 0..self.width {
                let offset = (y * self.width + x) as usize;
                if self.group_letters[offset] == 0 {
                    continue;
                }
                visited.fill(false);
                self.group_cells.push(Vec::new());
                self.mark_group(x, y, self.group_letters[offset], group_id, &mut visited);
                for &cell in self.group_cells[group_id].iter() {
                    self.neigbours[cell].extend(self.group_cells[group_id].iter());
                }
                group_id += 1;
            }
        }
        eprintln!("marked {} groups", group_id);
    }

    fn mark_neigbours(&mut self) {
        for y in 0..self.height {
            for x in 0..self.width {
                let offset = (y * self.width + x) as usize;
                let mut values: HashSet<usize> =
                    HashSet::from_iter(self.neigbours[offset].iter().cloned());
                for dy in -1..=1 {
                    for dx in -1..=1 {
                        if dx == 0 && dy == 0 {
                            continue;
                        }
                        let (xx, yy) = (x + dx, y + dy);
                        if xx < 0 || yy < 0 || xx >= self.width || yy >= self.height {
                            continue;
                        }
                        values.insert((yy * self.width + xx) as usize);
                    }
                }
                self.neigbours[offset].clear();
                values.remove(&offset);
                self.neigbours[offset].clear();
                self.neigbours[offset].extend(values.into_iter());
            }
        }
    }

    fn prepare(&mut self) {
        let size = (self.width * self.height) as usize;
        self.masks.resize(size, 0);
        self.digits.resize(size, 0);
        for group in &self.group_cells {
            let group_mask_full = full_mask(group.len());
            for index in group {
                self.masks[*index] = group_mask_full;
            }
        }
        for index in 0..size {
            let initial_digit = self.initial_digits[index];
            if initial_digit == 0 {
                continue;
            }
            let neg_mask = !(1 << initial_digit);
            self.masks[index] = 0;
            for dst in &self.neigbours[index] {
                self.masks[*dst] &= neg_mask;
            }
            self.digits[index] = initial_digit;
        }
        if false {
            eprintln!("digits: {:?}", &self.digits);
            eprintln!("masks: {:?}", &self.masks);
        }
    }

    fn search(&mut self, prio: &mut [(u32, usize)], d: i32) {
        self.search_called += 1;
        if prio.len() == 0 {
            if false {
                eprintln!("[{}] prio is empty after {} calls", d, self.search_called);
            }
            self.answer = Some(self.digits.clone());
            return;
        }
        if self.answer != None {
            if false {
                eprintln!(
                    "[{}] answer already found after {} calls",
                    d, self.search_called
                );
            }
            return;
        }
        let (current_prio, current_index) = prio[0];
        if current_prio == 0 {
            if false {
                eprintln!(
                    "[{}] no choices for {} after {} calls",
                    d, current_index, self.search_called
                );
            }
            return;
        }
        let current_mask = self.masks[current_index];
        assert_eq!(self.digits[current_index], 0);
        let saved_masks: Vec<_> = self.neigbours[current_index]
            .iter()
            .map(|&index| self.masks[index])
            .collect();

        let choices: Vec<u8> = (1..=6)
            .filter(|&n| 0 != (current_mask & (1 << n)))
            .collect();
        if false {
            eprintln!("[{}] choices for {}: {:?}", d, current_index, &choices);
        }
        for value in choices {
            self.digits[current_index] = value;
            let neg_mask = !(1 << value);
            for (&i, &m) in self.neigbours[current_index].iter().zip(saved_masks.iter()) {
                self.masks[i] = m & neg_mask;
            }
            for i in 1..prio.len() {
                prio[i].0 = u8::count_ones(self.masks[prio[i].1]);
            }
            prio[1..].sort();
            if false {
                eprintln!("[{}] testing {} for index {}", d, value, current_index);
            }
            self.search(&mut prio[1..], d + 1);
        }

        for (&index, &saved_mask) in self.neigbours[current_index].iter().zip(saved_masks.iter()) {
            self.masks[index] = saved_mask;
        }
        self.digits[current_index] = 0;
    }

    fn solve(&mut self, reader: &mut dyn Read, writer: &mut dyn Write) {
        self.read(reader);
        self.mark_all_groups();
        self.mark_neigbours();
        self.prepare();

        let mut cell_priorities: Vec<(u32, usize)> = self
            .masks
            .iter()
            .enumerate()
            .map(|(index, mask)| (u8::count_ones(*mask), index))
            .collect();
        cell_priorities.sort();
        let num_locked = cell_priorities
            .iter()
            .filter(|(ones, _)| *ones == 0)
            .count();
        if false {
            eprintln!(
                "cell_priorities: {:?}, num_locked: {}, first unlocked: {:?}",
                &cell_priorities, num_locked, cell_priorities[num_locked]
            );
        }
        self.search(&mut cell_priorities[num_locked..], 0);
        if false {
            eprintln!(
                "answer: {:?}, search_called: {}",
                self.answer, self.search_called
            );
        }
        let mut writer = BufWriter::new(writer);
        if let Some(answer) = &self.answer {
            for y in 0..self.height {
                for x in 0..self.width {
                    let offset = (y * self.width + x) as usize;
                    write!(writer, "{}", answer[offset]);
                }
                writeln!(writer);
            }
        }
    }
}

fn main() {
    Solver::new().solve(&mut io::stdin(), &mut io::stdout())
}

mod tests {
    #[test]
    fn test1() {
        use crate::Solver;
        let mut input = r#"4 5
G. G4 G1 G.
R. G. B. B.
G. R. R4 B.
G. R. R2 B.
G3 G. R. B.
"#
        .as_bytes();
        let mut output: Vec<u8> = Vec::new();
        Solver::new().solve(&mut input, &mut output);
        let expected = r"2413
1525
2341
1523
3414"
            .trim();
        assert_eq!(expected, String::from_utf8(output).unwrap().trim());
    }
}
