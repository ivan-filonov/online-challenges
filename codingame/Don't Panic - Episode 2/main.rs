// https://www.codingame.com/ide/puzzle/don't-panic-episode-2
#[derive(Debug)]
struct InitialData {
    num_floors: i32,
    width: i32,
    num_rounds: i32,
    exit_floor: i32,
    exit_pos: i32,
    num_total_clones: i32,
    num_additional_elevators: i32,
    num_elevators: i32,
    elevator_floor: Vec<i32>,
    elevator_pos: Vec<i32>,
}

impl InitialData {
    fn empty() -> Self {
        Self {
            num_floors: 0,
            width: 0,
            num_rounds: 0,
            exit_floor: 0,
            exit_pos: 0,
            num_total_clones: 0,
            num_additional_elevators: 0,
            num_elevators: 0,
            elevator_floor: Vec::new(),
            elevator_pos: Vec::new(),
        }
    }
    fn parse_first_line(&mut self, line: &str) {
        let numbers: Vec<i32> = line.trim().split(" ").map(|s| s.parse().unwrap()).collect();
        assert_eq!(numbers.len(), 8);
        self.num_floors = numbers[0];
        self.width = numbers[1];
        self.num_rounds = numbers[2];
        self.exit_floor = numbers[3];
        self.exit_pos = numbers[4];
        self.num_total_clones = numbers[5];
        self.num_additional_elevators = numbers[6];
        self.num_elevators = numbers[7];
    }
    fn add_elevator(&mut self, line: &str) {
        let numbers: Vec<i32> = line.trim().split(" ").map(|s| s.parse().unwrap()).collect();
        assert_eq!(numbers.len(), 2);
        self.elevator_floor.push(numbers[0]);
        self.elevator_pos.push(numbers[1]);
    }
}

#[derive(Debug)]
struct RoundInfo {
    clone_floor: i32,
    clone_pos: i32,
    direction: i32,
}

trait Environment {
    fn read_initial(&mut self) -> InitialData;
    fn read_round(&mut self) -> Option<RoundInfo>;
    fn command(&mut self, command: Command);
}

enum Command {
    WAIT,
    BLOCK,
    ELEVATOR,
}

trait Solver {
    fn init(&mut self, initial_data: InitialData);
    fn next_cmd(&mut self, round_info: Option<RoundInfo>) -> Command;
    fn have_solution(&self) -> bool;
}

mod cg_env {
    use std::io::{self, BufRead, BufReader};

    use crate::{Environment, InitialData, RoundInfo};

    pub fn create_env() -> Box<dyn Environment> {
        Box::new(CgEnv::new())
    }
    struct CgEnv<'a> {
        reader: Box<dyn BufRead + 'a>,
    }
    impl<'a> CgEnv<'a> {
        fn new() -> Self {
            Self {
                reader: Box::new(BufReader::new(io::stdin())),
            }
        }
    }
    impl<'a> Environment for CgEnv<'a> {
        fn command(&mut self, command: crate::Command) {
            match command {
                crate::Command::WAIT => println!("WAIT"),
                crate::Command::BLOCK => println!("BLOCK"),
                crate::Command::ELEVATOR => println!("ELEVATOR"),
            }
        }
        fn read_round(&mut self) -> Option<crate::RoundInfo> {
            let mut buf = String::new();
            self.reader.read_line(&mut buf).unwrap();
            let parts: Vec<_> = buf.trim().split(" ").collect();
            match parts[2] {
                "NONE" => None,
                "LEFT" => Some(RoundInfo {
                    clone_floor: parts[0].parse().unwrap(),
                    clone_pos: parts[1].parse().unwrap(),
                    direction: -1,
                }),
                "RIGHT" => Some(RoundInfo {
                    clone_floor: parts[0].parse().unwrap(),
                    clone_pos: parts[1].parse().unwrap(),
                    direction: 1,
                }),
                _ => todo!(),
            }
        }
        fn read_initial(&mut self) -> InitialData {
            let mut initial_data = InitialData::empty();
            let mut buf = String::new();
            let mut d = String::new();
            self.reader.read_line(&mut buf).unwrap();
            d.push_str(&buf);
            initial_data.parse_first_line(buf.as_str());
            for _ in 0..initial_data.num_elevators {
                buf = String::new();
                self.reader.read_line(&mut buf).unwrap();
                initial_data.add_elevator(buf.as_str());
                d.push_str(&buf);
            }
            dbg!(d);
            initial_data
        }
    }
}

