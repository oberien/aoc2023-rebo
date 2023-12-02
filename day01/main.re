//let input = File::read_to_string("sample.txt");
//let input = File::read_to_string("sample2.txt");
let input = File::read_to_string("input.txt");
let input = input.unwrap().trim().split("\n");


//print(f"part1: {part1(input)}");

print(f"part2: {part2(input)}");

fn part1(input: List<string>) -> int {
    let mut sum = 0;
    for line in input {
        sum += get_line(line);
    }
    sum
}
fn get_line(line: string) -> int {
    let digits = line.find_matches("\\d");
    let first = digits.get(0).unwrap().parse_int().unwrap();
    let last = digits.last().unwrap().parse_int().unwrap();
    first * 10 + last
}

struct Number {
    name: string,
    digit: int,
}
static numbers = List::of(
    Number { name: "one", digit: 1 },
    Number { name: "two", digit: 2 },
    Number { name: "three", digit: 3 },
    Number { name: "four", digit: 4 },
    Number { name: "five", digit: 5 },
    Number { name: "six", digit: 6 },
    Number { name: "seven", digit: 7 },
    Number { name: "eight", digit: 8 },
    Number { name: "nine", digit: 9 },
);

fn part2(input: List<string>) -> int {
    let mut sum = 0;
    for line in input {
        let mut cleaned = "";
        let mut i = 0;
        while i < line.len_utf8() {
            let rest = line.slice(i);
            for num in numbers {
                if rest.starts_with(num.name) || rest.starts_with(f"{num.digit}") {
                    cleaned = f"{cleaned}{num.digit}";
                }
            }
            i += 1;
        }
        let res = get_line(cleaned);
        print(f"{line} - {cleaned}: {res}");
        sum += res;
    }
    sum
}

