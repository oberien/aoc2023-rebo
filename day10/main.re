//let input = File::read_to_string("sample.txt");
//let input = File::read_to_string("sample2.txt");
//let input = File::read_to_string("sample3.txt");
//let input = File::read_to_string("sample4.txt");
//let input = File::read_to_string("sample5.txt");
let input = File::read_to_string("input.txt");
let input = input.unwrap().trim().split("\n");

let field = parse_input(input);

let p1 = part1(field);
print(f"part1: {p1}");
field.clean();
let expanded = expand_field(field);
expanded.print();
let p2 = part2(field, expanded);
print(f"part2: {p2}");

fn part1(mut field: Field) -> int {
    let mut prev = Point::new(-1, -1);
    let mut current = field.start;
    let mut length = 0;
    while {
        field.path_fields.insert(current);
        let next = get_next(current, field);
        let new = if next.a == prev {
            next.b
        } else {
            next.a
        };
        prev = current;
        current = new;
        length += 1;
        current != field.start
    } {}
    length / 2
}
fn part2(orig: Field, field: Field) -> int {
    for y in List::range(0, field.height) {
        for x in List::range(0, field.width) {
            flow(field, Point::new(x, y));
        }
        field.print();
    }
    field.print();
    let mut sum = 0;
    for y in List::range(0, orig.height) {
        for x in List::range(0, orig.width) {
            let p0 = Point::new(x*2, y*2);
            let p1 = Point::new(x*2+1, y*2);
            let p2 = Point::new(x*2, y*2+1);
            let p3 = Point::new(x*2+1, y*2+1);
            let inside = field.fields.get(p0).unwrap() == Tile::Inside
                && field.fields.get(p1).unwrap() == Tile::Inside
                && field.fields.get(p2).unwrap() == Tile::Inside
                && field.fields.get(p3).unwrap() == Tile::Inside;
            if inside {
                field.fields.insert(p0, Tile::Marked);
                field.fields.insert(p1, Tile::Marked);
                field.fields.insert(p2, Tile::Marked);
                field.fields.insert(p3, Tile::Marked);
                field.print();
//                sleep(200);
            }
            sum += inside.to_int();
        }
    }

    sum
}


// -----------------------
// Part 1 Solution Helpers
// -----------------------

struct Next {
    a: Point,
    b: Point,
}
fn get_next(cur: Point, field: Field) -> Next {
    let x = get_lrtb(cur);

    match field.fields.get(cur).unwrap() {
        Tile::Empty => panic(f"{cur} Empty"),
        Tile::Start => panic(f"{cur} Start"),
        Tile::Inside => panic(f"{cur} Inside"),
        Tile::Outside => panic(f"{cur} Outside"),
        Tile::Current => panic(f"{cur} Current"),
        Tile::Marked => panic(f"{cur} Marked"),
        Tile::TB => Next { a: x.top, b: x.bottom },
        Tile::LR => Next { a: x.left, b: x.right },
        Tile::TL => Next { a: x.top, b: x.left },
        Tile::TR => Next { a: x.top, b: x.right },
        Tile::BL => Next { a: x.bottom, b: x.left },
        Tile::BR => Next { a: x.bottom, b: x.right },
    }
}

// -----------------------
// Part 2 Solution Helpers
// -----------------------

