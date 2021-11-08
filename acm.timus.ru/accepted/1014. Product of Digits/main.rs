//use std::io::prelude::*;
use std::io::{BufRead, BufReader, BufWriter, Read, Write};

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let mut reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
    let mut buf = String::new();
    reader.read_line(&mut buf).expect("ok");
    let mut n: i32 = buf.trim().parse().expect("number");
    if n == 0 {
        writeln!(writer, "10");
        return;
    } else if n < 10 {
        writeln!(writer, "{}", n);
        return;
    }
    let mut res = String::new();
    for i in (2..10).rev() {
        while n % i == 0 {
            n /= i;
            res.insert(0, ('0' as u8 as i32 + i) as u8 as char);
        }
    }
    if n == 1 {
        writeln!(writer, "{}", res);
    } else {
        writeln!(writer, "-1");
    }
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn test() {
        use crate::solve;
        let mut buf_in = r#"10"#.as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"25"#.trim());
    }

    #[test]
    fn test2() {
        use crate::solve;
        let mut buf_in = r#"11"#.as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"-1"#.trim());
    }
}
