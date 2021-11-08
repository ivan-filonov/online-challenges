use std::io::prelude::*;
use std::io::{BufReader, BufWriter, Read, Write};

const INF: i32 = i32::MAX / 4;
#[derive(Clone)]
struct GraphEdges {
    N: usize,
    edges: Vec<i32>,
}

impl GraphEdges {
    fn new(num_vertices: usize) -> GraphEdges {
        let mut v: Vec<i32> = Vec::new();
        v.resize(num_vertices * num_vertices, INF);
        GraphEdges {
            N: num_vertices,
            edges: v,
        }
    }
    fn get(&self, i: usize, j: usize) -> i32 {
        self.edges[i + j * self.N]
    }
    fn at_mut(&mut self, i: usize, j: usize) -> &mut i32 {
        &mut self.edges[i + j * self.N]
    }
    fn set(&mut self, i: usize, j: usize, d: i32) {
        self.edges[i + j * self.N] = d;
    }
    fn set2(&mut self, i: usize, j: usize, d: i32) {
        self.set(i, j, d);
        self.set(j, i, d);
    }
    fn fill(&mut self, val: i32) {
        for i in self.edges.iter_mut() {
            *i = val;
        }
    }
}

const NO_SOLUTION: &str = "No solution.";
fn solve(input: &mut dyn Read, writer: &mut dyn Write) {
    let reader = BufReader::new(input);
    let mut writer = BufWriter::new(writer);
    let mut lines = reader.lines();
    loop {
        let header = lines.next().unwrap().unwrap();
        if "-1" == header {
            break;
        }
        // read graph
        let mut header_iter = header.split_ascii_whitespace().map(|s| s.parse::<usize>());
        let num_vertices = header_iter.next().unwrap().unwrap();
        let N = num_vertices;
        let num_edges = header_iter.next().unwrap().unwrap();
        let mut graph = GraphEdges::new(num_vertices);
        for _ in 0..num_edges {
            let edge_def = lines.next().unwrap().unwrap();
            let mut edge_iter = edge_def.split_ascii_whitespace().map(|s| s.parse::<i32>());
            let vertex_u = edge_iter.next().unwrap().unwrap() - 1;
            let vertex_v = edge_iter.next().unwrap().unwrap() - 1;
            let dist = edge_iter.next().unwrap().unwrap();
            if !(0..N).contains(&(vertex_u as usize)) || !(0..N).contains(&(vertex_v as usize)) {
                continue;
            }
            // use vertex_u vertex_v dist
            if dist < graph.get(vertex_u as usize, vertex_v as usize) {
                graph.set2(vertex_u as usize, vertex_v as usize, dist);
            }
        }
        // find best edge
        let mut routes = GraphEdges::new(num_vertices);
        let mut best_route_u: usize = 0;
        let mut best_route_v: usize = 0;
        let mut best_dist = INF;
        for i in 0..N {
            routes.set(i, i, 0);
            // попытаться обновить лучший цикл
            for j in 0..i {
                for k in 0..i {
                    if j == k {
                        continue;
                    }
                    let new_dist = graph.get(i, j) + graph.get(i, k) + routes.get(j, k);
                    if new_dist < best_dist {
                        best_dist = new_dist;
                        best_route_u = i;
                        best_route_v = j;
                    }
                }
            }
            // попытаться добавить ребрa к пути
            for j in 0..i {
                routes.set(i, j, INF);
                // routes[i,j]=drop
                for k in 0..i {
                    let j_k_plus_k_i = routes.get(j, k) + graph.get(k, i);
                    if routes.get(i, j) > j_k_plus_k_i {
                        routes.set(i, j, j_k_plus_k_i);
                    }
                }
                routes.set(j, i, routes.get(i, j));
            }
            // попытаться склеить пути
            for j in 0..i {
                for k in 0..i {
                    let j_i_plust_i_k = routes.get(j, i) + routes.get(i, k);
                    if routes.get(j, k) > j_i_plust_i_k {
                        routes.set(j, k, j_i_plust_i_k);
                    }
                }
            }
        }
        if best_dist == INF {
            writeln!(writer, "{}", NO_SOLUTION);
            continue;
        }
        graph.set2(best_route_u, best_route_v, INF);
        routes = graph.clone();

        // floyd_warshall_on_routes on routes
        for i in 0..N {
            routes.set(i, i, 0);
        }
        for i in 0..N {
            for j in 0..N {
                for k in 0..N {
                    let j_i_plus_i_k = routes.get(j, i) + routes.get(i, k);
                    if routes.get(j, k) > j_i_plus_i_k {
                        routes.set(j, k, j_i_plus_i_k);
                    }
                }
            }
        }
        // build solution
        let mut solution: Vec<usize> = Vec::new();
        let u = best_route_u;
        let mut v = best_route_v;
        while u != v {
            solution.push(v + 1);
            for i in 0..N {
                if routes.get(v, u) == routes.get(i, u) + graph.get(v, i) {
                    v = i;
                    break;
                }
            }
        }
        solution.push(v + 1);
        // output solution
        for (n, v) in solution.iter().enumerate() {
            if n > 0 {
                write!(writer, " ");
            }
            write!(writer, "{}", v);
        }
        writeln!(writer, "");
    }
}

fn main() {
    solve(&mut std::io::stdin(), &mut std::io::stdout());
}

mod tests {
    #[test]
    fn basic_test() {
        use crate::solve;
        let mut buf_in = r#"5 7
1 4 1
1 3 300
3 1 10
1 2 16
2 3 100
2 5 15
5 3 20
4 6
1 2 40
1 3 50
1 40 60
2 3 10
2 4 30
3 4 20
4 3
1 2 10
1 3 20
1 4 30
-1
"#
        .as_bytes();
        let mut buf_out: Vec<u8> = Vec::new();
        solve(&mut buf_in, &mut buf_out);
        let output = String::from_utf8(buf_out).expect("valid utf8 string");
        assert_eq!(
            output,
            r#"1 3 5 2
2 3 4
No solution.
"#
        );
    }
}