fn flow(mut field: Field, p: Point) {
    let mut worklist = List::of(p);
    let mut visited = List::new();
    let tile = loop {
        let p = match worklist.remove(0) {
            Option::Some(p) => p,
            Option::None => {
                // we visited all recursively adjacent empty fields and didn't find
                // anything Outside -> we are inside
                break Tile::Inside
            }
        };
        let tile = field.fields.get(p);
        if tile != Option::Some(Tile::Empty) {
            continue;
        }

        visited.push(p);

        field.fields.insert(p, Tile::Current);
//        field.print();
        if p.x <= 0 || p.x >= field.width - 1 || p.y <= 0 || p.y >= field.height - 1 {
            break Tile::Outside;
        }
        let x = get_lrtb(p);
        let adjacent_outside = field.fields.get(x.top) == Option::Some(Tile::Outside)
            || field.fields.get(x.right) == Option::Some(Tile::Outside)
            || field.fields.get(x.bottom) == Option::Some(Tile::Outside)
            || field.fields.get(x.left) == Option::Some(Tile::Outside);
        if adjacent_outside {
            break Tile::Outside;
        }
        let adjacent_inside = field.fields.get(x.top) == Option::Some(Tile::Inside)
            || field.fields.get(x.right) == Option::Some(Tile::Inside)
            || field.fields.get(x.bottom) == Option::Some(Tile::Inside)
            || field.fields.get(x.left) == Option::Some(Tile::Inside);
        if adjacent_inside {
            break Tile::Inside;
        }

        worklist.push(x.top);
        worklist.push(x.right);
        worklist.push(x.bottom);
        worklist.push(x.left);
    };
    visited.reverse();
    for p in visited {
        field.fields.insert(p, tile);
//        field.print();
    }
}

// ----------------
// Parser / Helpers
// ----------------

struct Field {
    start: Point,
    fields: Map<Point, Tile>,
    width: int,
    height: int,
    path_fields: Set<Point>,
}
impl Field {
    fn print(self) {
        let mut s = "";
        for y in List::range(0, self.height) {
            for x in List::range(0, self.width) {
                s = f"{s}{self.fields.get(Point::new(x, y)).unwrap().to_str()}";
            }
            s = f"{s}\n";
        }
        print(s);
//        sleep(10);
    }
    fn clean(mut self) {
        for y in List::range(0, self.height) {
            for x in List::range(0, self.width) {
                let p = Point::new(x, y);
                if !self.path_fields.contains(p) {
                    self.fields.insert(p, Tile::Empty);
                }
            }
        }
    }
}

struct Point {
    x: int,
    y: int,
}
impl Point {
    fn new(x: int, y: int) -> Point {
        Point { x: x, y: y }
    }
}

enum Tile {
    Empty,
    Start,
    TB,
    LR,
    TL,
    TR,
    BL,
    BR,
    Inside,
    Outside,
    Current,
    Marked,
}
impl Tile {
    fn from_str(s: string) -> Tile {
        match s {
            "." => Tile::Empty,
            "S" => Tile::Start,
            "|" => Tile::TB,
            "-" => Tile::LR,
            "J" => Tile::TL,
            "L" => Tile::TR,
            "7" => Tile::BL,
            "F" => Tile::BR,
            _ => panic(f"unknown tile {s:?}"),
        }
    }
    fn to_str(self) -> string {
        match self {
            Tile::Empty => ".",
            Tile::Start => "S",
            Tile::TB => "┃",
            Tile::LR => "━",
            Tile::TL => "┛",
            Tile::TR => "┗",
            Tile::BL => "┓",
            Tile::BR => "┏",
            Tile::Inside => "I",
            Tile::Outside => "O",
            Tile::Current => "C",
            Tile::Marked => "█",
        }
    }
}

struct Lrtb {
    left: Point,
    right: Point,
    top: Point,
    bottom: Point,
}
fn get_lrtb(point: Point) -> Lrtb {
    Lrtb {
        top: Point::new(point.x, point.y - 1),
        right: Point::new(point.x + 1, point.y),
        bottom: Point::new(point.x, point.y + 1),
        left: Point::new(point.x - 1, point.y),
    }
}

