//let input = File::read_to_string("sample.txt");
let input = File::read_to_string("input.txt");
let input = input.unwrap().trim().split("\n");
let games = parse_games(input);

print(f"part1: {part1(games)}");
print(f"part2: {part2(games)}");

struct Game {
    id: int,
    draws: List<Draw>,
}
struct Draw {
    red: int,
    green: int,
    blue: int,
}

fn parse_games(input: List<string>) -> List<Game> {
    let mut games = List::new();
    for game in input {
        games.push(parse_game(game));
    }
    games
}
fn parse_game(input: string) -> Game {
    let parts = input.captures("Game (\\d+): (.*)").unwrap();
    let id = parts.get(1).unwrap().unwrap().parse_int().unwrap();
    let mut draws = List::new();
    for draw in parts.get(2).unwrap().unwrap().split(";") {
        draws.push(parse_draw(draw));
    }
    Game { id: id, draws: draws }
}
fn parse_draw(input: string) -> Draw {
    let colors = input.split(",");
    let mut draw = Draw { red: 0, green: 0, blue: 0 };
    for color in colors {
        let captures = color.captures("(\\d+) (\\w+)").unwrap();
        let num = captures.get(1).unwrap().unwrap().parse_int().unwrap();
        let color = captures.get(2).unwrap().unwrap();
        match color {
            "red" => draw.red += num,
            "green" => draw.green += num,
            "blue" => draw.blue += num,
            _ => panic(f"unknown color {color}"),
        }
    }
    draw
}

fn part1(games: List<Game>) -> int {
    let mut sum = 0;
    'games: for game in games {
        for draw in game.draws {
            if draw.red > 12 || draw.green > 13 || draw.blue > 14 {
                continue 'games;
            }
        }
        sum += game.id;
    }
    sum
}

fn part2(games: List<Game>) -> int {
    let mut sum = 0;
    for game in games {
        let mut min = Draw { red: 0, green: 0, blue: 0 };
        for draw in game.draws {
            min.red = min.red.max(draw.red);
            min.green = min.green.max(draw.green);
            min.blue = min.blue.max(draw.blue);
        }
        let power = min.red * min.green * min.blue;
        sum += power;
    }
    sum
}

