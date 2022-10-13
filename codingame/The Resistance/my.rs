use std::{
    collections::HashSet,
    io::{BufRead, BufReader, BufWriter, Read, Write},
};

macro_rules! parse_input {
    ($x:expr, $t:ident) => {
        $x.trim().parse::<$t>().unwrap()
    };
}

struct State<'a> {
    morse_alpha: Vec<&'a str>,
}

struct Trie {
    next: Vec<i32>,
    word_len: Vec<i8>,
    word_num: Vec<u32>,
    used: usize,
    capacity: usize,
}

impl Trie {
    fn new(new_capacity: usize) -> Self {
        Self {
            next: vec![0; new_capacity * 2],
            word_len: vec![0; new_capacity * 2],
            word_num: vec![0; new_capacity * 2],
            used: 0,
            capacity: new_capacity,
        }
    }
    fn insert(&mut self, word: Vec<u8>) {
        let mut index: usize = 0;
        let mut ofs: usize = 0;
        let word_len = word.len();
        for cur in word {
            ofs = index * 2 + cur as usize;
            if ofs >= self.next.len() {
                self.capacity *= 2;
                self.next.resize(self.capacity * 2, 0);
                self.word_len.resize(self.capacity * 2, 0);
                self.word_num.resize(self.capacity * 2, 0);
            }
            if self.next[ofs] == 0 {
                self.used += 1;
                self.next[ofs] = self.used as i32;
            }
            index = self.next[ofs] as usize;
        }
        self.word_len[ofs] = word_len as i8;
        self.word_num[ofs] += 1;
    }

    fn scan(&self, word: &Vec<u8>, pos: usize) -> Vec<(i32, i32)> {
        let iter = word.iter().skip(pos);
        let mut index = 0usize;
        let mut result = HashSet::new();
        for &cur in iter {
            let ofs = index * 2 + cur as usize;
            index = self.next[ofs] as usize;
            let word_len = self.word_len[ofs];
            if word_len > 0 {
                result.insert((word_len, self.word_num[ofs]));
            }
            if 0 == index {
                break;
            }
        }
        result
            .into_iter()
            .map(|(x, n)| (x as i32, n as i32))
            .collect()
    }
}

impl<'a> State<'a> {
    fn new() -> Self {
        let parts: Vec<&str> = MORSE.split("\n").skip(1).step_by(2).collect();
        State { morse_alpha: parts }
    }

    fn encode(&self, s: &str) -> Vec<u8> {
        s.as_bytes()
            .iter()
            .map(|b| self.morse_alpha[(b - 'A' as u8) as usize])
            .collect::<String>()
            .bytes()
            .map(|x| if x == '.' as u8 { 0 } else { 1 })
            .collect()
    }

    fn solve(&mut self, input: &mut dyn Read, writer: &mut dyn Write) {
        let mut reader = BufReader::new(input);
        let mut writer = BufWriter::new(writer);

        let mut encoded = String::new();
        reader.read_line(&mut encoded).expect("encoded line");
        encoded.pop();
        eprintln!("encoded input: \"{}\"", encoded);
        let encoded: Vec<u8> = encoded
            .into_bytes()
            .into_iter()
            .map(|x| if x == '.' as u8 { 0 } else { 1 })
            .collect();

        let mut buf = String::new();
        reader.read_line(&mut buf).expect("word count");
        let num_words: i32 = buf.trim().parse().expect("number");
        eprintln!("word count: {}", num_words);

        let mut trie = Trie::new((num_words * 4) as usize);
        for _ in 0..num_words {
            buf.clear();
            reader.read_line(&mut buf).expect("word");
            let word = buf.trim_end();
            trie.insert(self.encode(word));
        }

        let mut data = vec![0u64; encoded.len() + 1];
        data[0] = 1;
        for index in 0..encoded.len() {
            let count = data[index];
            if count == 0 {
                continue;
            }

            for (word_len, word_num) in trie.scan(&encoded, index) {
                let offset = index + word_len as usize;
                if offset < data.len() {
                    data[offset] += count * word_num as u64;
                }
            }
        }
        writeln!(writer, "{}", data.last().unwrap());
    }
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
fn main() {
    let mut state = State::new();
    state.solve(&mut std::io::stdin(), &mut std::io::stdout());
}

const MORSE: &str = r#"A
.-
B
-...
C
-.-.
D
-..
E
.
F
..-.
G
--.
H
....
I
..
J
.---
K
-.-
L
.-..
M
--
N
-.
O
---
P
.--.
Q
--.-
R
.-.
S
...
T
-
U
..-
V
...-
W
.--
X
-..-
Y
-.--
Z
--..
"#;

mod tests {
    #[test]
    fn test() {
        use crate::State;
        let mut buf_in = r#"......-...-..---.-----.-..-..-..
5
HELL
HELLO
OWORLD
WORLD
TEST"#
            .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        State::new().solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(output.trim(), r#"2"#.trim());
    }
}
