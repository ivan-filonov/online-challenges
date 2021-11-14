// A Turing machine used for computability research is well known to computer scientists. We will give a brief description of this abstraction. Turing machine is an automatic device that works on a tape (1) of potentially unlimited length. The tape is divided into cells each containing a character. One of the cells is called a viewed, or current, one (2). At any point of time the Turing machine has a condition that is stored in the control unit (4). Besides, the read/write head (3) of the control unit is pointing to the current cell.
// The control unit can execute one action per time interval (step). The action includes a state change, a possible change of the character in the current cell, and a possible movement to the adjacent cell. These actions are defined in a special table, called a control table. We will denote the movements along the tape with the following symbols: "<" — to the left, ">" — to the right, "=" — no movement.
// The control table is actually a program for the Turing machine. The work of the Turing machine is considered to be done if no line in the control table contains the combination of the current character and condition.
// The input data for the Turing Machine are placed beforehand in the cells of the tape. The result is written to the same tape. Assume that the initial condition for the Turing machine is equal to 1 and the input data on the tape are bounded on both ends with '#' characters. (All tape cells except those that filled with minuses are filled with '#' character.) The control block is placed at the leftmost '-' character of the input data. The input tape contains '-' (minus) character repeated n times (1 ≤ n ≤ 200), and the input contains an integer k.
// Imagine that the minuses are placed in circle. Starting with the first one each k-th uncrossed minus is crossed, i.e. it turns into a '+' (plus). The execution stops when there is only one uncrossed minus is left. Your task is to describe the control table for the Turing machine that will cross all minuses except one (it's position defined according to the above algorithm, but you may use any method) for the given k. For example, for n = 10 and k = 3 the fourth minus will remain uncrossed.
// You may place the following characters on the tape: '+', '#', 'A'..'Z'. The cells initially filled with minuses may only contain '-' and '+' characters. After the execution the read/write head must point to the uncrossed minus. The number of the steps s must not exceed 1 000 000. The number of the line in the control table p must not exceed 10000. Tape size limited with 10001 cells (5000 both side from the initial read/write head position).
// Input
// The input contains an integer number k (1 ≤ k ≤ 200).
// Output
// The output describes the control table for the Turing machine for the given k. The first line of the output contains the number of rows p in the table (1 < p < 10000). Then p lines follow describing the table itself. Each row of the table contains five items: current condition (an integer number), current character (a character), new condition (an integer number), new character (a character), moving direction (a character). The items are separated with a single space characters. The condition numbers may range from 1 to 30000.

//use std::io::prelude::*;
use std::collections::{HashMap, HashSet};
use std::convert::TryInto;
use std::io::{BufRead, BufReader, BufWriter, Read, Write};

struct Action {
    state: i32,
    c: char,
    new_state: i32,
    new_c: char,
    shift: char,
}

impl Action {
    fn new(state: i32, c: char) -> Self {
        Self {
            state: state,
            c: c,
            new_state: state,
            new_c: c,
            shift: '=',
        }
    }
    fn to_string(&self) -> String {
        let mut buf_out: Vec<u8> = Vec::new();
        write!(
            buf_out,
            "{} {} {} {} {}",
            self.state, self.c, self.new_state, self.new_c, self.shift
        );
        String::from_utf8(buf_out).unwrap()
    }
    fn from_string(s: &str) -> Self {
        let parts: Vec<_> = s.split(char::is_whitespace).collect();
        assert_eq!(parts.len(), 5);
        Self {
            state: parts[0].parse().unwrap(),
            c: parts[1].chars().next().unwrap(),
            new_state: parts[2].parse().unwrap(),
            new_c: parts[3].chars().next().unwrap(),
            shift: parts[4].chars().next().unwrap(),
        }
    }
}

fn run(tape: &mut Vec<char>, state: i32, head: usize, table: &Vec<Action>) {
    let mut index: HashMap<(i32, char), &Action> = HashMap::new();
    for action in table.iter() {
        index.insert((action.state, action.c), action);
    }
    let mut state = 1;
    let mut head = 1;
    for _ in 0..1000000 {
        let c = tape[head];
        match index.get(&(state, c)) {
            Some(action) => {
                tape[head] = action.new_c;
                let new_head = match action.shift {
                    '>' => head + 1,
                    '<' => head - 1,
                    '=' => head,
                    _ => panic!(),
                };
                state = action.new_state;
                head = new_head;
            }
            None => break,
        };
    }
}

// 1231. Turing: One, Two, Three, …
fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn test_tm() {
        use crate::*;
        let descr = r#"1 - 2 - >
2 - 3 + >
3 # 4 # <
4 + 4 + <
4 - 5 - ="#;
        let tm: Vec<_> = descr.split('\n').map(|s| Action::from_string(s)).collect();
        let mut tape: Vec<char> = Vec::new();
        tape.push('#');
        for _ in 0..2 {
            tape.push('-');
        }
        tape.push('#');
        run(&mut tape, 1, 1, &tm);
        let s: String = tape.into_iter().collect();
        assert_eq!(s, "#-+#");
    }

    #[test]
    fn test_action() {
        use crate::*;
        let ac = Action::from_string("1 - 2 - >");
        assert_eq!(ac.state, 1);
        assert_eq!(ac.new_state, 2);
        assert_eq!(ac.c, '-');
        assert_eq!(ac.new_c, '-');
        assert_eq!(ac.shift, '>');
        assert_eq!(
            Action::from_string(&ac.to_string()).to_string(),
            ac.to_string()
        );
    }

    #[test]
    fn test() {
        use crate::solve;
        let mut buf_in = r#"2"#.as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(
            output.trim(),
            r#"5
1 - 2 - >
2 - 3 + >
3 # 4 # <
4 + 4 + <
4 - 5 - =
        "#
            .trim()
        );
    }
}
