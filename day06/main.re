//let input = File::read_to_string("sample.txt");
let input = File::read_to_string("input.txt");
let input = input.unwrap().trim().split("\n");

print(f"part1: {part1(parse_input1(input))}");
print(f"part2: {part2(parse_input2(input))}");

fn part1(races: List<Race>) -> int {
    let mut res = 1;
    for race in races {
        let mut wins = 0;
        for i in List::range(0, race.duration + 1) {
            let range = calc_range(i, race.duration);
            if range > race.record {
                wins += 1;
            }
        }
        res *= wins;
    }
    res
}
fn part2(race: Race) -> int {
    // range = speed * drive_duration
    // speed = press_duration
    // race.duration = press_duration + drive_duration
    // <=> press_duration = race.duration - drive_duration
    //
    // => speed = race.duration - drive_duration
    // => range = (race.duration - drive_duration) * drive_duration
    // => range = race.duration * drive_duration - drive_duration^2
    // => 0 = -drive_duration^2 + race.duration * drive_duration - range
    //
    // quadratic formula:
    // ax^2 + bx + c = 0
    // x_{1,2} = (-b ± sqrt(b^2 - 4ac)) / (2a)

    let a = -1;
    let b = race.duration;
    let c = -race.record;
    let tmp = b.pow(2) - (4 * a * c);
    let drive_duration1 = (-b.to_float() + tmp.to_float().sqrt()) / (2.0 * a.to_float());
    let drive_duration2 = (-b.to_float() - tmp.to_float().sqrt()) / (2.0 * a.to_float());
    drive_duration2.floor(0).to_int() - drive_duration1.ceil(0).to_int() + 1
}

fn calc_range(press_duration: int, race_duration: int) -> int {
    assert(race_duration >= press_duration);
    press_duration * (race_duration - press_duration)
}

struct Race {
    duration: int,
    record: int,
}

fn parse_input1(input: List<string>) -> List<Race> {
    let durations = input.get(0).unwrap().find_matches("\\d+");
    let records = input.get(1).unwrap().find_matches("\\d+");
    assert_eq(durations.len(), records.len());
    let mut races = List::new();
    for i in List::range(0, durations.len()) {
        races.push(Race {
            duration: durations.get(i).unwrap().parse_int().unwrap(),
            record: records.get(i).unwrap().parse_int().unwrap(),
        })
    }
    races
}

fn parse_input2(input: List<string>) -> Race {
    let duration = input.get(0).unwrap().find_matches("\\d+").join("").parse_int().unwrap();
    let record = input.get(1).unwrap().find_matches("\\d+").join("").parse_int().unwrap();
    Race { duration: duration, record: record }
}
