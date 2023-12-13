//let input = File::read_to_string("sample.txt");
let input = File::read_to_string("input.txt");
let input = input.unwrap().trim().split("\n");

let input = parse_input(input);

print(f"part1: {part1(input.clone())}");
print(f"part2: {part2(input)}");

fn part1(rows: List<Row>) -> int {
    let mut sum = 0;
    for i in List::range(0, rows.len()) {
        let mut row = rows.get(i).unwrap();
        print(f"doing {i:04}: {row.to_string()}");
        let res = brute_force_row(int::min_value(), row.broken.remove(0).unwrap(), 0, row);
//        let res = brute_force_row(1, row.broken.remove(0).unwrap(), 0, row);
        print(res);
        sum += res;
    }
    sum
}

// brute-force definitely doesn't work where
// after 10 hours I have 4/1000 solutions.
fn part2(rows: List<Row>) -> int {
    let mut sum = 0;
    for i in List::range(0, rows.len()) {
        let mut row = rows.get(i).unwrap().expand();
        print(f"doing {i:04}: {row.to_string()}");
        let res = brute_force_row(int::min_value(), row.broken.remove(0).unwrap(), 0, row);
//        let res = brute_force_row(1, row.broken.remove(0).unwrap(), 0, row);
        print(res);
        sum += res;
    }
    sum
}

fn brute_force_row(depth: int, broken: int, mut index: int, row: Row) -> int {
    if depth > 0 { print(f"{indent(depth)}brute-force {broken:2}: {row.to_string()}"); }
    if broken == 0 {
        // after a broken field must be a space (Operational or Unknown)
        if row.springs.get(index) == Option::Some(Spring::Broken) {
        if depth > 0 { print(f"{indent(depth+1)}MISS: no space after broken"); }
            return 0;
        }
        // we can't have any more broken springs
        if row.broken.is_empty() {
            let mut valid_solution = true;
            for i in List::range(index, row.springs.len()) {
                valid_solution &= row.springs.get(i).unwrap() != Spring::Broken;
            }
            if valid_solution {
                if depth > 0 { print(f"{indent(depth+1)}FOUND {row.to_string()}"); }
            } else {
                if depth > 0 { print(f"{indent(depth+1)}MISS: broken found with none left") }
            }
            return valid_solution.to_int();
        }
        // we have more broken springs, continue after non-broken spring
        let row = row.fix_spring(index);
        let res = brute_force_row(depth + 1, row.broken.remove(0).unwrap(), index + 1, row);
        if depth > 0 { print(f"{indent(depth)}{res}") }
        return res;
    }

    let within_broken_block = row.springs.get(index - 1) == Option::Some(Spring::Broken);
    while index < row.springs.len() {
        let current = row.springs.get(index).unwrap();
        // if we are in a block but encounter an operational spring, it's over
        if within_broken_block && current == Spring::Operational {
            if depth > 0 { print(f"{indent(depth+1)}MISS: broken block not full") }
            return 0;
        }
        // we must start / continue here
        if current == Spring::Broken || within_broken_block {
            return brute_force_row(depth + 1, broken - 1, index + 1, row.break_spring(index));
        }
        // skip
        if current == Spring::Operational {
            index += 1;
            continue;
        }

        // we can start here or go next
        assert_eq(current, Spring::Unknown);
        if depth > 0 { print(f"{indent(depth)}start_here"); }
        let start_here = brute_force_row(depth + 1, broken - 1, index + 1, row.break_spring(index));
        // if we aren't within a broken-block
        let go_next = if !within_broken_block {
            if depth > 0 { print(f"{indent(depth)}go_next"); }
            brute_force_row(depth + 1, broken, index + 1, row.fix_spring(index))
        } else {
            0
        };

        if depth > 0 { print(f"{indent(depth)}start_here got {start_here}") }
        if depth > 0 { print(f"{indent(depth)}go_next got {go_next}") }
        let res = start_here + go_next;
        if depth > 0 { print(f"{indent(depth)}total {res}") }
        return res;
    }
    0
}

struct Row {
    springs: List<Spring>,
    broken: List<int>,
}
impl Row {
    fn expand(self) -> Row {
        let mut springs = List::new();
        for i in List::range(0, 5) {
            springs.extend(self.springs);
            springs.push(Spring::Unknown);
        }
        assert_eq(springs.pop().unwrap(), Spring::Unknown);
        Row {
            springs: springs,
            broken: self.broken.repeat(5),
        }
    }
    fn to_string(self) -> string {
        let mut s = "";
        for spring in self.springs {
            s = f"{s}{spring.to_string()}";
        }
        s = f"{s} ";
        for broken in self.broken {
            s = f"{s}{broken},";
        }
        s
    }
    fn break_spring(self, index: int) -> Row {
        assert(self.springs.get(index) != Option::Some(Spring::Operational));
        let mut clone = self.clone();
        clone.springs.set(index, Spring::Broken);
        clone
    }
    fn fix_spring(self, index: int) -> Row {
        assert(self.springs.get(index) != Option::Some(Spring::Broken));
        let mut clone = self.clone();
        clone.springs.set(index, Spring::Operational);
        clone
    }
}
enum Spring {
    Operational,
    Broken,
    Unknown,
}
impl Spring {
    fn to_string(self) -> string {
        match self {
            Spring::Operational => ".",
            Spring::Broken => "#",
            Spring::Unknown => "?",
        }
    }
}

fn parse_input(input: List<string>) -> List<Row> {
    let mut rows = List::new();
    for line in input {
        rows.push(parse_row(line));
    }
    rows
}

fn parse_row(row: string) -> Row {
    let parts = row.split(" ");
    let mut springs = List::new();
    for spring in parts.get(0).unwrap().chars_utf32() {
        springs.push(parse_spring(spring));
    }
    let mut broken = List::new();
    for b in parts.get(1).unwrap().split(",") {
        broken.push(b.parse_int().unwrap());
    }
    Row { springs: springs, broken: broken }
}
fn parse_spring(c: string) -> Spring {
    match c {
        "." => Spring::Operational,
        "#" => Spring::Broken,
        "?" => Spring::Unknown,
        _ => panic(f"unknown field {c}"),
    }
}
fn indent(depth: int) -> string {
    let s = "    ";
    s.repeat(depth)
}