mod solver_main {
    use std::{
        cmp::{self, Ordering},
        collections::{BTreeSet, HashMap, HashSet},
    };

    use crate::{Command, InitialData, RoundInfo, Solver};
    pub fn create_solver() -> Box<dyn Solver> {
        Box::new(SolverImpl::new())
    }
    struct SolverImpl {
        initial_data: InitialData,
        first: bool,
        commands: String,
        command_pos: usize,
    }
    impl SolverImpl {
        fn solve(&mut self, start: RoundInfo) {
            #[derive(Clone)]
            struct Item {
                x: i32,
                y: i32,
                dir: i32,
                cost: i32,
                commands: String,
                elevators_avl: i32,
                rounds_avl: i32,
                can_block: bool,
                new_elevators: HashSet<(i32, i32)>,
            }
            impl Item {
                fn coords(&self) -> (i32, i32, i32, i32) {
                    (self.x, self.y, self.dir, self.elevators_avl)
                }
                fn comparable_part(&self) -> (i32, i32, i32, i32, i32, i32, usize) {
                    (
                        -self.rounds_avl,
                        self.cost,
                        -self.elevators_avl,
                        self.y,
                        self.x,
                        self.dir,
                        self.commands.len(),
                    )
                }
                fn wait(&self) -> Item {
                    let mut item = self.clone();
                    item.x += item.dir;
                    item.commands.push_str("W");
                    item.cost += 1;
                    item.can_block = false;
                    item.rounds_avl -= 1;
                    item
                }
                fn block(&self) -> Item {
                    let mut item = self.clone();
                    item.x -= item.dir;
                    item.dir = -item.dir;
                    item.commands.push_str("BWWW");
                    item.cost += 4;
                    item.can_block = false;
                    item.rounds_avl -= 3;
                    item
                }
                fn use_elevator(&self) -> Item {
                    let mut item = self.clone();
                    item.y += 1;
                    item.commands.push_str("W");
                    item.cost += 1;
                    item.can_block = true;
                    item.rounds_avl -= 1;
                    item
                }
                fn build_elevator(&self) -> Item {
                    let mut item = self.clone();
                    item.new_elevators.insert((self.x, self.y));
                    item.y += 1;
                    item.commands.push_str("EWWW");
                    item.cost += 4;
                    item.can_block = true;
                    item.elevators_avl -= 1;
                    item.rounds_avl -= 3;
                    item
                }
            }
            impl Ord for Item {
                fn cmp(&self, other: &Self) -> Ordering {
                    self.comparable_part().cmp(&other.comparable_part())
                }
            }
            impl PartialOrd for Item {
                fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
                    Some(self.cmp(other))
                }
            }
            impl PartialEq for Item {
                fn eq(&self, other: &Self) -> bool {
                    self.comparable_part() == other.comparable_part()
                }
            }
            impl Eq for Item {}
            let elevators: HashSet<_> = self
                .initial_data
                .elevator_pos
                .iter()
                .cloned()
                .zip(self.initial_data.elevator_floor.iter().cloned())
                .collect();
            let mut pqueue = BTreeSet::new();
            pqueue.insert(Item {
                x: start.clone_pos,
                y: start.clone_floor,
                dir: start.direction,
                cost: 0,
                commands: String::new(),
                elevators_avl: self.initial_data.num_additional_elevators,
                can_block: true,
                rounds_avl: self.initial_data.num_rounds,
                new_elevators: HashSet::new(),
            });
            let mut visited: HashMap<_, Item> = HashMap::new();
            let mut answer: Option<Item> = None;
            while let Some(item) = pqueue.pop_last() {
                if item.x < 0
                    || item.x >= self.initial_data.width
                    || item.y > self.initial_data.num_floors
                    || item.elevators_avl < 0
                    || item.rounds_avl < 0
                {
                    continue;
                }
                if item.x == self.initial_data.exit_pos && item.y == self.initial_data.exit_floor {
                    if answer.is_none() || answer.as_ref().unwrap().cmp(&item) == Ordering::Greater
                    {
                        answer = Some(item.clone());
                    }
                }
                if let Some(known) = visited.get(&item.coords()) {
                    if known.cmp(&item) != Ordering::Greater {
                        continue;
                    }
                }
                visited.insert(item.coords(), item.clone());
                let on_elevator = elevators.contains(&(item.x, item.y))
                    || item.new_elevators.contains(&(item.x, item.y));
                if on_elevator {
                    pqueue.insert(item.use_elevator());
                } else {
                    pqueue.insert(item.wait());
                    pqueue.insert(item.build_elevator());
                    if item.can_block {
                        pqueue.insert(item.block());
                    }
                }
            }
            dbg!(visited.len());
            let mut exit_coords = (
                self.initial_data.exit_pos,
                self.initial_data.exit_floor,
                1,
                self.initial_data.num_additional_elevators,
            );
            if let Some(answer) = answer {
                self.commands = answer.commands.clone();
            }
            dbg!(&self.commands);
        }
        fn new() -> Self {
            Self {
                initial_data: InitialData::empty(),
                first: true,
                commands: String::new(),
                command_pos: 0,
            }
        }
    }
    impl Solver for SolverImpl {
        fn have_solution(&self) -> bool {
            !self.commands.is_empty()
        }
        fn init(&mut self, initial_data: InitialData) {
            // dbg!(&initial_data);
            self.initial_data = initial_data;
        }
        fn next_cmd(&mut self, round_info: Option<RoundInfo>) -> Command {
            if round_info.is_some() {
                // dbg!(&round_info);
            }
            if self.first && round_info.is_some() {
                self.first = false;
                let round = round_info.unwrap();
                self.solve(round);
            }

            let ch = self.commands.as_bytes().get(self.command_pos);
            self.command_pos += 1;

            match ch {
                Some(b'E') => Command::ELEVATOR,
                Some(b'B') => Command::BLOCK,
                _ => Command::WAIT,
            }
        }
    }
}

