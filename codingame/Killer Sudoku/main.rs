use std::io::{self, BufReader, BufWriter, Read, Write};

const ECHO: bool = false;
const DEBUG: bool = true;

trait SolverState {
    fn read(&mut self, reader: &mut BufReader<&mut dyn Read>);
    fn write(&self, writer: &mut BufWriter<&mut dyn Write>);
    fn solve(&mut self) -> bool;
}

mod v2 {
    use crate::{SolverState, DEBUG, ECHO};
    use std::{
        cell::{Ref, RefCell},
        collections::HashSet,
        io::{BufRead, BufReader, BufWriter, Read, Write},
        num,
        rc::Rc,
    };

    struct Cell {
        id: i32,
        name: String,
        value: Option<i32>,
        mask_avl: i32,
        groups: [Rc<RefCell<Group>>; 4],
    }

    struct Group {
        id: i32,
        name: String,
        value: i32,
        sum: i32,
        zeros: i32,
        mask_used: i32,
        mask_avl: i32,
        cells: Vec<Rc<RefCell<Cell>>>,
    }

    impl Group {
        fn preallloc() -> Vec<Rc<RefCell<Group>>> {
            let mut data = Vec::new();

            for name_fmt in ["row", "col", "box"] {
                for i in 0..9 {
                    let g = Group {
                        id: data.len() as i32,
                        name: format!("{}_{}", name_fmt, i + 1),
                        value: 45,
                        sum: 0,
                        zeros: 0,
                        mask_used: 0,
                        mask_avl: 0x3fe,
                        cells: Vec::new(),
                    };
                    data.push(Rc::new(RefCell::new(g)));
                }
            }
            for c in 0..(85 - 27) {
                let g = Group {
                    id: data.len() as i32,
                    name: format!("cage_{}", (c as u8 + b'A') as char),
                    value: 0,
                    sum: 0,
                    zeros: 0,
                    mask_used: 0,
                    mask_avl: 0x3fe,
                    cells: Vec::new(),
                };
                data.push(Rc::new(RefCell::new(g)));
            }

            data
        }
    }

    impl Cell {
        fn put(&mut self, value: i32) {
            assert!(self.value.is_none());
            assert!(self.mask_avl & (1 << value) != 0);
            self.value = Some(value);
            self.mask_avl = 0;
            let bit = 1 << value;
            for g in &self.groups {
                let mut group = g.borrow_mut();
                assert!(
                    group.mask_used & bit == 0,
                    "setting cell {} to {} is prohibited by group {} mask_used {:10b}",
                    self.name,
                    value,
                    group.name,
                    group.mask_avl
                );
                group.mask_used |= bit;
                assert!(
                    group.mask_avl & bit != 0,
                    "setting cell {} to {} is prohibited by group {} mask_avl {:10b}",
                    self.name,
                    value,
                    group.name,
                    group.mask_avl
                );
                group.mask_avl &= !bit;
                group.sum += value;
                assert!(group.sum <= group.value);
                group.zeros -= 1;
                if group.zeros == 0 {
                    assert!(group.sum == group.value);
                }
            }
        }

        fn preallloc(groups: &Vec<Rc<RefCell<Group>>>) -> Vec<Rc<RefCell<Cell>>> {
            let mut cells = Vec::new();
            for y in 0..9 {
                for x in 0..9 {
                    let g = (y / 3) * 3 + x / 3;
                    let cell = Cell {
                        id: (y * 9 + x) as i32,
                        name: format!("{}{}", (y as u8 + b'A') as char, x + 1),
                        value: None,
                        mask_avl: 0x3fe,
                        groups: [
                            Rc::clone(&groups[y + 0]),
                            Rc::clone(&groups[x + 9]),
                            Rc::clone(&groups[g + 9 * 2]),
                            Rc::clone(&groups[0 + 9 * 3]),
                        ],
                    };
                    cells.push(Rc::new(RefCell::new(cell)));
                }
            }
            cells
        }
    }

    pub struct State {
        sum_masks: [[i32; 46]; 10],
        groups: Vec<Rc<RefCell<Group>>>,
        cells: Vec<Rc<RefCell<Cell>>>,
    }

