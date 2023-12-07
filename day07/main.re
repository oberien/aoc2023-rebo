//let input = File::read_to_string("sample.txt");
let input = File::read_to_string("input.txt");
let input = input.unwrap().trim().split("\n");

print(f"part1: {part1(parse_input(input, Category::from_counts))}");
print(f"part2: {part1(parse_input(input, Category::from_counts_with_jokers))}");

fn part1(mut hands: List<Hand>) -> int {
    hands.sort();
    let mut winnings = 0;
    for i in List::range(0, hands.len()) {
        winnings += (i+1) * hands.get(i).unwrap().value;
    }
    winnings
}

struct Hand {
    category: Category,
    cards: List<Card>,
    value: int,
}
impl Hand {
    fn new(cards: List<Card>, value: int, new_cat: fn(Map<Card, Int>) -> Category) -> Hand {
        assert_eq(cards.len(), 5);
        let mut counts = Map::new();
        for card in cards {
            let mut count = counts.entry(card).or_insert(Int { value: 0 });
            count.value += 1;
        }
        Hand { category: new_cat(counts), cards: cards, value: value }
    }
}
struct Int { value: int }
impl Int {
    fn new(value: int) -> Int { Int { value: value } }
}
// Rebo doesn't properly support generic anonymous functions / closures yet.
// Therefore `List::{sort_by,sort_by_key}` isn't possible yet.
// To support both part1 and part2, I'd need to practically duplicate all code.
// As I don't want to, just comment out/in the relevant Jack below.
enum Card {
    // For part2
    Jack,
    Two, Three, Four, Five, Six,
    Seven, Eight, Nine, Ten,
    // For part1
    //Jack,
    Queen, King, Ace,
}
impl Card {
    fn from_string(s: string) -> Option<Card> {
        match s {
            "2" => Option::Some(Card::Two),
            "3" => Option::Some(Card::Three),
            "4" => Option::Some(Card::Four),
            "5" => Option::Some(Card::Five),
            "6" => Option::Some(Card::Six),
            "7" => Option::Some(Card::Seven),
            "8" => Option::Some(Card::Eight),
            "9" => Option::Some(Card::Nine),
            "T" => Option::Some(Card::Ten),
            "J" => Option::Some(Card::Jack),
            "Q" => Option::Some(Card::Queen),
            "K" => Option::Some(Card::King),
            "A" => Option::Some(Card::Ace),
            _ => Option::None,
        }
    }
}
enum Category {
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    FullHouse,
    FourOfAKind,
    FiveOfAKind,
}
impl Category {
    fn from_counts(counts: Map<Card, Int>) -> Category {
        let mut values = counts.values();
        values.sort();
        values.reverse();
        Category::from_sorted_values(values)
    }
    fn from_counts_with_jokers(mut counts: Map<Card, Int>) -> Category {
        let jokers = counts.remove(Card::Jack).unwrap_or(Int { value: 0 });
        let values = counts.values();
        values.sort();
        values.reverse();
        match values.get(0) {
            Option::Some(mut largest) => largest.value += jokers.value,
            Option::None => values.push(jokers),
        }
        Category::from_sorted_values(values)
    }
    fn from_sorted_values(values: List<Int>) -> Category {
        if values.get(0) == Option::Some(Int::new(5)) {
            Category::FiveOfAKind
        } else if values.get(0) == Option::Some(Int::new(4)) {
            Category::FourOfAKind
        } else if values.get(0) == Option::Some(Int::new(3)) && values.get(1) == Option::Some(Int::new(2)) {
            Category::FullHouse
        } else if values.get(0) == Option::Some(Int::new(3)) {
            Category::ThreeOfAKind
        } else if values.get(0) == Option::Some(Int::new(2)) && values.get(1) == Option::Some(Int::new(2)) {
            Category::TwoPair
        } else if values.get(0) == Option::Some(Int::new(2)) {
            Category::OnePair
        } else {
            Category::HighCard
        }
    }
}

fn parse_input(input: List<string>, new_cat: fn(Map<Card, Int>) -> Category) -> List<Hand> {
    let mut hands = List::new();
    for line in input {
        let captures = line.captures("^([0-9TJQKA]{5}) (\\d+)$").unwrap();
        let card_strings = captures.get(1).unwrap().unwrap();
        let value = captures.get(2).unwrap().unwrap().parse_int().unwrap();
        let mut cards = List::new();
        for card in card_strings.chars_utf32() {
            let card = Card::from_string(card);
            cards.push(card.unwrap());
        }
        hands.push(Hand::new(cards, value, new_cat));
    }
    hands
}
