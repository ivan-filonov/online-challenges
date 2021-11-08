use std::io::prelude::*;
use std::io::{BufRead, BufReader, BufWriter, Read, Write};

const V: char = '│';
const H: char = '─';
const TL: char = '┌';
const TR: char = '┐';
const BL: char = '└';
const BR: char = '┘';
const ANY: char = '?';

struct Field {
    data: [[char; 50]; 20],
    fragments: i32,
}

impl Field {
    fn new() -> Self {
        Field {
            data: [['.'; 50]; 20],
            fragments: 0,
        }
    }

    fn read(&mut self, reader: &mut dyn BufRead) {
        let mut line = String::new();
        let mut data = [['.'; 50]; 20];
        let mut fragments: i32 = 0;
        for row in 0..20 {
            if !reader.read_line(&mut line).is_ok() {
                return;
            }
            for (col, c) in (0..50).zip(line.chars()) {
                match c {
                    H | V | TL | TR | BL | BR => fragments += 1,
                    _ => {}
                }
                data[row][col] = c;
            }
            line.clear();
        }
        self.data = data;
        self.fragments = fragments;
    }

    fn print(&self) {
        for row in self.data.iter() {
            for c in row.iter() {
                print!("{}", c);
            }
            println!();
        }
    }

    fn get(&self, y: usize, x: usize) -> char {
        self.data[y][x]
    }

    fn set(&mut self, y: usize, x: usize, c: char) {
        self.data[y][x] = c;
    }
}

struct Square {
    top: i32,
    left: i32,
    side: i32,
    points: Vec<(usize, usize, char)>,
}

impl Square {
    fn new(top: i32, left: i32, side: i32) -> Self {
        let mut points: Vec<(usize, usize, char)> = Vec::with_capacity(side as usize * 4 - 4);
        let right = (left + side - 1) as usize;
        let bottom = (top + side - 1) as usize;
        points.push((top as usize, left as usize, TL));
        for x in (left + 1) as usize..right {
            points.push((top as usize, x, H));
        }
        points.push((top as usize, right, TR));
        for y in (top + 1) as usize..bottom {
            points.push((y, left as usize, V));
            points.push((y, right, V));
        }
        points.push((bottom, left as usize, BL));
        for x in (left + 1) as usize..right {
            points.push((bottom, x, H));
        }
        points.push((bottom as usize, right as usize, BR));
        Square {
            top: top,
            left: left,
            side: side,
            points: points,
        }
    }

    fn check(&self, field: &Field) -> (bool, u32) {
        let mut mismatches = 0u32;
        let mut num_any = 0u32;

        for &(y, x, expected_char) in &self.points {
            let c = field.get(y, x);
            if c == ANY {
                num_any += 1
            } else if c == '.' {
                return (false, 0);
            } else if c != expected_char {
                mismatches += 1;
            }
        }

        if num_any == 4 * self.side as u32 - 4 {
            (false, 0)
        } else {
            (true, mismatches)
        }
    }

    fn mark_any(&self, field: &mut Field) {
        for &(y, x, _) in &self.points {
            let c = field.get(y, x);
            if c != ANY {
                assert_ne!(field.fragments, 0);
                field.fragments -= 1;
            }
            field.set(y, x, ANY);
        }
    }

    fn render(&self, field: &mut Field) {
        for &(y, x, c) in &self.points {
            field.set(y, x, c);
        }
    }
}

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let mut reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
    let mut field = Field::new();
    field.read(&mut reader);

    let mut squares: Vec<(Square, u32)> = Vec::new();
    for top in 0..19 {
        for left in 0..49 {
            if field.get(top, left) == '.' {
                continue;
            }
            for side in 2..20 {
                if top + side > 20 || left + side > 50 {
                    break;
                }
                let square = Square::new(top as i32, left as i32, side as i32);
                let (ok, mismatches) = square.check(&field);
                if ok {
                    squares.push((square, mismatches));
                }
            }
        }
    }
    // сначала самые большие квадраты
    squares.reverse();

    let mut answer: Vec<(i32, i32, i32)> = Vec::new();
    while squares.len() > 0 {
        squares.sort_by_key(|t| -t.0.side);
        let mut temp: Vec<Square> = Vec::new();
        let mut added = false;
        for (square, mismatches) in squares.into_iter() {
            if mismatches == 0 && !added {
                added = true;
                answer.push((square.top, square.left, square.side));
                square.mark_any(&mut field);
            } else {
                temp.push(square);
            }
        }
        if field.fragments < 1 {
            break;
        }
        squares = Vec::new();
        for square in temp.into_iter() {
            let (ok, mismatches) = square.check(&field);
            if ok {
                squares.push((square, mismatches));
            }
        }
    }

    answer.reverse();
    writeln!(writer, "{}", answer.len());
    for (y, x, s) in answer {
        writeln!(writer, "{} {} {}", x, y, s);
    }
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn basic_test() {
        use crate::solve;
        let mut buf_in = r#"..................................................
..................................................
..................................................
..................................................
..................................................
..................................................
...........┌─────┐................................
...........│.....│................................
....┌──────│.....│..........┌─┐...................
....│......│.....│..........│.│...................
....│......│.....│..........└─┘...................
....│......│....┌│────┐.............┌─┐...........
....│......└─────┘....│.............│.│...........
....│......│....│.....│.............└─┘...........
....│......│....│.....│.........┌──┐..............
....└──────┘....│.....│.........│..│..............
................│.....│.........│..│..............
................└─────┘.........└──┘..............
..................................................
..................................................
"#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(
            output,
            r#"6
16 11 7
32 14 4
4 8 8
11 6 7
36 11 3
28 8 3
"#
        );
    }
}