    impl State {
        pub fn new() -> Self {
            let mut sum_masks: [[i32; 46]; 10] = [[0; 46]; 10];
            for m in (2..=0x3fe as i32).step_by(2) {
                let size = m.count_ones();
                let sum: usize = (1..=9)
                    .filter_map(|x| {
                        let bit = 1 << x;
                        if m & bit != 0 {
                            Some(x)
                        } else {
                            None
                        }
                    })
                    .sum();
                sum_masks[size as usize][sum] |= m;
            }
            let groups = Group::preallloc();
            let cells = Cell::preallloc(&groups);
            Self {
                sum_masks,
                groups,
                cells,
            }
        }

        fn after_read(&mut self) {
            for g in &self.groups {
                let mut group = g.borrow_mut();
                if group.cells.is_empty() {
                    continue;
                }
                group.after_read();
                group.mask_avl &=
                    self.sum_masks[group.zeros as usize][(group.value - group.sum) as usize];
                assert!(
                    group.mask_avl.count_ones() >= group.zeros as u32,
                    "mask for group {} is too restrictive: {:10b}",
                    group.name,
                    group.mask_avl
                );
            }
        }
    }

    impl Group {
        fn after_read(&mut self) {
            for cell_ref in &self.cells {
                let cell: Ref<Cell> = cell_ref.borrow();
                if let Some(value) = cell.value {
                    self.sum += value;
                    let bit = 1 << value;
                    self.mask_used |= bit;
                    self.mask_avl &= !bit;
                } else {
                    self.zeros += 1;
                }
            }
        }
    }

    impl State {
        fn update_masks(&mut self) -> bool {
            let mut g_changed = 0;
            for g in &self.groups {
                let mut group = g.borrow_mut();
                if group.zeros == 0 {
                    continue;
                }
                let old_mask = group.mask_avl;
                group.mask_avl &=
                    self.sum_masks[group.zeros as usize][(group.value - group.sum) as usize];
                assert!(
                    group.mask_avl.count_ones() >= group.zeros as u32,
                    "mask for group {} is too restrictive: {:10b}",
                    group.name,
                    group.mask_avl
                );
                if old_mask != group.mask_avl {
                    g_changed += 1;
                }
            }

            let mut c_changed = 0;
            for c in &self.cells {
                let mut cell = c.borrow_mut();
                if cell.value.is_some() {
                    continue;
                }
                let old_mask = cell.mask_avl;
                let mask_update = cell
                    .groups
                    .iter()
                    .map(|g| g.borrow().mask_avl)
                    .fold(cell.mask_avl, |x, y| x & y);
                cell.mask_avl = mask_update;
                assert!(mask_update != 0, "cell {} mask updated to 0", cell.name);
                if mask_update != old_mask {
                    c_changed += 1;
                }
            }

            if DEBUG && (g_changed + c_changed) > 0 {
                if g_changed > 0 && c_changed > 0 {
                    eprintln!(
                        "updated {} group masks and {} cell masks",
                        g_changed, c_changed
                    );
                } else if c_changed > 0 {
                    eprintln!("updated {} cell masks", c_changed);
                } else if g_changed > 0 {
                    eprintln!("updated {} group masks", g_changed);
                }
            }
            (g_changed + c_changed) > 0
        }

        fn update_single(&mut self) -> bool {
            for c in &self.cells {
                let mut cell = c.borrow_mut();
                if cell.value.is_some() {
                    continue;
                }
                if cell.mask_avl.count_ones() == 1 {
                    let mut v = 1;
                    while cell.mask_avl & (1 << v) == 0 {
                        v += 1;
                    }
                    if DEBUG {
                        eprintln!("update single-bit cell {} to {}", cell.name, v);
                    }
                    cell.put(v);
                    return true;
                }
            }

            false
        }

        fn update_hidden_single(&mut self) -> bool {
            for g in &self.groups {
                let group = g.borrow();
                if group.zeros < 2 || group.value != 45 {
                    continue;
                }
                let masks: Vec<_> = group.cells.iter().map(|c| c.borrow().mask_avl).collect();
                let mut bit_counts = [0i32; 10];
                let mut bit_seen = [0usize; 10];
                for (index, &mask) in masks.iter().enumerate() {
                    for v in 1..=9 {
                        let bit = 1 << v;
                        if mask & bit != 0 {
                            bit_counts[v] += 1;
                            bit_seen[v] = index;
                        }
                    }
                }

                for i in 1..=9 {
                    if bit_counts[i] == 1 {
                        let value = i as i32;
                        let cell_ref = Rc::clone(&group.cells[bit_seen[i]]);
                        let mut cell = cell_ref.borrow_mut();
                        if DEBUG {
                            eprintln!(
                                "update hidden single {} in group {} to {}",
                                cell.name, group.name, value
                            );
                        }
                        drop(group);
                        cell.put(value);
                        return true;
                    }
                }
            }
            false
        }

