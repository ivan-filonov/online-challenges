use std::io;

macro_rules! parse_input {
    ($x:expr, $t:ident) => ($x.trim().parse::<$t>().unwrap())
}

struct Main {
    initial_count : u32,
    must_survive : u32,
    lanes: Vec<u8>,
    visited: Vec<bool>,
    plan: Vec<Cmd>,
}

impl Main {
    fn new(initial_count: u32, must_survive: u32)->Self {
        Self{
            initial_count, must_survive,
            lanes : Vec::new(),
            visited: Vec::new(),
            plan: Vec::new(),
        }
    }

    // 0
    // 1
    // 2
    // 3
    // L0 to L3 are lanes of the road. A dot character . represents a safe space, a zero 0 represents a hole in the road.
    fn add_lane(&mut self, id: usize, s: &str) {
        assert!(id < 4);
        let bit : u8 = 1 << id;
        if self.lanes.len() < s.len() {
            self.lanes.resize(s.len(), 0);
        }
        for (&src, dst) in s.as_bytes().iter().zip(self.lanes.iter_mut()) {
            if src == b'.' {
                *dst |= bit;
            }
        }
    }

    fn lane_mask(&self, from : usize, to : usize) -> u8 {
        let to = if to > self.lanes.len() { self.lanes.len() } else { to };
        self.lanes[from..to] .iter().fold(15u8, |acc, val| acc & val)
    }

    fn sim(&self, x:u32, speed:u32, mask:u8, cmd: Cmd) -> (u32, u32, u8) {
        let speed = match cmd {
            Cmd::SPEED if speed <= 50 => speed + 1,
            Cmd::SLOW if speed > 0 => speed - 1,
            _=> speed
        };
        let mut mask = match cmd {
            Cmd::UP if mask % 2 == 0 => mask >> 1,
            Cmd::DOWN if mask < 8 => mask << 1,
            _=> mask
        };
        let xto = x + speed;
        let check_lane = match cmd { Cmd::JUMP=> false, _=> true};
        if check_lane && speed > 1 {
            let lane_mask = self.lane_mask(x as usize, xto as usize);
            mask &= lane_mask;
            match cmd {
                // TODO  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                Cmd::UP => mask &= lane_mask >> 1,
                Cmd::DOWN => mask &= lane_mask << 1,
                _ => {}
            }
        }
        mask = mask & self.lanes.get(xto as usize).unwrap_or(&15);
        (xto, speed, mask)
    }

    fn compress(x : u32, speed: u32, mask: u8) -> usize {
        let x = x as usize;
        let s = speed as usize;
        let m = mask as usize;
        m + 16 * (s + 51 * x)
    }

    fn dfs(&mut self, x : u32, speed: u32, mask: u8) -> bool {
        if self.visited.is_empty() {
            self.visited.resize(328000, false);
            self.plan.resize(328000, Cmd::SPEED);
        }
        let compressed_from = Main::compress(x, speed, mask);
        self.visited[compressed_from] = true;
        for cmd in Cmd::iter() {
            let (xto, speedto, maskto) = self.sim(x, speed, mask, *cmd);
            let score = maskto.count_ones();
            if score < self.must_survive {
                continue;
            }
            if xto as usize >= self.lanes.len() {
                self.plan[compressed_from] = *cmd;
                return true;
            }
            if !self.visited[Main::compress(xto, speedto, maskto)] {
                if self.dfs(xto, speedto, maskto) {
                    self.plan[compressed_from] = *cmd;
                    return true;
                }
            }
        }
        false
    }

    fn think(&mut self, x: u32, speed: u32, mask: u8) -> Cmd {
        if x == 0 {
            dbg!(x, speed, mask, self.dfs(x, speed, mask));
        }
        *self.plan.get(Self::compress(x, speed, mask)).unwrap()
    }
}

const CMD : &[&str] = &["SPEED", "SLOW", "JUMP", "WAIT", "UP", "DOWN"];

#[repr(u8)]
#[derive(Clone, Copy)]
enum Cmd { SPEED, SLOW, JUMP, WAIT, UP, DOWN }

impl Cmd {
    fn iter() -> std::slice::Iter<'static, Cmd> {
        static CMD : [Cmd; 6] = [Cmd::SPEED, Cmd::SLOW, Cmd::JUMP, Cmd::WAIT, Cmd::UP, Cmd::DOWN];
        CMD.iter()
    }
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
fn main() {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let initial_count = parse_input!(input_line, u32); // the amount of motorbikes to control
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let must_survive = parse_input!(input_line, u32); // the minimum amount of motorbikes that must survive

    let mut main = Main::new(initial_count, must_survive);

    for i in 0..4 {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        main.add_lane(i, input_line.trim());
    }

    // game loop
    loop {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let speed = parse_input!(input_line, u32); // the motorbikes' speed
        let mut mask = 0u8;
        let mut maxx = 0;
        for i in 0..initial_count as usize {
            let mut input_line = String::new();
            io::stdin().read_line(&mut input_line).unwrap();
            let inputs = input_line.split(" ").collect::<Vec<_>>();
            let x = parse_input!(inputs[0], u32); // x coordinate of the motorbike
            maxx = u32::max(maxx, x);
            let y = parse_input!(inputs[1], i32); // y coordinate of the motorbike
            eprintln!("id={} y={}", i, y);
            let a = parse_input!(inputs[2], i32); // indicates whether the motorbike is activated "1" or detroyed "0"
            if a == 1 {
                mask |= 1 << y;
            }
        }

        let cmd = main.think(maxx, speed, mask);

        // A single line containing one of 6 keywords: SPEED, SLOW, JUMP, WAIT, UP, DOWN.
        println!("{}", CMD[cmd as usize]);
    }
}
