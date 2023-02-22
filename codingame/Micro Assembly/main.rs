use std::io::{self, BufReader, BufWriter, Read, Write};

const ECHO: bool = true;
const DEBUG: bool = true;

mod v1 {
    use std::io::{BufRead, BufReader, BufWriter, Read, Write};

    use crate::ECHO;

    enum RegImm {
        Reg(usize),
        Imm(i32),
    }

    enum Opcode {
        Mov(usize, RegImm),
        Add(usize, RegImm, RegImm),
        Sub(usize, RegImm, RegImm),
        Jne(usize, usize, RegImm),
    }

    pub struct State {
        ip: usize,
        regs: [i32; 4],
        opcodes: Vec<Opcode>,
    }

    impl State {
        pub fn new() -> Self {
            Self {
                ip: 0,
                regs: [0; 4],
                opcodes: Vec::new(),
            }
        }

        pub fn read(&mut self, reader: &mut BufReader<&mut dyn Read>) {
            let mut buf = String::new();
            reader.read_line(&mut buf).unwrap();
            if ECHO {
                eprintln!("{}", buf.trim());
            }
            let regs = buf
                .trim()
                .split(" ")
                .map(|s| s.parse().expect("number"))
                .collect::<Vec<i32>>();
            assert!(regs.len() == 4);
            for i in 0..4 {
                self.regs[i] = regs[i];
            }

            buf = String::new();
            reader.read_line(&mut buf).unwrap();
            if ECHO {
                eprintln!("{}", buf.trim());
            }
            let num_ops: i32 = buf.trim().parse().unwrap();
            for _ in 0..num_ops {
                buf = String::new();
                reader.read_line(&mut buf).unwrap();
                if ECHO {
                    eprintln!("{}", buf.trim());
                }
                let mut iter = buf.trim().split(" ");
                let opcode = match iter.next().unwrap() {
                    "MOV" => {
                        let dst = iter.next().unwrap();
                        let src = iter.next().unwrap();
                        Opcode::Mov(parse_reg(dst), parse_reg_imm(src))
                    }
                    "ADD" => {
                        let dst = iter.next().unwrap();
                        let arg1 = iter.next().unwrap();
                        let arg2 = iter.next().unwrap();
                        Opcode::Add(parse_reg(dst), parse_reg_imm(arg1), parse_reg_imm(arg2))
                    }
                    "SUB" => {
                        let dst = iter.next().unwrap();
                        let arg1 = iter.next().unwrap();
                        let arg2 = iter.next().unwrap();
                        Opcode::Sub(parse_reg(dst), parse_reg_imm(arg1), parse_reg_imm(arg2))
                    }
                    "JNE" => {
                        let addr = iter.next().unwrap();
                        let reg = iter.next().unwrap();
                        let reg_imm = iter.next().unwrap();
                        Opcode::Jne(
                            addr.parse().unwrap(),
                            parse_reg(reg),
                            parse_reg_imm(reg_imm),
                        )
                    }
                    _ => panic!(),
                };
                self.opcodes.push(opcode);
            }
        }

        pub fn write(&mut self, writer: &mut BufWriter<&mut dyn Write>) {
            for i in 0..4 {
                if i > 0 {
                    write!(writer, " ").unwrap();
                }
                write!(writer, "{}", self.regs[i]).unwrap();
            }
        }

        fn get(&self, r: &RegImm) -> i32 {
            match r {
                RegImm::Imm(v) => *v,
                RegImm::Reg(r) => self.regs[*r],
            }
        }

        pub fn solve(&mut self) {
            while self.ip < self.opcodes.len() {
                self.ip = match &self.opcodes[self.ip] {
                    Opcode::Mov(dst, src) => {
                        self.regs[*dst] = self.get(src);
                        self.ip + 1
                    }
                    Opcode::Add(dst, arg1, arg2) => {
                        self.regs[*dst] = self.get(arg1) + self.get(arg2);
                        self.ip + 1
                    }
                    Opcode::Sub(dst, arg1, arg2) => {
                        self.regs[*dst] = self.get(arg1) - self.get(arg2);
                        self.ip + 1
                    }
                    Opcode::Jne(addr, reg, reg_imm) => {
                        if self.regs[*reg] == self.get(reg_imm) {
                            self.ip + 1
                        } else {
                            *addr
                        }
                    }
                }
            }
        }
    }

    fn parse_reg(s: &str) -> usize {
        (s.as_bytes()[0] - b'a') as usize
    }

    fn parse_reg_imm(s: &str) -> RegImm {
        let c0 = s.as_bytes()[0];
        if c0 >= b'a' && c0 <= b'd' {
            RegImm::Reg((c0 - b'a') as usize)
        } else {
            RegImm::Imm(s.parse().unwrap())
        }
    }
}

fn solve(reader: &mut dyn Read, writer: &mut dyn Write) {
    let mut reader = BufReader::new(reader);
    let mut writer = BufWriter::new(writer);

    use v1::State;
    let state = &mut State::new();
    state.read(&mut reader);
    state.solve();
    state.write(&mut writer);
}

fn main() {
    let mut streams = (io::stdin(), io::stdout());

    solve(&mut streams.0, &mut streams.1);
}

mod test {
    #[test]
    fn test1() {
        let mut t_in = r#"1 2 3 -4
2
MOV b 3
MOV c a"#
            .as_bytes();
        let t_out = r#"1 3 1 -4"#;
        let mut out = Vec::new();
        crate::solve(&mut t_in, &mut out);
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out.trim());
    }
}
