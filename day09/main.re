//let input = File::read_to_string("sample.txt");
let input = File::read_to_string("input.txt");
let input = input.unwrap().trim().split("\n");

let input = parse_input(input);

print(f"part1: {extrapolate(input, Extrapolation::Forward)}");
print(f"part2: {extrapolate(input, Extrapolation::Backward)}");

fn extrapolate(input: List<List<int>>, dir: Extrapolation) -> int {
    let mut sum = 0;
    for value in input {
        let res = dir.extrapolate(value, predict_next(value, dir));
        sum += res;
    }
    sum
}

fn part2(input: List<List<int>>) -> int {
    panic("")
}

fn predict_next(previous: List<int>, dir: Extrapolation) -> int {
    let mut this = List::new();
    let mut current = previous.first().unwrap();
    let mut all_zeroes = true;
    for next in previous.slice(1) {
        let val = next - current;
        all_zeroes &= val == 0;
        this.push(val);
        current = next;
    }
    let res = if all_zeroes {
        0
    } else {
        dir.extrapolate(this, predict_next(this, dir))
    };
    res
}

enum Extrapolation {
    Forward,
    Backward,
}
impl Extrapolation {
    fn extrapolate(self, this: List<int>, next_rec: int) -> int {
        match self {
            Extrapolation::Forward => this.last().unwrap() + next_rec,
            Extrapolation::Backward => this.first().unwrap() - next_rec,
        }
    }
}

fn parse_input(lines: List<string>) -> List<List<int>> {
    let mut values = List::new();
    for line in lines {
        let mut history = List::new();
        for num in line.find_matches("-?\\d+") {
            history.push(num.parse_int().unwrap());
        }
        values.push(history);
    }
    values
}
