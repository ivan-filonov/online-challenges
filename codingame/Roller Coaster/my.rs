use std::io;

macro_rules! parse_input {
    ($x:expr, $t:ident) => ($x.trim().parse::<$t>().unwrap())
}

#[derive(Copy, Clone)]
struct Saved {
    end : usize,
    count : u32,
}
fn solve(capacity : u32, max_runs: u32, groups: Vec<u32>) -> u64 {
    let mut end : usize = 0;
    let mut available : usize = groups.len();
    let mut result : u64 = 0;

    let mut saved : Vec<Option<Saved>> = vec![None; available];

    for turn in 0..max_runs {
        if let Some(saved_point) = saved[end] {
            result += saved_point.count as u64;
            end = saved_point.end;
            continue;
        }

        let mut count : u32 = 0;
        let start = end;
        while available > 0 && count + groups[end] <= capacity{
            count += groups[end];
            //eprint!("+{} ", groups[end]);
            end += 1;
            if end == groups.len() {
                end = 0;
            }
            available -= 1;
        }
        result += count as u64;
        available = groups.len();
        saved[start] = Some(Saved{end, count});
        //eprintln!("=> {} ({})", count, result);
    }

    result
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
fn main() {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let inputs = input_line.split(" ").collect::<Vec<_>>();
    let capacity = parse_input!(inputs[0], u32);
    let max_runs = parse_input!(inputs[1], u32);
    let num_groups = parse_input!(inputs[2], u32);
    let mut groups = Vec::new();
    eprintln!("cap {} runs {}", capacity, max_runs);
    for i in 0..num_groups as usize {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let group_size = parse_input!(input_line, u32);
        groups.push(group_size);
    }
    eprintln!("{:?}", groups);

    println!("{}", solve(capacity, max_runs, groups));
}

mod test {
    #[test]
    fn test() {
        use crate::solve;
        assert_eq!(solve(3, 3, vec![3, 1, 1, 2]), 7);
    }
    #[test]
    fn test2() {
        use crate::solve;
        assert_eq!(solve(5, 3, vec![2, 3, 5, 4]), 14);
    }
    #[test]
    fn test3() {
        use crate::solve;
        assert_eq!(solve(10, 100, vec![1]), 100);
    }
}
