// https://www.codingame.com/ide/puzzle/there-is-no-spoon-episode-2
trait Environment {
    fn read(&mut self) -> Vec<String>;
    fn write(&mut self, links: Vec<SolutionLink>);
}
#[derive(Debug)]
struct SolutionLink {
    x1: i32,
    y1: i32,
    x2: i32,
    y2: i32,
    amount: i32,
}
trait Solver {
    fn solve(&mut self, input: Vec<String>) -> Vec<SolutionLink>;
    fn solved(&self) -> bool;
}
mod cg_env {
    use std::io::{self, BufRead, BufReader};

    use crate::Environment;

    pub fn create_env() -> Box<dyn Environment> {
        Box::new(CgEnv {})
    }
    struct CgEnv {}
    impl Environment for CgEnv {
        fn read(&mut self) -> Vec<String> {
            let mut v = Vec::new();
            let mut s: String;
            let mut r = BufReader::new(io::stdin());
            for _ in 0..2 {
                s = String::new();
                r.read_line(&mut s).unwrap();
                v.push(s.trim().to_string());
            }
            let h: usize = v[1].parse().unwrap();
            for _ in 0..h {
                s = String::new();
                r.read_line(&mut s).unwrap();
                v.push(s.trim().to_string());
            }
            let d = v.join("\n");
            dbg!(d);
            v
        }
        fn write(&mut self, links: Vec<crate::SolutionLink>) {
            for link in links {
                println!(
                    "{} {} {} {} {}",
                    link.x1, link.y1, link.x2, link.y2, link.amount
                );
            }
        }
    }
}

mod solver_main {
    use std::{
        collections::{HashSet, VecDeque},
        rc::Rc,
    };