        fn update_pair(&mut self) -> bool {
            for g in &self.groups {
                let group = g.borrow_mut();
                if group.zeros < 3 {
                    continue;
                }
                let masks: Vec<_> = group
                    .cells
                    .iter()
                    .filter_map(|c| {
                        let cell = c.borrow();
                        if cell.value.is_none() {
                            Some(cell.mask_avl)
                        } else {
                            None
                        }
                    })
                    .collect();

                assert!(masks.len() > 2);
                let mut pair = None;
                for i0 in 0..masks.len() - 1 {
                    if masks[i0].count_ones() != 2 {
                        continue;
                    }
                    for i1 in i0 + 1..masks.len() {
                        if masks[i0] == masks[i1] {
                            pair = Some(masks[i0]);
                        }
                    }
                }

                if let Some(mask) = pair {
                    if !masks.iter().any(|&x| x != mask && x & mask != 0) {
                        continue;
                    }
                    let mut changed = true;
                    for c in &group.cells {
                        let mut cell = c.borrow_mut();
                        if cell.value.is_some() {
                            continue;
                        }
                        if cell.mask_avl != mask {
                            let old_mask = cell.mask_avl;
                            cell.mask_avl &= !mask;
                            if cell.mask_avl != old_mask {
                                changed = true;
                            }
                        }
                    }

                    if changed {
                        if DEBUG {
                            eprintln!("update pair in group {} with mask {:10b}", group.name, mask);
                        }
                        return true;
                    }
                }
            }
            false
        }

        fn update_triple(&mut self) -> bool {
            for g in &self.groups {
                let group = g.borrow();
                if group.zeros < 4 || group.value != 45 {
                    continue;
                }
                let masks = group
                    .cells
                    .iter()
                    .filter_map(|c| {
                        let cell = c.borrow();
                        if cell.value.is_none() {
                            Some(cell.mask_avl)
                        } else {
                            None
                        }
                    })
                    .collect::<Vec<_>>();
                if masks.len() < 4 {
                    continue;
                }

                let mut triple = None;
                'search: for i in 1..=7 {
                    let mask = 1 << i;
                    for j in i + 1..=8 {
                        let mask = mask | (1 << j);
                        for k in j + 1..=9 {
                            let mask = mask | (1 << k);
                            let mut exact = 0;
                            let mut other = 0;
                            for &m in &masks {
                                if m & !mask == 0 {
                                    exact += 1;
                                } else if m & mask != 0 {
                                    other += 1;
                                }
                            }

                            if exact == 3 && other > 0 {
                                if DEBUG {
                                    eprintln!(
                                        "candidate triple in group {}: {}/{}/{}",
                                        group.name, i, j, k
                                    );
                                }
                                triple = Some(mask);
                                break 'search;
                            }
                        }
                    }
                }

                if let Some(mask) = triple {
                    let mut changed = false;
                    for c in &group.cells {
                        let mut cell = c.borrow_mut();
                        if cell.value.is_some() {
                            continue;
                        }
                        if cell.mask_avl & !mask == 0 {
                            continue;
                        }
                        let old_mask = cell.mask_avl;
                        cell.mask_avl &= !mask;
                        if cell.mask_avl != old_mask {
                            changed = true;
                        }
                    }
                    return changed;
                }
            }
            false
        }

        fn get_group_cells_set(&self, group_id: i32) -> HashSet<i32> {
            self.groups[group_id as usize]
                .borrow()
                .cells
                .iter()
                .map(|c| c.borrow().id)
                .collect()
        }

        fn cell_set_to_cage_set(&self, cells: &HashSet<i32>) -> HashSet<i32> {
            cells
                .iter()
                .map(|&c| self.cells[c as usize].borrow().groups[3].borrow().id)
                .collect()
        }

        fn get_cell_set_values(&self, cells: &HashSet<i32>) -> Vec<(i32, Option<i32>)> {
            cells
                .iter()
                .map(|&id| {
                    let cell = self.cells[id as usize].borrow();
                    assert!(id == cell.id);
                    (cell.id, cell.value)
                })
                .collect()
        }

