use std::io;

mod table_data {
    use std::collections::HashMap;

    const TABLE : &str = "H He Li Be B C N O F Ne Na Mg Al Si P S Cl Ar K Ca Sc Ti V Cr Mn Fe Co Ni Cu Zn Ga Ge As Se Br Kr Rb Sr Y Zr Nb Mo Tc Ru Rh Pd Ag Cd In Sn Sb Te I Xe Cs Ba La Ce Pr Nd Pm Sm Eu Gd Tb Dy Ho Er Tm Yb Lu Hf Ta W Re Os Ir Pt Au Hg Tl Pb Bi Po At Rn Fr Ra Ac Th Pa U Np Pu Am Cm Bk Cf Es Fm Md No Lr Rf Db Sg Bh Hs Mt Ds Rg Cn Nh Fl Mc Lv Ts Og";
    pub struct Data {
        items: HashMap<String, String>,
    }
    impl Data {
        pub fn new() -> Self {
            let items: Vec<_> = TABLE.split_whitespace().collect();
            let mut itemsMap = HashMap::new();
            for item in items {
                itemsMap.insert(item.to_lowercase(), item.to_string());
            }

            Self { items: itemsMap }
        }

        pub fn contains(&self, key: &str) -> bool {
            self.items.contains_key(key)
        }

        pub fn get(&self, key: &str) -> String {
            self.items.get(key).unwrap().to_string()
        }
    }
}

struct Solver {
    table_data: table_data::Data,
    word: String,
    answer: Vec<String>,
}

impl Solver {
    fn new() -> Self {
        Self {
            table_data: table_data::Data::new(),
            word: String::new(),
            answer: Vec::new(),
        }
    }
    fn search(&mut self, prefix: String) {
        // dbg!(&prefix);
        if prefix.len() == self.word.len() {
            self.answer.push(prefix);
            return;
        }
        for n in 1..=2 {
            if prefix.len() + n > self.word.len() {
                continue;
            }
            let key = self.word[prefix.len()..prefix.len() + n].to_string();
            // dbg!(&key);
            if !self.table_data.contains(&key) {
                continue;
            }
            // dbg!(&self.table_data.get(&key));
            self.search(prefix.clone() + &self.table_data.get(&key));
        }
    }
    fn solve(&mut self, word: String) -> Vec<String> {
        self.word = word.to_lowercase();
        self.search("".to_string());
        self.answer.sort();
        self.answer.clone()
    }
}

fn main() {
    let mut line = String::new();
    io::stdin().read_line(&mut line).unwrap();
    let word = line.trim().to_lowercase();
    let mut solver = Solver::new();
    let answer = solver.solve(word);
    if answer.is_empty() {
        println!("none");
    }
    for item in answer.iter().map(|s| s.to_string()) {
        println!("{}", item);
    }
}

mod test {
    use crate::*;

    fn test(name: &str, expected: &[&str]) {
        let mut solver = Solver::new();
        let answer = solver.solve(String::from(name));
        assert_eq!(answer.len(), expected.len());
        for (i, j) in answer.iter().enumerate() {
            assert_eq!(
                expected[i], j,
                "wrong answer at index {} ({} vs. {})",
                i, expected[i], j
            );
        }
    }

    #[test]
    fn test1() {
        test("Carbon", &["CArBON", "CaRbON"]);
    }
}