    use crate::{SolutionLink, Solver};
    pub fn create_solver() -> Box<dyn Solver> {
        Box::new(SolverImpl::new())
    }
    struct Info {
        node_x: Vec<usize>,
        node_y: Vec<usize>,
        node_power: Vec<i32>,
        node_links: Vec<Vec<usize>>,
        link_node1: Vec<usize>,
        link_node2: Vec<usize>,
        link_intersected: Vec<Vec<usize>>,
        link_length: Vec<i32>,
    }
    impl Info {
        fn empty() -> Self {
            Self {
                node_x: Vec::new(),
                node_y: Vec::new(),
                node_power: Vec::new(),
                node_links: Vec::new(),
                link_node1: Vec::new(),
                link_node2: Vec::new(),
                link_intersected: Vec::new(),
                link_length: Vec::new(),
            }
        }
        fn build_from(width: usize, height: usize, board: Vec<u8>) -> Self {
            // init nodes
            let mut node_id = 0;
            let mut node_x = Vec::new();
            let mut node_y = Vec::new();
            let mut node_power = Vec::new();
            let mut nodes_by_x: Vec<_> = (0..width).map(|_| Vec::<usize>::new()).collect();
            let mut nodes_by_y: Vec<_> = (0..height).map(|_| Vec::<usize>::new()).collect();
            let mut node_links = Vec::new();
            for y in 0..height {
                for x in 0..width {
                    let ofs = y * width + x;
                    let ch = board[ofs];
                    if ch == b'.' {
                        continue;
                    }
                    nodes_by_x[x].push(node_id);
                    nodes_by_y[y].push(node_id);

                    node_x.push(x);
                    node_y.push(y);
                    node_power.push((ch - b'0') as i32);
                    node_links.push(Vec::new());
                    node_id += 1;
                }
            }

            // init links
            assert_eq!(width * height, board.len());
            let mut hlinks = Vec::<i32>::with_capacity(width * height);
            for _ in 0..width * height {
                hlinks.push(-1);
            }
            let mut link_id = 0;
            let mut link_node1 = Vec::new();
            let mut link_node2 = Vec::new();
            let mut link_length = Vec::new();
            for y in 0..height {
                let v = &nodes_by_y[y];
                if v.len() < 2 {
                    continue;
                }
                for i in 0..v.len() - 1 {
                    let (node1, node2) = (v[i], v[i + 1]);
                    let x1 = node_x[node1];
                    let x2 = node_x[node2];
                    assert!(x1 < x2);
                    for x in x1 + 1..x2 {
                        hlinks[y * width + x] = link_id as i32;
                    }
                    node_links[node1].push(link_id);
                    node_links[node2].push(link_id);
                    link_node1.push(node1.min(node2));
                    link_node2.push(node2.max(node1));
                    link_length.push((x2 - x1) as i32);
                    link_id += 1;
                }
            }

            // link intersections
            let mut intersects = Vec::with_capacity(link_node1.len());
            for _ in 0..link_node1.len() {
                intersects.push(Vec::new());
            }
            for x in 0..width {
                let v = &nodes_by_x[x];
                if v.len() < 2 {
                    continue;
                }
                for i in 0..v.len() - 1 {
                    let (node1, node2) = (v[i], v[i + 1]);
                    let y1 = node_y[node1];
                    let y2 = node_y[node2];
                    assert!(y1 < y2);
                    let mut current_intersects = Vec::new();
                    for y in y1 + 1..y2 {
                        let hlink = hlinks[y * width + x];
                        if hlink < 0 {
                            continue;
                        }
                        current_intersects.push(hlink as usize);
                    }
                    for &id in current_intersects.iter() {
                        intersects[id].push(link_id);
                    }

                    intersects.push(current_intersects);
                    link_node1.push(node1.min(node2));
                    link_node2.push(node2.max(node1));
                    link_length.push((y2 - y1) as i32);
                    node_links[node1].push(link_id);
                    node_links[node2].push(link_id);
                    link_id += 1;
                }
            }

            // finalize
            Self {
                node_x,
                node_y,
                node_power,
                node_links,
                link_node1,
                link_node2,
                link_intersected: intersects,
                link_length,
            }
        }
    }
    struct State {
        info: Rc<Info>,
        node_unspent: Vec<i32>,
        sum_unspent: i32,
        link_unspent: Vec<i32>,
        link_rank: Vec<i32>,
        dirty_nodes: Vec<usize>,
    }
    impl State {
        fn dup(&self) -> Self {
            Self {
                info: self.info.clone(),
                node_unspent: self.node_unspent.clone(),
                sum_unspent: self.sum_unspent.clone(),
                link_unspent: self.link_unspent.clone(),
                link_rank: self.link_rank.clone(),
                dirty_nodes: Vec::new(),
            }
        }
        fn get_possible_moves(&self) -> Vec<(usize, i32)> {
            self.link_unspent
                .iter()
                .enumerate()
                .filter_map(|(link_id, &unspent)| {
                    if unspent > 0 && self.link_rank[link_id] > -1 {
                        Some((link_id, unspent))
                    } else {
                        None
                    }
                })
                .collect()
        }
        fn add_link(&mut self, link_id: usize, amount: i32) -> bool {
            assert!(self.link_rank[link_id] != -1);
            assert!(
                self.link_unspent[link_id] >= amount,
                "add_link id={link_id}: unspent={} amount={}",
                self.link_unspent[link_id],
                amount
            );
            let nodes = [self.info.link_node1[link_id], self.info.link_node2[link_id]];
            for check_node in nodes {
                assert!(
                    self.node_unspent[check_node] >= amount,
                    "node {},{}: unspent={}, amount to use={}",
                    self.info.node_x[check_node],
                    self.info.node_y[check_node],
                    self.node_unspent[check_node],
                    amount
                );
            }

            // update link and its nodes
            self.link_rank[link_id] += amount;
            self.link_unspent[link_id] -= amount;
            let mut dirty_nodes = HashSet::new();
            for node_id in nodes {
                self.node_unspent[node_id] -= amount;
                self.sum_unspent -= amount;
                let node_links = &self.info.node_links[node_id]; //.clone();
                for &dirty_link in node_links.iter() {
                    if self.link_unspent[dirty_link] > self.node_unspent[node_id] {
                        self.link_unspent[dirty_link] = self.node_unspent[node_id];
                        dirty_nodes.insert(&self.info.link_node1[dirty_link]);
                        dirty_nodes.insert(&self.info.link_node2[dirty_link]);
                    }
                }
            }

            // cross links
            for &crossed_id in self.info.link_intersected[link_id].iter() {
                assert!(self.link_rank[crossed_id] <= 0);
                self.link_rank[crossed_id] = -1;
                self.link_unspent[crossed_id] = 0;
                dirty_nodes.insert(&self.info.link_node1[crossed_id]);
                dirty_nodes.insert(&self.info.link_node2[crossed_id]);
            }

            // validate connectivity
            if self.node_unspent[nodes[0]] == 0 && self.node_unspent[nodes[1]] == 0 {
                let mut deque = VecDeque::new();
                deque.extend(nodes);
                let mut visited = Vec::with_capacity(self.num_nodes());
                for _ in 0..self.num_nodes() {
                    visited.push(false);
                }
                let mut open = false;
                let mut count = 0;
                while let Some(node_id) = deque.pop_front() {
                    if visited[node_id] {
                        continue;
                    }
                    visited[node_id] = true;
                    count += 1;
                    if self.node_unspent[node_id] > 0 {
                        open = true;
                        break;
                    }
                    for &link_id in self.info.node_links[node_id].iter() {
                        if self.link_rank[link_id] > 0 {
                            deque.push_back(self.info.link_node1[link_id]);
                            deque.push_back(self.info.link_node2[link_id]);
                        }
                    }
                }
                if count < self.num_nodes() && !open {
                    return false;
                }
            }

            self.dirty_nodes.extend(dirty_nodes);
            true
        }
        fn node_links_unspent_sum(&self, node_id: usize) -> i32 {
            self.info.node_links[node_id]
                .iter()
                .filter_map(|&link_id| {
                    if self.link_rank[link_id] < 0 {
                        None
                    } else {
                        Some(self.link_unspent[link_id])
                    }
                })
                .sum()
        }
        fn num_nodes(&self) -> usize {
            self.node_unspent.len()
        }
        fn num_links(&self) -> usize {
            self.link_unspent.len()
        }
        fn build_sure_links(&mut self) -> bool {
            let mut deque = VecDeque::new();
            deque.extend(0..self.num_nodes());

            // spending full nodes
            let mut modified = false;
            while let Some(node_id) = deque.pop_front() {
                let unspent = self.node_unspent[node_id];
                let avl_links = self.node_links_unspent_sum(node_id);

                if unspent == avl_links {
                    // connect node
                    let links = self.info.node_links[node_id].clone();
                    for link_id in links {
                        if self.link_rank[link_id] == -1 || self.link_unspent[link_id] == 0 {
                            continue;
                        }
                        if !self.add_link(link_id, self.link_unspent[link_id]) {
                            return false;
                        }
                        modified = true;
                    }
                } else if unspent == avl_links - 1 {
                    // check if all links of this node can accept value 2:
                    let all_links_unspent_eq_2 = self.info.node_links[node_id]
                        .iter()
                        .filter_map(|&link_id| {
                            if self.link_rank[link_id] > -1 && self.link_unspent[link_id] == 2 {
                                Some(true)
                            } else {
                                None
                            }
                        })
                        .fold(true, |a, b| a && b);
                    if all_links_unspent_eq_2 {
                        let links = self.info.node_links[node_id].clone();
                        for link_id in links {
                            if self.link_rank[link_id] == -1 || self.link_unspent[link_id] != 2 {
                                continue;
                            }
                            if !self.add_link(link_id, 1) {
                                return false;
                            }
                            modified = true;
                        }
                    }
                }

                // push dirty nodes to retest
                if !self.dirty_nodes.is_empty() {
                    deque.extend(self.dirty_nodes.iter());
                    self.dirty_nodes.clear();
                }
            }

            modified
        }
        fn is_valid(&self) -> bool {
            // node links vs node power
            for node_id in 0..self.num_nodes() {
                let unspent = self.node_unspent[node_id];
                if unspent == 0 {
                    continue;
                }
                let avl = self.node_links_unspent_sum(node_id);
                if avl < unspent {
                    return false;
                }
            }
            true
        }
        fn is_solved(&self) -> bool {
            self.sum_unspent == 0
        }
        fn initial_for_info(info: Rc<Info>) -> Self {
            let num_nodes = info.node_x.len();
            assert_eq!(info.node_power.len(), num_nodes);
            assert_eq!(info.node_y.len(), num_nodes);
            let node_unspent = info.node_power.clone();
            let num_links = info.link_node1.len();
            assert_eq!(info.link_node2.len(), num_links);
            let mut link_unspent = Vec::with_capacity(num_links);
            let mut link_rank = Vec::with_capacity(num_links);
            for link_id in 0..num_links {
                link_rank.push(0);
                link_unspent.push(
                    node_unspent[info.link_node1[link_id]]
                        .min(node_unspent[info.link_node2[link_id]])
                        .min(2),
                );
            }

            let sum_unspent = node_unspent.iter().sum();
            Self {
                info,
                node_unspent,
                sum_unspent,
                link_unspent,
                link_rank,
                dirty_nodes: Vec::new(),
            }
        }
        fn empty() -> Self {
            Self {
                info: Rc::new(Info::empty()),
                node_unspent: Vec::new(),
                sum_unspent: 0,
                link_unspent: Vec::new(),
                link_rank: Vec::new(),
                dirty_nodes: Vec::new(),
            }
        }
        fn build_solution(&self) -> Vec<SolutionLink> {
            self.link_rank
                .iter()
                .enumerate()
                .filter_map(|(link_id, &rank)| {
                    if rank < 1 {
                        None
                    } else {
                        Some((
                            rank,
                            self.info.link_node1[link_id],
                            self.info.link_node2[link_id],
                        ))
                    }
                })
                .map(|(rank, node1, node2)| SolutionLink {
                    x1: self.info.node_x[node1] as i32,
                    y1: self.info.node_y[node1] as i32,
                    x2: self.info.node_x[node2] as i32,
                    y2: self.info.node_y[node2] as i32,
                    amount: rank,
                })
                .collect()
        }
    }
    struct SolverImpl {
        info: Rc<Info>,
        state: Option<State>,
    }
    impl SolverImpl {
        fn search(&mut self, state: State) -> Option<State> {
            let mut state = state;
            if state.build_sure_links() {
                if state.is_valid() && state.is_solved() {
                    return Some(state);
                }
            }
            let mut moves = state.get_possible_moves();
            moves.sort_by_key(|(link_id, _)| self.info.link_length[*link_id]);
            for (link_id, amount) in moves {
                for spend in (1..=amount).rev() {
                    let mut state = state.dup();
                    if !state.add_link(link_id, spend) {
                        continue;
                    }
                    if !state.is_valid() {
                        continue;
                    }
                    if state.is_solved() {
                        return Some(state);
                    }
                    let searched = self.search(state);
                    if searched.is_some() {
                        return searched;
                    }
                }
            }
            None
        }
        fn parse_input(input: Vec<String>) -> (usize, usize, Vec<u8>) {
            let width: usize = input[0].parse().unwrap();
            let height: usize = input[1].parse().unwrap();
            let board_string = input[2..].join("");
            let board = board_string.as_bytes();
            assert_eq!(board.len(), width * height, "кривой размер буфера");
            (width, height, board.to_vec())
        }
        fn new() -> Self {
            Self {
                info: Rc::new(Info::empty()),
                state: None,
            }
        }
    }
    impl Solver for SolverImpl {
        fn solved(&self) -> bool {
            if let Some(state) = &self.state {
                state.is_solved()
            } else {
                false
            }
        }
        fn solve(&mut self, input: Vec<String>) -> Vec<crate::SolutionLink> {
            let (width, height, board) = SolverImpl::parse_input(input);
            self.info = Rc::new(Info::build_from(width, height, board));
            let state = State::initial_for_info(Rc::clone(&self.info));
            self.state = self.search(state);
            self.state.as_ref().unwrap().build_solution()
        }
    }
}
fn main() {
    let mut environment = cg_env::create_env();
    let input = environment.read();
    let mut solver = solver_main::create_solver();
    environment.write(solver.solve(input));
}

