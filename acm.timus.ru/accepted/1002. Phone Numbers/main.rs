use std::io::prelude::*;
use std::io::{BufReader, BufWriter, Read, Write};

const LETTER_ENCODING: &[u32] = &[
    2, 2, 2, 3, 3, 3, 4, 4, 1, 1, 5, 5, 6, 6, 0, 7, 0, 7, 7, 8, 8, 8, 9, 9, 9, 0,
];

struct Trie {
    next: Vec<i32>,
    word: Vec<i32>,
    used: usize,
    capacity: usize,
}
impl Trie {
    fn new(new_capacity: usize) -> Trie {
        Trie {
            next: vec![0; new_capacity * 10],
            word: vec![-1; new_capacity * 10],
            used: 0,
            capacity: new_capacity,
        }
    }

    fn clear(&mut self) {
        for x in self.next.iter_mut() {
            *x = 0;
        }
        for x in self.word.iter_mut() {
            *x = -1;
        }
        self.used = 0;
    }

    fn insert(&mut self, word: &str, word_id: usize) {
        let mut index: usize = 0;
        let mut ofs: usize = 0;
        for b in word.bytes() {
            if !(b as char).is_ascii_lowercase() {
                continue;
            }
            let cur = LETTER_ENCODING[(b - 'a' as u8) as usize];
            ofs = index * 10 + cur as usize;
            if ofs >= self.next.len() {
                self.capacity *= 2;
                self.next.resize(self.capacity * 10, 0);
                self.word.resize(self.capacity * 10, -1);
            }
            if self.next[ofs] == 0 {
                self.used += 1;
                self.next[ofs] = self.used as i32;
            }
            index = self.next[ofs] as usize;
        }
        self.word[ofs] = word_id as i32;
    }

    fn scan(&self, word: &str, pos: usize) -> Vec<usize> {
        let iter = word[pos..].bytes().map(|b| b - '0' as u8);
        let mut index: usize = 0;
        let mut result: Vec<usize> = Vec::new();
        for cur in iter {
            let ofs = index * 10 + cur as usize;
            index = self.next[ofs] as usize;
            let word = self.word[ofs];
            if word > -1 {
                result.push(word as usize);
            }
            if 0 == index {
                break;
            }
        }
        result
    }
}

enum State {
    GetPnum,
    GetWordCount,
    GetWords { count: usize },
}

fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
    let mut words: Vec<String> = Vec::new();
    let mut trie = Trie::new(1000);
    let mut state: State = State::GetPnum;
    let mut pnum: String = String::new();
    for line in reader.lines().map(|a| a.expect("correct input")) {
        state = match state {
            State::GetPnum => {
                if line == "-1" {
                    break;
                }
                pnum = line;
                trie.clear();
                words.clear();
                State::GetWordCount
            }
            State::GetWordCount => State::GetWords {
                count: line.parse::<usize>().expect("correct integer"),
            },
            State::GetWords { count } => {
                trie.insert(&line, words.len());
                words.push(line);
                State::GetWords { count: count - 1 }
            }
        };
        if let State::GetWords { count: 0 } = state {
            let mut prefixes: Vec<Vec<usize>> = Vec::new();
            prefixes.resize(pnum.len() + 1, Vec::new());
            for i in 0..=pnum.len() {
                let prefix = prefixes[i].clone();
                if i > 0 && prefix.len() == 0 {
                    continue;
                }
                let new_prefix_len = prefix.len() + 1;
                for word_id in trie.scan(&pnum, i as usize) {
                    let word_len = words[word_id].len();
                    let next_index = i + word_len;
                    if prefixes[next_index].len() == 0
                        || prefixes[next_index].len() > new_prefix_len
                    {
                        let mut new_prefix = prefix.clone();
                        new_prefix.push(word_id);
                        prefixes[next_index] = new_prefix;
                    }
                }
            }
            let answer_index = pnum.len();
            let answer = &prefixes[answer_index];
            if answer.len() == 0 {
                writeln!(writer, "No solution.");
            } else {
                for (n, w) in answer.iter().enumerate() {
                    if n > 0 {
                        write!(writer, " ");
                    }
                    write!(writer, "{}", words[*w]);
                }
                writeln!(writer, "");
            }

            state = State::GetPnum;
        }
    }
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn basic_test() {
        use crate::solve;
        let mut buf_in = r#"2285064252258215
5
long
false
cat
tail
black
-1
"#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(
            output,
            r#"cat long black tail
"#
        );
    }
}
