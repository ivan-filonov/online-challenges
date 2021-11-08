//use std::io::prelude::*;
use std::io::{BufRead, BufReader, BufWriter, Read, Write};

fn read1(reader: &mut dyn BufRead, pixels: i32) -> [[u8; 12]; 12] {
    let mut data = [[0; 12]; 12];
    for _ in 0..pixels {
        let mut s = String::new();
        reader.read_line(&mut s).expect("ok");
        let v: Vec<usize> = s
            .split(char::is_whitespace)
            .filter(|s| s.len() > 0)
            .map(|s| s.parse::<usize>().unwrap())
            .collect();
        assert_eq!(v.len(), 2);
        let (x, y) = (v[0], v[1]);
        data[y][x] = 1;
    }
    data
}

fn encode1(data: [[u8; 12]; 12], writer: &mut dyn Write) {
    let mut count = 0;
    for x in 1..11 {
        for y in 1..11 {
            if data[y][x] == 1 {
                count += 1;
            }
        }
    }
    writeln!(writer, "{}", count);
    for x in 1..11 {
        for y in 1..11 {
            if data[y][x] == 1 {
                writeln!(writer, "{} {}", x, y);
            }
        }
    }
}

fn read2(reader: &mut dyn BufRead, header: String) -> [[u8; 12]; 12] {
    let mut data = [[0; 12]; 12];
    let v: Vec<usize> = header
        .split(char::is_whitespace)
        .filter(|s| s.len() > 0)
        .map(|s| s.parse::<usize>().unwrap())
        .collect();
    assert_eq!(v.len(), 2);
    let mut x: Vec<usize> = Vec::new();
    let mut y: Vec<usize> = Vec::new();
    let mut i: usize = 0;
    x.push(v[0]);
    y.push(v[1]);

    for line in reader.lines().map(|s| s.unwrap()) {
        if line.contains(".") {
            break;
        }
        if line.contains("R") {
            x.push(x[i] + 1);
            y.push(y[i] + 0);
        }
        if line.contains("T") {
            x.push(x[i] + 0);
            y.push(y[i] + 1);
        }
        if line.contains("L") {
            x.push(x[i] - 1);
            y.push(y[i] + 0);
        }
        if line.contains("B") {
            x.push(x[i] + 0);
            y.push(y[i] - 1);
        }
        i += 1;
    }

    assert_eq!(x.len(), y.len());
    for (&yy, &xx) in y.iter().zip(x.iter()) {
        data[yy][xx] = 1;
    }

    data
}

fn encode2(data: [[u8; 12]; 12], writer: &mut dyn Write) {
    let mut pt: Vec<(usize, usize)> = Vec::new();
    let mut data = data;
    'outer: for x in 1..11 {
        for y in 1..11 {
            if data[y][x] == 1 {
                pt.push((x, y));
                data[y][x] = 0;
                break 'outer;
            }
        }
    }

    let (x0, y0) = pt.first().unwrap();
    writeln!(writer, "{} {}", x0, y0);

    let mut i: usize = 0;
    while i != pt.len() {
        let &(x, y) = pt.get(i).unwrap();
        if data[y][x + 1] == 1 {
            data[y][x + 1] = 0;
            pt.push((x + 1, y));
            write!(writer, "R");
        }
        if data[y + 1][x] == 1 {
            data[y + 1][x] = 0;
            pt.push((x, y + 1));
            write!(writer, "T");
        }
        if data[y][x - 1] == 1 {
            data[y][x - 1] = 0;
            pt.push((x - 1, y));
            write!(writer, "L");
        }
        if data[y - 1][x] == 1 {
            data[y - 1][x] = 0;
            pt.push((x, y - 1));
            write!(writer, "B");
        }
        writeln!(writer, "{}", if i == pt.len() - 1 { "." } else { "," });
        i += 1;
    }
}

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let mut reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);

    let mut header_line = String::new();
    reader.read_line(&mut header_line).expect("ok");
    let parsed_header = header_line.trim().parse::<i32>();
    if parsed_header.is_ok() {
        encode2(read1(&mut reader, parsed_header.unwrap()), &mut writer);
    } else {
        encode1(read2(&mut reader, header_line), &mut writer);
    }
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn test1() {
        use crate::solve;
        let mut buf_in = r#"6
2 3
2 4
3 3
3 4
4 2
4 3"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(
            output.trim(),
            r#"2 3
RT,
RT,
,
B,
,
."#
            .trim()
        );
    }

    #[test]
    fn test2() {
        use crate::solve;
        let mut buf_in = r#"2 3
RT,
RT,
,
B,
,
."#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(
            output.trim(),
            r#"6
2 3
2 4
3 3
3 4
4 2
4 3"#
                .trim()
        );
    }
}
