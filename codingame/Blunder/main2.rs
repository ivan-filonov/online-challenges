use std::io::{self, BufReader, BufWriter, Read, Write};

const ECHO: bool = false;
const DEBUG: bool = true;

mod v1 {
    use crate::{DEBUG, ECHO};
    use std::{
        io::{BufRead, BufReader, BufWriter, Read, Write},
        mem::MaybeUninit,
    };

    struct Room {
        value: i32,
        next: [Option<usize>; 2],
        suffix_value: Option<i32>,
    }

    pub struct State {
        rooms: [Room; 10000],
    }

    impl State {
        pub fn new() -> Self {
            let mut rooms: [MaybeUninit<Room>; 10000] =
                unsafe { MaybeUninit::uninit().assume_init() };
            for room in &mut rooms {
                room.write(Room {
                    value: 0,
                    next: [None; 2],
                    suffix_value: None,
                });
            }
            Self {
                rooms: unsafe { std::mem::transmute(rooms) },
            }
        }

        pub fn read(&mut self, reader: &mut BufReader<&mut dyn Read>) {
            let mut buf = String::new();
            reader.read_line(&mut buf).unwrap();
            if ECHO {
                eprintln!("{}", buf.trim());
            }
            let count: usize = buf.trim().parse().unwrap();

            for _ in 0..count {
                buf = String::new();
                reader.read_line(&mut buf).unwrap();
                if ECHO {
                    eprintln!("{}", buf.trim());
                }
                let mut it = buf.trim().split(" ");
                let room: i32 = it.next().unwrap().parse().unwrap();
                let room_ref = &mut self.rooms[room as usize];
                room_ref.value = it.next().unwrap().parse().unwrap();
                assert!(room_ref.next[0].is_none());
                room_ref.next[0] = it.next().unwrap().parse().ok();
                assert!(room_ref.next[1].is_none());
                room_ref.next[1] = it.next().unwrap().parse().ok();
            }
        }

        pub fn write(&mut self, writer: &mut BufWriter<&mut dyn Write>) {
            writeln!(writer, "{}", self.rooms[0].suffix_value.unwrap());
        }

        fn dfs(&mut self, room: usize, visited: &mut Vec<bool>) {
            if visited[room] {
                return;
            }
            visited[room] = true;
            let suffix0 = if let Some(index) = self.rooms[room].next[0] {
                self.dfs(index, visited);
                self.rooms[self.rooms[room].next[0].unwrap()].suffix_value
            } else {
                None
            };
            let suffix1 = if let Some(index) = self.rooms[room].next[1] {
                self.dfs(index, visited);
                self.rooms[self.rooms[room].next[1].unwrap()].suffix_value
            } else {
                None
            };
            self.rooms[room].suffix_value = Some(
                self.rooms[room].value
                    + if suffix0.is_some() && suffix1.is_some() {
                        suffix0.unwrap().max(suffix1.unwrap())
                    } else if suffix0.is_some() {
                        suffix0.unwrap()
                    } else if suffix1.is_some() {
                        suffix1.unwrap()
                    } else {
                        0
                    },
            );
        }

        pub fn solve(&mut self) {
            let mut visited = vec![false; 10000];
            self.dfs(0, &mut visited);
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
        let mut t_in = r#"15
0 17 1 2
1 15 3 4
2 15 4 5
3 20 6 7
4 12 7 8
5 11 8 9
6 18 10 11
7 19 11 12
8 12 12 13
9 11 13 14
10 13 E E
11 14 E E
12 17 E E
13 19 E E
14 15 E E"#
            .as_bytes();
        let t_out = r#"88"#;
        let mut out = Vec::new();
        crate::solve(&mut t_in, &mut out);
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out.trim());
    }
}
