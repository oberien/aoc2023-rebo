//let input = File::read_to_string("sample.txt");
//let input = File::read_to_string("sample2.txt");
//let input = File::read_to_string("sample3.txt");
let input = File::read_to_string("input.txt");
let input = input.unwrap().trim();

let input = parse_input(input);
dbg(input);

print(f"part1: {part1(input)}");
print(f"part2: {part2(input)}");

fn part1(input: Input) -> int {
    let mut current = "AAA";
    let mut steps = 0;
    let mut dir_index = 0;
    loop {
        steps += 1;
        let node = input.nodes.get(current).unwrap();
        current = match input.dirs.get(dir_index).unwrap() {
            Dir::Left => node.left,
            Dir::Right => node.right,
        };
        dir_index = (dir_index + 1) % input.dirs.len();
        if current == "ZZZ" {
            break;
        }
    }
    steps
}

// WTF why is this the solution? It doesn't make any logical sense.
// The only reason why LCM works is that their input is structured that way.
// There is no guarantee or any other reason why the problem statement as stated
// can be solved this way.
// This "solution" is not a solution to the problem at hand, but a solution to
// the given input. It does not generalize to the problem in any way or form.
// It only works because distance from start to cycle = cycle length and
// because the entry point in the cycle is exactly the end node.
fn part2(input: Input) -> int {
    let mut distances = List::new();
    for node in input.nodes.values() {
        if node.name.ends_with("A") {
            distances.push(get_steps_to_first_end(node.name, input));
        }
    }
    let mut lcm = distances.get(0).unwrap();
    for i in List::range(1, distances.len()) {
        lcm = lcm.lcm(distances.get(i).unwrap());
    }
    lcm
}

fn get_steps_to_first_end(start: string, input: Input) -> int {
    let mut current = start;
    let mut steps = 0;
    let mut dir_index = 0;
    loop {
        steps += 1;
        let node = input.nodes.get(current).unwrap();
        current = match input.dirs.get(dir_index).unwrap() {
            Dir::Left => node.left,
            Dir::Right => node.right,
        };
        dir_index = (dir_index + 1) % input.dirs.len();
        if current.ends_with("Z") {
            break;
        }
    }
    steps
}

struct Input {
    dirs: List<Dir>,
    nodes: Map<string, Node>,
}
enum Dir {
    Left,
    Right,
}
struct Node {
    name: string,
    left: string,
    right: string,
}

fn parse_input(input: string) -> Input {
    let parts = input.split("\n\n");
    let dirs = parse_directions(parts.get(0).unwrap());
    let mut nodes = Map::new();
    for line in parts.get(1).unwrap().split("\\n") {
        let node = parse_node(line);
        nodes.insert(node.name, node);
    }
    Input { dirs: dirs, nodes: nodes }
}
fn parse_directions(input: string) -> List<Dir> {
    let mut dirs = List::new();
    for c in input.chars_utf32() {
        dirs.push(match c {
            "L" => Dir::Left,
            "R" => Dir::Right,
            _ => panic(f"unknown direction {c}"),
        });
    }
    dirs
}
fn parse_node(line: string) -> Node {
    let captures = line.captures("^(\\w{3}) = \\((\\w{3}), (\\w{3})\\)$").unwrap();
    Node {
        name: captures.get(1).unwrap().unwrap(),
        left: captures.get(2).unwrap().unwrap(),
        right: captures.get(3).unwrap().unwrap(),
    }
}

// ---------------------
// Failed Part2 Attempts
// ---------------------

// brute-force takes too long
fn part2_bruteforce(input: Input) -> int {
    // get start states
    let mut current = List::new();
    for node in input.nodes.values() {
        if node.name.ends_with("A") {
            current.push(node.name);
        }
    }

    let mut steps = 0;
    let mut dir_index = 0;
    loop {
        steps += 1;
        let mut new_current = List::new();

        for node in current {
            let node = input.nodes.get(node).unwrap();
            new_current.push(match input.dirs.get(dir_index).unwrap() {
                Dir::Left => node.left,
                Dir::Right => node.right,
            });
        }
        dir_index = (dir_index + 1) % input.dirs.len();
        current = new_current;

        // check if all states are end-states
        let mut all_end = true;
        for node in current {
            all_end &= node.ends_with("Z");
        }
        if all_end { break; }
    }
    steps
}