fn parse_input(input: List<string>) -> Field {
    let mut fields = Map::new();
    let mut start = Option::None;
    let mut y = 0;
    for line in input {
        let mut x = 0;
        for char in line.chars_utf32() {
            let point = Point::new(x, y);
            let tile = Tile::from_str(char);
            if tile == Tile::Start {
                assert_eq(start, Option::None);
                start = Option::Some(point);
            }
            fields.insert(point, tile);
            x += 1;
        }
        y += 1;
    }
    let mut field = Field {
        start: start.unwrap(),
        fields: fields,
        width: input.get(0).unwrap().len_utf8(),
        height: input.len(),
        path_fields: Set::new(),
    };
    replace_start_tile(field);
    field
}
fn replace_start_tile(mut field: Field) {
    let x = get_lrtb(field.start);
    let mut top = false;
    let mut right = false;
    let mut bottom = false;
    let mut left = false;
    match field.fields.get(x.top) {
        Option::Some(tile) => match tile {
            Tile::BL => top = true,
            Tile::TB => top = true,
            Tile::BR => top = true,
            _ => (),
        },
        _ => (),
    }
    match field.fields.get(x.right) {
        Option::Some(tile) => match tile {
            Tile::TL => right = true,
            Tile::LR => right = true,
            Tile::BL => right = true,
            _ => (),
        },
        _ => (),
    }
    match field.fields.get(x.bottom) {
        Option::Some(tile) => match tile {
            Tile::TR => bottom = true,
            Tile::TB => bottom = true,
            Tile::TL => bottom = true,
            _ => (),
        },
        _ => (),
    }
    match field.fields.get(x.left) {
        Option::Some(tile) => match tile {
            Tile::BR => left = true,
            Tile::LR => left = true,
            Tile::TR => left = true,
            _ => (),
        },
        _ => (),
    }
    assert_eq(top.to_int() + right.to_int() + bottom.to_int() + left.to_int(), 2);
    let start_tile = if top && left {
        Tile::TL
    } else if top && bottom {
        Tile::TB
    } else if top && right {
        Tile::TR
    } else if bottom && left {
        Tile::BL
    } else if bottom && right {
        Tile::BR
    } else if left && right {
        Tile::LR
    } else {
        panic(f"unknown combination t:{top} r:{right} b:{bottom} l:{left}")
    };
    field.fields.insert(field.start, start_tile);
}

fn expand_field(field: Field) -> Field {
    let mut new = Map::new();
    let new_width = field.width * 2;
    let new_height = field.height * 2;
    for p in field.path_fields.values() {
        let tile = field.fields.get(p).unwrap();
        let new_p = Point::new(p.x * 2, p.y * 2);
        let x = get_lrtb(new_p);
        new.insert(new_p, tile);
        match tile {
            Tile::Empty => panic("expand Empty"),
            Tile::Start => panic("expand Start"),
            Tile::Inside => panic("expand Inside"),
            Tile::Outside => panic("expand Outside"),
            Tile::Current => panic("expand Current"),
            Tile::Marked => panic("expand Marked"),
            Tile::TB => { new.insert(x.top, Tile::TB); new.insert(x.bottom, Tile::TB); },
            Tile::LR => { new.insert(x.left, Tile::LR); new.insert(x.right, Tile::LR); },
            Tile::TL => { new.insert(x.top, Tile::TB); new.insert(x.left, Tile::LR); },
            Tile::TR => { new.insert(x.top, Tile::TB); new.insert(x.right, Tile::LR); },
            Tile::BL => { new.insert(x.bottom, Tile::TB); new.insert(x.left, Tile::LR); },
            Tile::BR => { new.insert(x.bottom, Tile::TB); new.insert(x.right, Tile::LR); },
        }
    }
    let mut new_path_fields = Set::new();
    for y in List::range(0, new_height) {
        for x in List::range(0, new_width) {
            let p = Point::new(x, y);
            match new.get(p) {
                Option::Some(some) => { new_path_fields.insert(p); },
                Option::None => { new.insert(p, Tile::Empty); },
            }
        }
    }

    Field {
        start: Point::new(field.start.x * 2, field.start.y * 2),
        fields: new,
        width: new_width,
        height: new_height,
        path_fields: new_path_fields,
    }
}
