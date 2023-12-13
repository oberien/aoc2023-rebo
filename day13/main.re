//let input = File::read_to_string("sample.txt");
let input = File::read_to_string("input.txt");
let input = input.unwrap().trim().split("\n\n");

let input = parse_input(input);

static DBG = false;

print(f"part1: {part1(input)}");
print(f"part2: {part2(input)}");

fn part1(fields: List<Field>) -> int {
    let mut sum = 0;
    for field in fields {
        if DBG { print(field.to_string()); }
        let res = find_reflection_numbers(field);
        assert_eq(res.len(), 1);
        sum += res.get(0).unwrap();
    }
    sum
}

fn part2(fields: List<Field>) -> int {
    let mut sum = 0;
    'outer: for mut field in fields {
        let orig = find_reflection_numbers(field);
        assert_eq(orig.len(), 1);
        let orig = orig.get(0).unwrap();
        for y in List::range(0, field.rows.len()) {
            for x in List::range(0, field.cols.len()) {
                swap(field, x, y);
//                print(f"trying: {x}:{y}");
//                print(field.to_string());
                let finds = find_reflection_numbers(field);
                // swap back
                swap(field, x, y);
                finds.remove_element(orig);
                // no find
                if finds.len() == 0 {
                    continue;
                }
                assert_eq(finds.len(), 1);
                let num = finds.get(0).unwrap();
//                print(num);
                sum += num;
                continue 'outer;
            }
        }
    }
    sum
}

fn find_reflection_numbers(field: Field) -> List<int> {
    let mut finds = List::new();
    let f = find_reflection_axis(field.cols);
    finds.extend(f);
    for f in find_reflection_axis(field.rows) {
        finds.push(f * 100);
    }
    finds
}

fn find_reflection_axis(rows: List<Row>) -> List<int> {
    let mut finds = List::new();
    'outer: for i in List::range(1, rows.len()) {
        if DBG { print(f"    checking {i}"); }
        for j in List::range(i, rows.len()) {
            let left = match rows.get(i - 1 - (j - i)) {
                Option::Some(left) => left,
                Option::None => break,
            };
            let right = match rows.get(j) {
                Option::Some(right) => right,
                Option::None => break,
            };
            if DBG { print(f"        left  ({i - 1 - (i - j):2}): {left.to_string()}"); }
            if DBG { print(f"        right ({j:2}): {right.to_string()}"); }
            if left != right {
                continue 'outer;
            }
        }
        if DBG { print(f"    found {i}"); }
        finds.push(i);
    }
    finds
}

fn swap(mut field: Field, x: int, y: int) {
    let replace = match field.rows.get(y).unwrap().tiles.get(x).unwrap() {
        Tile::Ash => Tile::Rock,
        Tile::Rock => Tile::Ash,
    };
    field.rows.get(y).unwrap().tiles.set(x, replace);
    field.cols.get(x).unwrap().tiles.set(y, replace);
}

struct Field {
    rows: List<Row>,
    cols: List<Row>,
}
impl Field {
    fn to_string(self) -> string {
        let mut s = "";
        for row in self.rows {
            s = f"{s}{row.to_string()}\n";
        }
        s
    }
}
struct Row {
    tiles: List<Tile>,
}
impl Row {
    fn to_string(self) -> string {
        let mut s = "";
        for tile in self.tiles {
            s = f"{s}{tile.to_string()}";
        }
        s
    }
}
enum Tile {
    Ash,
    Rock,
}
impl Tile {
    fn to_string(self) -> string {
        match self {
            Tile::Ash => ".",
            Tile::Rock => "#",
        }
    }
}

fn parse_input(input: List<string>) -> List<Field> {
    let mut fields = List::new();
    for field in input {
        fields.push(parse_field(field));
    }
    fields
}
fn parse_field(input: string) -> Field {
    let mut rows = List::new();
    for line in input.split("\n") {
        rows.push(parse_row(line));
    }
    let mut cols = List::new();
    for x in List::range(0, rows.get(0).unwrap().tiles.len()) {
        let mut column = List::new();
        for y in List::range(0, rows.len()) {
            column.push(rows.get(y).unwrap().tiles.get(x).unwrap());
        }
        cols.push(Row { tiles: column });
    }
    Field { rows: rows, cols: cols }
}
fn parse_row(row: string) -> Row {
    let mut tiles = List::new();
    for c in row.chars_utf32() {
        tiles.push(parse_tile(c));
    }
    Row { tiles: tiles }
}
fn parse_tile(c: string) -> Tile {
    match c {
        "." => Tile::Ash,
        "#" => Tile::Rock,
        _ => panic(f"unknown tile {c}"),
    }
}
fn indent(depth: int) -> string {
    let s = "    ";
    s.repeat(depth)
}
