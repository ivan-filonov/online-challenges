type Int = i64;

#[derive(Debug, Clone)]
pub struct Input {
    pos_a: Int,
    pos_b: Int,
    jump_a: Int,
    jump_b: Int,
    div: Int,
}

mod cg_env {
    use std::io;

    use crate::*;

    pub fn read() -> Input {
        let mut line = String::new();
        io::stdin().read_line(&mut line).unwrap();
        let mut iter = line
            .trim()
            .split_whitespace()
            .map(|s| s.parse::<Int>().unwrap());
        let pos_a = iter.next().unwrap();
        let pos_b = iter.next().unwrap();
        let jump_a = iter.next().unwrap();
        let jump_b = iter.next().unwrap();
        let div = iter.next().unwrap();
        Input {
            pos_a,
            pos_b,
            jump_a,
            jump_b,
            div,
        }
    }
}

mod solver_main {
    use crate::*;
    fn gcd(a: Int, b: Int, x: &mut Int, y: &mut Int) -> Int {
        if a == 0 {
            *x = 0;
            *y = 1;
            return b;
        }
        let mut x1 = 0;
        let mut y1 = 0;
        let d = gcd(b % a, a, &mut x1, &mut y1);
        *x = y1 - (b / a) * x1;
        *y = x1;
        return d;
    }
    pub fn solve(input: Input) -> Int {
        let jump_a = input.jump_a % input.div;
        let jump_b = input.jump_b % input.div;
        if jump_a == jump_b {
            return 0;
        }
        let pos_a = input.pos_a % input.div;
        let pos_b = input.pos_b % input.div;
        let div = input.div;
        let (mut pos_slow, mut pos_fast, mut jump_slow, mut jump_fast) = if jump_a < jump_b {
            (pos_a, pos_b, jump_a, jump_b)
        } else {
            (pos_b, pos_a, jump_b, jump_a)
        };
        if pos_slow < pos_fast {
            pos_slow += div;
        }
        let initial_dist = pos_slow - pos_fast;
        let rel_speed = jump_fast - jump_slow;
        eprintln!(
            "pos_slow: {}, pos_fast: {}, jump_slow: {}, jump_fast: {} , div: {}",
            &pos_slow, &pos_fast, &jump_slow, &jump_fast, &div
        );

        eprintln!(
            "initial_dist: {}, rel_speed: {}, i_d % r_s: {}, div % r_s: {}",
            &initial_dist,
            &rel_speed,
            initial_dist % rel_speed,
            div % rel_speed
        );
        if initial_dist % rel_speed == 0 {
            return initial_dist / rel_speed;
        }

        // rel_speed * answer + div * x = initial_dist
        let (mut x, mut y) = (0, 0);
        let d = gcd(rel_speed, div, &mut x, &mut y);
        if initial_dist % d != 0 {
            eprintln!("initial_dist % d != 0");
            return 0;
        }

        eprintln!("d: {}, x: {}, y: {}", &d, &x, &y);

        eprintln!(
            "{} * ({}) + {} * ({}) = {} ",
            rel_speed,
            x * initial_dist / d,
            div,
            y * initial_dist / d,
            initial_dist
        );

        if x < 0 {
            eprintln!("div/d: {}, r_s/d: {}", div / d, rel_speed / d);
            while x < 0 {
                x += div / d;
                y -= rel_speed / d;
            }
            eprintln!(
                "{} * ({}) + {} * ({}) = {} ",
                rel_speed,
                x * initial_dist / d,
                div,
                y * initial_dist / d,
                initial_dist
            );
        }

        (x * initial_dist / d) % (div / d)
    }
}

fn main() {
    let input = cg_env::read();
    let answer = solver_main::solve(input);
    if answer > 0 {
        println!("{}", answer);
    } else {
        println!("Impossible");
    }
}

mod test {
    use rand::Rng;

    use crate::*;

    // generate 5 random numbers in 0 .. 100
    fn gen_random_input() -> Input {
        let mut rng = rand::thread_rng();
        let mut input = Input {
            pos_a: rng.gen_range(0..1000),
            pos_b: rng.gen_range(0..1000),
            jump_a: rng.gen_range(0..1000),
            jump_b: rng.gen_range(0..1000),
            div: rng.gen_range(2..1000),
        };
        input.pos_a = (input.pos_a + input.jump_a) % input.div;
        input.pos_b = (input.pos_b + input.jump_b) % input.div;
        input.jump_a = input.jump_a % input.div;
        input.jump_b = input.jump_b % input.div;
        input
    }

    fn brute_force(input: &Input) -> Int {
        let mut a = input.pos_a;
        let mut b = input.pos_b;
        for i in 0..10000 {
            if a == b {
                return i;
            }
            a = (a + input.jump_a) % input.div;
            b = (b + input.jump_b) % input.div;
        }
        0
    }

    fn test(input: Input, expected: Int) {
        let answer = solver_main::solve(input);
        assert_eq!(
            answer, expected,
            "answer = {} != expected = {}",
            answer, expected
        );
    }
    #[test]
    fn test1() {
        test(
            Input {
                pos_a: 1,
                pos_b: 2,
                jump_a: 3,
                jump_b: 4,
                div: 5,
            },
            4,
        );
    }

    #[test]
    fn multi_tests1() {
        for _ in 0..10 {
            let input = gen_random_input();
            let brute = brute_force(&input);
            let answer = solver_main::solve(input.clone());
            assert_eq!(
                brute, answer,
                "bruteforced {:?} != solver's answer {:?}\ninput was: {:?}",
                brute, answer, input
            );
            eprintln!("test passed for {:?}", input);
        }
    }
}
