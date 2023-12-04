//let input = File::read_to_string("sample.txt");
let input = File::read_to_string("input.txt");
let input = input.unwrap().trim().split("\n");

let cards = parse_input(input);

print(f"part1: {part1(cards)}");
print(f"part2: {part2(cards)}");

fn part1(cards: List<Card>) -> int {
    let mut sum = 0;
    for card in cards {
        sum += card.score;
    }
    sum
}
fn part2(cards: List<Card>) -> int {
    let mut worklist = List::range(0, cards.len());
    let mut sum = 0;
    loop {
        let id = worklist.pop();
        if id.is_none() {
            break;
        }
        let id = id.unwrap();
        let card = cards.get(id).unwrap();
        sum += 1;
        for i in List::range(id+1, id+1+card.wins) {
            worklist.push(i);
        }
    }
    sum
}

struct Card {
    id: int,
    winning: Set<int>,
    drawn: Set<int>,
    wins: int,
    score: int,
}

fn parse_input(input: List<string>) -> List<Card> {
    let mut cards = List::new();
    for line in input {
        let captures = line.captures("^Card +(\\d+): (.*) \\| (.*)$").unwrap();
        let id = captures.get(1).unwrap().unwrap().parse_int().unwrap();
        let mut winning = Set::new();
        let mut drawn = Set::new();
        for w in captures.get(2).unwrap().unwrap().split(" +") {
            if w == "" { continue; }
            winning.insert(w.parse_int().unwrap());
        }
        for d in captures.get(3).unwrap().unwrap().split(" +") {
            if d == "" { continue; }
            drawn.insert(d.parse_int().unwrap());
        }
        let wins = winning.intersection(drawn).len();
        let score = if wins > 0 { int::pow(2, wins-1) } else { 0 };
        cards.push(Card { id: id, winning: winning, drawn: drawn, wins: wins, score: score });
    }
    cards
}