mod test {
    use crate::{solver_main, Environment};

    struct TestEnvironment {
        joined: String,
    }
    impl Environment for TestEnvironment {
        fn read(&mut self) -> Vec<String> {
            self.joined.split("\n").map(|s| s.to_string()).collect()
        }
        fn write(&mut self, _links: Vec<crate::SolutionLink>) {}
    }

    fn test(input: &str) {
        let mut environment = TestEnvironment {
            joined: input.to_string(),
        };
        let input = environment.read();
        let mut solver = solver_main::create_solver();
        let _result = solver.solve(input);
        assert!(solver.solved());
    }

    #[test]
    fn test1() {
        test("3\n3\n1.2\n...\n..1");
    }
    #[test]
    fn test3() {
        test("3\n3\n1.3\n...\n123");
    }
    #[test]
    fn test4() {
        test("4\n3\n14.3\n....\n.4.4");
    }
    #[test]
    fn test5() {
        test("5\n4\n4.544\n.2...\n..5.4\n332..");
    }
    #[test]
    fn test6() {
        test("7\n5\n2..2.1.\n.3..5.3\n.2.1...\n2...2..\n.1....2");
    }
    #[test]
    fn test7() {
        test("4\n4\n25.1\n47.4\n..1.\n3344");
    }
    #[test]
    fn test8() {
        test(
            "8\n8\n3.4.6.2.\n.1......\n..2.5..2\n1.......\n..1.....\n.3..52.3\n.2.17..4\n.4..51.2",
        );
    }
    #[test]
    fn test11() {
        test(
            "5\n14\n22221\n2....\n2....\n2....\n2....\n22321\n.....\n.....\n22321\n2....\n2....\n2.131\n2..2.\n2222.",
        );
    }
    #[test]
    fn test13() {
        test("23\n23\n3..2.2..1....3........4\n.2..1....2.6.........2.\n..3..6....3............\n.......2........1..3.3.\n..1.............3..3...\n.......3..3............\n.3...8.....8.........3.\n6.5.1...........1..3...\n............2..6.31..2.\n..4..4.................\n5..........7...7...3.3.\n.2..3..3..3............\n......2..2...1.6...3...\n....2..................\n.4....5...3............\n.................2.3...\n.......3.3..2.44....1..\n3...1.3.2.3............\n.2.....3...6.........5.\n................1......\n.1.......3.6.2...2...4.\n5...............3.....3\n4...................4.2");
    }
    #[test]
    fn test14() {
        test(
            r#"13
6
22221...22221
2.......2....
2.......2....
2.......2.131
2.......2..2.
22222...3222."#,
        );
    }
}
