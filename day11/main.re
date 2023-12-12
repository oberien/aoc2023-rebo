//let input = File::read_to_string("sample.txt");
let input = File::read_to_string("input.txt");
let input = input.unwrap().trim().split("\n");

let input = parse_input(input);
find_empty_rows(input);
find_empty_cols(input);

print(f"part1: {part1(input)}");
print(f"part2: {part2(input)}");

// actually generate a new expanded field
fn part1(i: Input) -> int {
    i.print();
    let i = expand_empty(i);
    i.print();

    let mut sum = 0;
    for g1 in i.galaxies.values() {
        for g2 in i.galaxies.values() {
            let dx = g2.x - g1.x;
            let dy = g2.y - g1.y;
            let dxy = dx.abs() + dy.abs();
            sum += dxy;
        }
    }
    // half because we calculate each path twice: a -> b and b -> a
    sum / 2
}

// We could have used the expanded field approach here as well,
// as the field doesn't store everything but only galaxies.
// As we don't store the full 2D array and the number of galaxies stayed
// the same, we could have just added 1_000_001 instead of 2 in the expand function.
// But this approach (multiplying the number of empty rows & cols between two galaxies
// by the distance) is the "more obvious" mathematical one.
fn part2(i: Input) -> int {
    let mut sum = 0;
    for g1 in i.galaxies.values() {
        for g2 in i.galaxies.values() {
            let x1 = g1.x.min(g2.x);
            let x2 = g1.x.max(g2.x);
            let y1 = g1.y.min(g2.y);
            let y2 = g1.y.max(g2.y);

            let mut empty_cols_between = 0;
            for x in List::range(x1, x2) {
                if i.empty_cols.contains(x) {
                    empty_cols_between += 1;
                }
            }
            let mut empty_rows_between = 0;
            for y in List::range(y1, y2) {
                if i.empty_rows.contains(y) {
                    empty_rows_between += 1;
                }
            }
            let dx = x2 - x1 + 1000000 * empty_cols_between - empty_cols_between;
            let dy = y2 - y1 + 1000000 * empty_rows_between - empty_rows_between;
            let dxy = dx + dy;
            sum += dxy;
        }
    }
    sum / 2
}

fn find_empty_rows(mut i: Input) {
    for y in List::range(0, i.height) {
        let mut empty = true;
        for x in List::range(0, i.width) {
            if i.galaxies.contains(Coord::new(x, y)) {
                empty = false;
                break;
            }
        }
        if empty {
            i.empty_rows.insert(y);
        }
    }
}
fn find_empty_cols(mut i: Input) {
    for x in List::range(0, i.width) {
        let mut empty = true;
        for y in List::range(0, i.height) {
            if i.galaxies.contains(Coord::new(x, y)) {
                empty = false;
                break;
            }
        }
        if empty {
            i.empty_cols.insert(x);
        }
    }
}
fn expand_empty(i: Input) -> Input {
    let mut new_x = 0;
    let mut new_y = 0;
    let mut new_width = i.width + i.empty_cols.len();
    let mut new_height = i.height + i.empty_rows.len();
    let mut galaxies = Set::new();
    let mut empty_rows = Set::new();
    let mut empty_cols = Set::new();

    for y in List::range(0, i.height) {
        new_x = 0;
        if i.empty_rows.contains(y) {
            new_y += 2;
            continue;
        }
        for x in List::range(0, i.width) {
            if i.empty_cols.contains(x) {
                new_x += 2;
                continue;
            }
            if i.galaxies.contains(Coord::new(x, y)) {
                galaxies.insert(Coord::new(new_x, new_y));
            }
            new_x += 1;
        }
        new_y += 1;
    }
    assert_eq(new_x, new_width);
    assert_eq(new_y, new_height);
    Input {
        galaxies: galaxies,
        width: new_width,
        height: new_height,
        empty_rows: empty_rows,
        empty_cols: empty_cols,
    }
}

struct Coord {
    y: int,
    x: int,
}
impl Coord { fn new(x: int, y: int) -> Coord { Coord { x: x, y: y } } }
struct Input {
    galaxies: Set<Coord>,
    width: int,
    height: int,
    empty_rows: Set<int>,
    empty_cols: Set<int>,
}
impl Input {
    fn print(self) {
        let mut s = "";
        for y in List::range(0, self.height) {
            for x in List::range(0, self.width) {
                if self.galaxies.contains(Coord::new(x, y)) {
                    s = f"{s}#";
                } else {
                    s = f"{s}.";
                }
            }
            s = f"{s}\n";
        }
        print(s);
    }
}

fn parse_input(input: List<string>) -> Input {
    let height = input.len();
    let width = input.get(0).unwrap().len_utf8();
    let mut galaxies = Set::new();
    for y in List::range(0, height) {
        let chars = input.get(y).unwrap().chars_utf32();
        for x in List::range(0, width) {
            let c = chars.get(x).unwrap();
            if c == "#" {
                galaxies.insert(Coord::new(x, y));
            }
        }
    }
    Input {
        galaxies: galaxies,
        width: width,
        height: height,
        empty_rows: Set::new(),
        empty_cols: Set::new(),
    }
}
