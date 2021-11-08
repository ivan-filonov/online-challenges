use std::io::prelude::*;
use std::io::{BufRead, BufReader, BufWriter, Read, Write};

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let mut reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
    let mut temp_line = String::new();
    reader.read_line(&mut temp_line).expect("input");
    let n = temp_line.trim().parse::<usize>().expect("N");
    for x in reader
        .lines()
        .map(|r| r.expect("word"))
        .filter(|s| s.trim().len() > 0)
    {
        let damaged = x
            .chars()
            .filter(|&c| c == '0' || c == '1')
            .collect::<String>();
        let mut fixed = damaged.to_string();

        let ones: Vec<usize> = damaged
            .chars()
            .enumerate()
            .filter(|&(_, c)| c == '1')
            .map(|(n, _)| n + 1)
            .collect();
        let num_ones = ones.len();
        let ones_pos_sum = ones.iter().fold(0, |acc, val| acc + val);
        let correction = ones_pos_sum % (n + 1);

        if damaged.len() == n && correction != 0 {
            for (pos, c) in damaged.chars().enumerate() {
                if c == '1' && (ones_pos_sum - pos - 1) % (n + 1) == 0 {
                    fixed.replace_range(pos..=pos, "0");
                    break;
                }
            }
        } else if damaged.len() > n {
            let mut ones_after = num_ones;
            let mut to_remove = 0;
            for (pos, c) in damaged.chars().enumerate() {
                if c == '0' {
                    if (ones_pos_sum - ones_after) % (n + 1) == 0 {
                        to_remove = pos;
                        break;
                    }
                } else {
                    if (ones_pos_sum - ones_after - pos) % (n + 1) == 0 {
                        to_remove = pos;
                        break;
                    }
                    ones_after -= 1;
                }
            }
            fixed.remove(to_remove);
        } else if damaged.len() < n {
            let mut pos_insert = damaged.len();
            let mut char_insert = '0';
            let mut ones_after = num_ones;
            let mut have_fix = false;
            for (pos, c) in damaged.chars().enumerate() {
                if (ones_pos_sum + ones_after) % (n + 1) == 0 {
                    pos_insert = pos;
                    have_fix = true;
                    break;
                }
                if (ones_pos_sum + ones_after + pos + 1) % (n + 1) == 0 {
                    pos_insert = pos;
                    char_insert = '1';
                    have_fix = true;
                    break;
                }
                if c == '1' {
                    ones_after -= 1;
                }
            }
            if !have_fix {
                if ones_pos_sum % (n + 1) == 0 {
                    // append 0
                } else {
                    char_insert = '1';
                }
            }
            fixed.insert(pos_insert, char_insert);
        }

        writeln!(writer, "{}", fixed).expect("ok");
    }
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn basic_test() {
        use crate::solve;
        let mut buf_in = r#"4
0 0 00

011
101 1
110 11"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(
            output.trim(),
            r#"0000
0110
1001
1111"#
                .trim()
        );
    }

    #[test]
    fn test3() {
        use crate::solve;
        let mut buf_in = r#"3
11
1 1
 1 1"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(
            output.trim(),
            r#"101
101
101"#
                .trim()
        );
    }

    #[test]
    fn test10() {
        use crate::solve;
        let mut buf_in = r#"10
101011010
100000000
100111100
101111110
001110000
111001000
001010110
000010100
"#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(
            output.trim(),
            r#"1010110101
1000000001
1001111001
1011111101
0011100001
1110010001
0010101101
0000101001
"#
            .trim()
        );
    }
}