fn main() {
    let mut e = cg_env::create_env();
    let initial_data = e.read_initial();
    let mut solver = solver_main::create_solver();
    solver.init(initial_data);
    loop {
        let o = e.read_round();
        let c = solver.next_cmd(o);
        e.command(c);
    }
}

mod test {
    use crate::{solver_main, Environment, InitialData, RoundInfo};

    struct TestEnvironment {
        lines: Vec<String>,
        pos: usize,
    }
    impl TestEnvironment {
        fn new(input: &str) -> Self {
            Self {
                lines: input.split("\n").map(|s| s.to_string()).collect(),
                pos: 0,
            }
        }
        fn next_line(&mut self) -> Option<String> {
            if self.pos >= self.lines.len() {
                None
            } else {
                let index = self.pos;
                self.pos += 1;
                Some(self.lines[index].to_string())
            }
        }
    }
    impl Environment for TestEnvironment {
        fn command(&mut self, _command: crate::Command) {}
        fn read_initial(&mut self) -> InitialData {
            let mut data = InitialData::empty();
            let line0 = self.next_line().unwrap();
            data.parse_first_line(line0.as_str());
            for _ in 0..data.num_elevators {
                data.add_elevator(self.next_line().unwrap().as_str());
            }
            data
        }
        fn read_round(&mut self) -> Option<RoundInfo> {
            None
        }
    }

    fn test(input: &str) {
        let mut e = TestEnvironment::new(input);
        let initial_data = e.read_initial();
        let mut s = solver_main::create_solver();
        let num_rounds = initial_data.num_rounds;
        s.init(initial_data);
        for _ in 0..num_rounds {
            s.next_cmd(None);
        }
        assert!(s.have_solution());
    }

    #[test]
    fn test1() {
        test("2 13 100 1 11 10 1 0\n");
    }
    #[test]
    fn test2() {
        test("13 36 67 11 12 41 4 34\n4 9\n10 3\n5 4\n2 23\n9 2\n3 34\n11 13\n1 24\n6 34\n2 24\n11 11\n5 34\n10 34\n4 23\n1 4\n8 1\n4 34\n1 17\n3 17\n2 34\n11 4\n6 13\n7 34\n8 9\n6 22\n8 34\n10 23\n8 23\n0 34\n9 17\n9 34\n2 3\n1 34\n7 17\n");
    }
}