        fn get_group_value(&self, id: i32) -> i32 {
            let group = self.groups[id as usize].borrow();
            assert!(group.id == id);
            group.value
        }

        fn get_group_name(&self, id: i32) -> String {
            let group = self.groups[id as usize].borrow();
            assert!(group.id == id);
            group.name.clone()
        }

        fn update_cell_mask(&self, cell_id: i32, mask: i32) -> bool {
            let mut cell = self.cells[cell_id as usize].borrow_mut();
            assert!(cell_id == cell.id);
            let old_mask = cell.mask_avl;
            cell.mask_avl &= mask;
            cell.mask_avl != old_mask
        }

        fn update_single_block(&mut self) -> bool {
            for block_index in 0..27 {
                let block = self.groups[block_index].borrow();
                if block.zeros == 0 {
                    continue;
                }
                if DEBUG {
                    eprintln!("scanning block {}", block.name);
                }
                drop(block);

                let block_cell_ids = self.get_group_cells_set(block_index as i32);
                let cage_ids = self.cell_set_to_cage_set(&block_cell_ids);

                let mut block_cells_with_sum = HashSet::new();
                let mut block_cells_known_sum = 0;
                for cage_id in cage_ids {
                    let cage_cell_ids = self.get_group_cells_set(cage_id);
                    let cage_cell_values = self.get_cell_set_values(&cage_cell_ids);
                    if cage_cell_values
                        .iter()
                        .all(|(id, value)| block_cell_ids.contains(id) || value.is_some())
                    {
                        // all empty cells are inside block, so we know their exact sum
                        if DEBUG {
                            eprintln!(
                                "all empty cells of {} are inside block",
                                self.get_group_name(cage_id)
                            );
                        }
                        let mut sum = self.get_group_value(cage_id);
                        for (cell_id, cell_value) in cage_cell_values {
                            if !block_cell_ids.contains(&cell_id) {
                                assert!(cell_value.is_some());
                                sum -= cell_value.unwrap();
                            } else {
                                block_cells_with_sum.insert(cell_id);
                            }
                        }
                        block_cells_known_sum += sum;
                    }
                }
                if block_cells_with_sum.is_empty() {
                    continue;
                }

                let block_cells_without_sum = &block_cell_ids - &block_cells_with_sum;
                if block_cells_without_sum.is_empty() {
                    continue;
                }

                let mut num_changed = 0;
                let sum_other = 45 - block_cells_known_sum;
                let mask = self.sum_masks[block_cells_without_sum.len()][sum_other as usize];
                for &cell_id in &block_cells_without_sum {
                    if self.update_cell_mask(cell_id, mask) {
                        num_changed += 1;
                    }
                }
                if num_changed > 0 {
                    if DEBUG {
                        eprintln!(
                            "updated {} cells with mask {:10b}",
                            block_cells_without_sum.len(),
                            mask
                        );
                    }
                    return true;
                }
            }
            false
        }
    }

    impl SolverState for State {
        fn solve(&mut self) -> bool {
            while self.update_masks()
                || self.update_single()
                || self.update_pair()
                || self.update_hidden_single()
                || self.update_triple()
                || self.update_single_block()
            {}

            true
        }

        fn read(&mut self, reader: &mut BufReader<&mut dyn Read>) {
            for y in 0..9 {
                let mut buf = String::new();
                reader.read_line(&mut buf).unwrap();
                if ECHO {
                    eprintln!("{} :{}", buf.trim_end(), y);
                }
                let buf = buf.as_bytes();
                for x in 0..9 {
                    let cell = y * 9 + x;
                    let value = if buf[x] == b'.' { 0 } else { buf[x] - b'0' };
                    let cage = buf[x + 10] - b'A';
                    if cage > 57 {
                        panic!("failed processing cage char '{}'", buf[x + 10] as char);
                    }

                    let cell_rc = &self.cells[cell];
                    let mut cell = cell_rc.borrow_mut();
                    cell.groups[3] = Rc::clone(&self.groups[cage as usize + 27]);

                    if value > 0 {
                        cell.value = Some(value as i32);
                        cell.mask_avl = 0;
                    } else {
                        cell.value = None;
                        cell.mask_avl = 0x3fe;
                    }
                    for g in &mut cell.groups {
                        g.borrow_mut().cells.push(Rc::clone(cell_rc));
                    }
                }
            }
            let mut buf = String::new();
            reader.read_line(&mut buf).unwrap();
            if ECHO {
                eprint!("{}", buf);
            }
            for cage_def in buf.trim().split(" ") {
                let cage = cage_def.as_bytes()[0] - b'A';
                let value: i32 = cage_def[2..].parse().unwrap();

                let mut group = self.groups[cage as usize + 27].borrow_mut();
                group.value = value;
            }

            self.after_read();
        }

        fn write(&self, writer: &mut BufWriter<&mut dyn Write>) {
            for i in 0..81 {
                if i > 0 && i % 9 == 0 {
                    writeln!(writer).unwrap();
                }
                let v = self.cells[i].borrow().value;
                let c = match v {
                    Some(value) => value as u8 + b'0',
                    None => b'.',
                };
                write!(writer, "{}", c as char).unwrap();
            }
        }
    }
}

