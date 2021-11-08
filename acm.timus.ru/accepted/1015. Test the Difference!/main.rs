//use std::io::prelude::*;
use std::collections::HashMap;
use std::io::{BufRead, BufReader, BufWriter, Read, Write};

// L R U F D B
type Dice = [u8; 6];

const L: usize = 0;
const R: usize = 1;
const U: usize = 2;
const F: usize = 3;
const D: usize = 4;
const B: usize = 5;

fn from_string(s: &str) -> Dice {
    let v: Vec<u8> = s
        .split(char::is_whitespace)
        .filter(|s| s.len() > 0)
        .map(|s| s.parse().unwrap())
        .collect();
    assert_eq!(v.len(), 6);
    [v[0], v[1], v[2], v[3], v[4], v[5]]
}

fn to_string(d: Dice) -> String {
    let mut s = String::new();
    for x in d.iter() {
        s.push_str(&i32::to_string(&(*x as i32)));
    }
    s
}

fn turn_F(d: Dice) -> Dice {
    [d[D], d[U], d[L], d[F], d[R], d[B]]
}

fn turn_L(d: Dice) -> Dice {
    [d[L], d[R], d[F], d[D], d[B], d[U]]
}

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let mut reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
    let mut buf = String::new();
    reader.read_line(&mut buf).expect("ok");
    let n: usize = buf.trim().parse().expect("correct number");

    let mut dices: Vec<Dice> = Vec::with_capacity(n);
    for _ in 0..n {
        buf.clear();
        reader.read_line(&mut buf).expect("ok");
        dices.push(from_string(&buf));
        //        println!("{:?}", to_string(dices.last().unwrap().clone()));
    }

    let mut dice_order: Vec<String> = Vec::new();
    let mut dice_to_list: HashMap<String, Vec<i32>> = HashMap::new();

    for (n, d) in dices.iter_mut().enumerate() {
        if d[F] != 1 {
            if d[L] == 1 || d[R] == 1 {
                *d = turn_F(*d);
            }
            while d[F] != 1 {
                *d = turn_L(*d);
            }
        }
        if d[B] != 2 {
            while d[U] != 2 {
                *d = turn_F(*d);
            }
        } else {
            while d[U] != 3 {
                *d = turn_F(*d);
            }
        }
        // println!("{:?}", to_string(*d));
        let key = to_string(*d);
        if !dice_to_list.contains_key(&key) {
            dice_to_list.insert(key.clone(), Vec::new());
            dice_order.push(key.clone());
        }
        dice_to_list.get_mut(&key).unwrap().push(n as i32 + 1);
    }

    writeln!(writer, "{}", dice_order.len());
    for k in dice_order.into_iter() {
        for (n, index) in dice_to_list.get(&k).unwrap().iter().enumerate() {
            if n > 0 {
                write!(writer, " ");
            }
            write!(writer, "{}", index);
        }
        writeln!(writer);
    }
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn test() {
        use crate::solve;
        let mut buf_in = r#"3
1 2 6 4 5 3
4 3 6 2 5 1
4 1 3 6 2 5"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(
            output.trim(),
            r#"2
1 2
3"#
            .trim()
        );
    }
}
