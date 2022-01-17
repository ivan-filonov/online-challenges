use std::{io, collections::{VecDeque, HashSet}};

macro_rules! parse_input {
    ($x:expr, $t:ident) => ($x.trim().parse::<$t>().unwrap())
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
fn main() {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let inputs = input_line.split(" ").collect::<Vec<_>>();
    let num_nodes = parse_input!(inputs[0], i32); // the total number of nodes in the level, including the gateways
    let num_links = parse_input!(inputs[1], i32); // the number of links
    let num_exits = parse_input!(inputs[2], i32); // the number of exit gateways

    let mut graph : Vec<Vec<i32>> = (0..num_nodes)
        .map(|_|Vec::<i32>::new())
        .collect();
    let mut ranks = vec![0i32; num_nodes as usize];
    let mut exit_ids : Vec<Vec<i32>> = (0..num_nodes)
        .map(|_|Vec::<i32>::new())
        .collect();

    {
        let mut protograph = graph.clone();
        for _ in 0..num_links as usize {
            let mut input_line = String::new();
            io::stdin().read_line(&mut input_line).unwrap();
            let inputs = input_line.split(" ").collect::<Vec<_>>();
            let n1 = parse_input!(inputs[0], i32); // N1 and N2 defines a link between these nodes
            let n2 = parse_input!(inputs[1], i32);

            protograph[n1 as usize].push(n2);
            protograph[n2 as usize].push(n1);
        }

        let mut exits = HashSet::<i32>::new();
        for _ in 0..num_exits as usize {
            let mut input_line = String::new();
            io::stdin().read_line(&mut input_line).unwrap();
            let ei = parse_input!(input_line, i32); // the index of a gateway node
            exits.insert(ei);
        }

        for (from, links) in protograph.into_iter().enumerate() {
            if exits.contains(&(from as i32)) {
                continue;
            }
            for to in links {
                if exits.contains(&to) {
                    ranks[from] += 1;
                    exit_ids[from].push(to);
                } else {
                    graph[from].push(to);
                }
            }
        }
    }
    // game loop
    loop {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let si = parse_input!(input_line, i32); // The index of the node on which the Bobnet agent is positioned this turn

        let mut d = vec![i32::MAX; num_nodes as usize];
        let mut r = vec![0 as i32; num_nodes as usize];
        d[si as usize] = 0;
        let mut bag = VecDeque::<i32>::new();
        bag.push_back(si);

        while let Some(from) = bag.pop_front() {
            for &to in graph[from as usize].iter() {
                if d[to as usize] != i32::MAX {
                    continue;
                }
                d[to as usize] = d[from as usize] + 1;
                r[to as usize] = r[from as usize] + ranks[to as usize];
                bag.push_back(to);
            }
        }

        let mut x : Option<usize> = None;
        if ranks[si as usize] == 1 {
            x = Some(si as usize);
            let xx = x.unwrap();
            eprintln!("uncoditional point {} (rank {}, path r {}, d {})", xx, ranks[xx], r[xx],d[xx]);
        } else {
            let max_rank = ranks.iter().max().unwrap();
            for (index, (&dist, &rank)) in d.iter().zip(r.iter()).enumerate() {
                if ranks[index] == 0 {
                    continue;
                }
                match x {
                    None => {
                        eprintln!("initial point {} (rank {}, path r {}, d {})", index, ranks[index], rank, dist);
                        x = Some(index);
                    },
                    Some(xx) => {
                        if rank - dist > r[xx] - d[xx] {
                            eprintln!("{} (rank {}, path r {}, d {}) replaced (1) with {} (rank {}, path r {}, d {})", xx, ranks[xx], r[xx],d[xx], index,ranks[index],rank, dist);
                            x = Some(index);
                        } else if (rank - dist == r[xx] - d[xx]) && (dist < d[xx]) {
                            eprintln!("{} (rank {}, path r {}, d {}) replaced (2) with {} (rank {}, path r {}, d {})", xx, ranks[xx], r[xx],d[xx], index,ranks[index],rank, dist);
                            x = Some(index);
                        }
                    }
                }            
            }
        }

        let x = x.unwrap();
        ranks[x] -= 1;
        let y = exit_ids[x].pop();


        // Example: 3 4 are the indices of the nodes you wish to sever the link between
        println!("{} {}", x, y.unwrap());
    }
}