fn solve(reader: &mut dyn Read, writer: &mut dyn Write) {
    use v2::State;
    let mut reader = BufReader::new(reader);
    let mut writer = BufWriter::new(writer);

    let state: &mut dyn SolverState = &mut State::new();
    state.read(&mut reader);
    assert!(state.solve(), "expected input is solvable");
    state.write(&mut writer);
}

fn main() {
    let mut streams = (io::stdin(), io::stdout());

    solve(&mut streams.0, &mut streams.1);
}

mod test {
    #[test]
    fn test1() {
        let mut t_in = r#"56..1..2. aabbccdde
..72..68. afghhiide
..2.87.15 jfggklmme
......3.9 jjgnklopp
.7....2.. qqgnooorr
9.634.8.. stuuvwwxx
2.9..8... stuuvvwyz
..41.2... sAuuByyyz
.8.4...3. CADDBEEFF
a=12 b=17 c=4 d=14 e=15 f=13 g=19 h=7 i=10 j=16 k=10 l=13 m=10 n=15 o=15 p=13 q=11 r=11 s=18 t=3 u=28 v=15 w=20 x=8 y=22 z=12 A=11 B=13 C=6 D=9 E=10 F=5"#
            .as_bytes();
        let t_out = r#"568913427
197254683
342687915
851726349
473891256
926345871
219538764
734162598
685479132"#;
        let mut out = Vec::new();
        crate::solve(&mut t_in, &mut out);
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out.trim());
    }

    #[test]
    fn test2() {
        let mut t_in = r#"........1 abbcdddef
....5.... agccchfff
...3..... ijjjhhkkl
.......9. ijmmnnokp
..6...... qrrsttuvv
....9.7.. qwxsyuuzA
.....3... qwxxyBBCA
......... qDxEEBBCC
..8..5... DDFFFBGHH
a=14 b=9 c=17 d=22 e=3 f=20 g=3 h=9 i=6 j=21 k=19 l=7 m=9 n=11 o=4 p=6 q=16 r=15 s=11 t=11 u=10 v=9 w=9 x=20 y=16 z=5 A=5 B=22 C=19 D=17 E=11 F=15 G=3 H=11"#
            .as_bytes();
        let t_out = r#"672489531
831752649
549316827
157238496
396547218
284691753
415873962
763924185
928165374"#;
        let mut out = Vec::new();
        crate::solve(&mut t_in, &mut out);
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out.trim());
    }

    #[test]
    fn test3() {
        let mut t_in = r#"......... abbcccddd
......... aabefghii
......... jkkefghil
......... jmnnnooil
......... mmmppqqrl
......... sttpuuqrr
......... svwwwxxxy
......... zvvABBCCy
......... zDDAAEEFF
a=7 b=23 c=13 d=13 e=5 f=10 g=17 h=11 i=28 j=12 k=8 l=16 m=20 n=9 o=11 p=22 q=14 r=8 s=15 t=13 u=4 v=17 w=11 x=16 y=8 z=4 A=19 B=10 C=7 D=13 E=15 F=6"#
            .as_bytes();
        let t_out = r#"496175238
218369547
753248691
531627489
827594316
649813752
962451873
374982165
185736924"#;
        let mut out = Vec::new();
        crate::solve(&mut t_in, &mut out);
        assert_eq!(String::from_utf8(out).unwrap().trim(), t_out.trim());
    }
}
