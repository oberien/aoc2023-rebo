//let input = File::read_to_string("sample.txt");
let input = File::read_to_string("input.txt");
let input = input.unwrap().trim().split("\n");

let schematic = parse_schematic(input);
let numbers = extract_numbers(schematic);

print(f"part1: {part1(schematic, numbers)}");
print(f"part2: {part2(schematic, numbers)}");

fn part1(schematic: Schematic, numbers: List<Number>) -> int {
    let mut sum = 0;
    let fields = schematic.fields.clone();
    for number in numbers {
        match adjacent_symbol(number, schematic) {
            Option::Some(symbol) => sum += number.num,
            Option::None => (),
        }
    }
    sum
}
fn part2(schematic: Schematic, numbers: List<Number>) -> int {
    let mut sum = 0;
    for y in List::range(0, schematic.height) {
        for x in List::range(0, schematic.width) {
            if schematic.get(x, y) == Option::Some("*") {
                let adjacent = adjacent_numbers(Coord { x: x, y: y }, numbers);
                let adjacent = adjacent.values();
                print(adjacent);
                if adjacent.len() == 2 {
                    sum += adjacent.get(0).unwrap().num * adjacent.get(1).unwrap().num;
                }
            }
        }
    }
    sum
}

struct Schematic {
    width: int,
    height: int,
    fields: Map<Coord, string>,
}
impl Schematic {
    fn get(self, x: int, y: int) -> Option<string> {
        self.fields.get(Coord { x: x, y: y })
    }
}
struct Coord {
    x: int,
    y: int,
}

fn parse_schematic(input: List<string>) -> Schematic {
    let mut fields = Map::new();
    let mut y = 0;
    for line in input {
        let mut x = 0;
        for char in line.split("") {
            if char == "" { continue; }
            fields.insert(Coord { x: x, y: y }, char);
            x += 1;
        }
        y += 1;
    }
    Schematic {
        width: input.get(0).unwrap().len_utf8(),
        height: input.len(),
        fields: fields,
    }
}

struct Number {
    num: int,
    fields: List<Coord>,
}
fn extract_numbers(schematic: Schematic) -> List<Number> {
    let mut numbers = List::new();
    for y in List::range(0, schematic.height) {
        let mut num = 0;
        let mut fields = List::new();
        for x in List::range(0, schematic.width) {
            let field = schematic.get(x, y).unwrap();
            if "0" <= field && field <= "9" {
                num *= 10;
                num += field.parse_int().unwrap();
                fields.push(Coord { x: x, y: y });
            } else if num > 0 {
                numbers.push(Number { num: num, fields: fields });
                num = 0;
                fields = List::new();
            }
        }
        if num > 0 {
            numbers.push(Number { num: num, fields: fields });
        }
    }
    numbers
}
fn adjacent_symbol(number: Number, schematic: Schematic) -> Option<string> {
    for coord in number.fields {
        for dy in List::range(-1, 2) {
            for dx in List::range(-1, 2) {
                let field = match schematic.get(coord.x + dx, coord.y + dy) {
                    Option::Some(field) => field,
                    Option::None => continue,
                };
                if ("0" <= field && field <= "9") || field == "." {
                    continue;
                }
                return Option::Some(field);
            }
        }
    }
    Option::None
}

fn adjacent_numbers(coord: Coord, numbers: List<Number>) -> Set<Number> {
    let mut adjacent = Set::new();
    for dy in List::range(-1, 2) {
        for dx in List::range(-1, 2) {
            let field = Coord { x: coord.x + dx, y: coord.y + dy };
            for number in numbers {
                if number.fields.contains(field) {
                    adjacent.insert(number);
                }
            }
        }
    }
    adjacent
}
