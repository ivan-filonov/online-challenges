use std::{
    collections::HashMap,
    io::{self, BufRead, BufReader, BufWriter, Read, Write},
};

fn c2b(c: u8) -> bool {
    c == b'-'
}

fn b2c(b: bool) -> &'static str {
    if b {
        "-"
    } else {
        "_"
    }
}

fn solve(reader: &mut dyn Read, writer: &mut dyn Write) {
    let mut reader = BufReader::new(reader);
    let mut writer = BufWriter::new(writer);

    let mut line = String::new();
    reader.read_line(&mut line);
    let num_inputs: i32 = line.trim_end().parse().unwrap();
    line.clear();
    reader.read_line(&mut line);
    let num_outputs: i32 = line.trim_end().parse().unwrap();

    let mut inputs: HashMap<String, String> = HashMap::new();

    for _ in 0..num_inputs {
        let mut line = String::new();
        reader.read_line(&mut line);
        let mut parts = line.trim().split(" ");
        let name = parts.next().unwrap();
        let signal = parts.next().unwrap();
        inputs.insert(name.to_string(), signal.to_string());
    }

    for _ in 0..num_outputs {
        let mut line = String::new();
        reader.read_line(&mut line);
        let mut parts = line.trim().split(" ");
        let name = parts.next().unwrap().to_string();
        write!(&mut writer, "{} ", name);
        let op = parts.next().unwrap().to_string();
        let arg1_name = parts.next().unwrap().to_string();
        let arg2_name = parts.next().unwrap().to_string();
        let arg1 = inputs.get(&arg1_name).unwrap();
        let arg2 = inputs.get(&arg2_name).unwrap();
        assert_eq!(arg1.len(), arg2.len());
        for i in 0..arg1.len() {
            let a = c2b(arg1.as_bytes()[i]);
            let b = c2b(arg2.as_bytes()[i]);
            let res = match op.as_str() {
                "AND" => a & b,
                "OR" => a | b,
                "XOR" => a ^ b,
                "NAND" => !(a & b),
                "NOR" => !(a | b),
                "NXOR" => !(a ^ b),
                &_ => todo!(),
            };
            write!(&mut writer, "{}", b2c(res));
        }
        writeln!(&mut writer);
    }
}

fn main() {
    let mut streams = (io::stdin(), io::stdout());

    solve(&mut streams.0, &mut streams.1);
}

mod test {
    #[test]
    fn test1() {
        let mut t_in = r#"2
3
A __---___---___---___---___
B ____---___---___---___---_
C AND A B
D OR A B
E XOR A B"#
            .as_bytes();
        let t_out = r#"C ____-_____-_____-_____-___
D __-----_-----_-----_-----_
E __--_--_--_--_--_--_--_--_
"#;
        let mut out = Vec::new();
        crate::solve(&mut t_in, &mut out);
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out.trim());
    }
}
