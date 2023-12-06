//let input = File::read_to_string("sample.txt");
let input = File::read_to_string("input.txt");
let input = input.unwrap().trim();

let input = parse_input(input);

print(f"part1: {part1(input)}");
print(f"part2: {part2(input)}");

fn part1(input: Input) -> int {
    let mut mapped = List::new();
    for mut num in input.seeds {
        let mut source = "seed";
        loop {
            let mut map = input.mappings.get(source).unwrap();
            num = map.map(num);
            source = map.to;
            if source == "location" {
                mapped.push(num);
                break;
            }
        }
    }
    mapped.min().unwrap()
}
fn part2(input: Input) -> int {
    let mut ranges = List::new();
    for i in List::range(0, input.seeds.len() / 2) {
        let start = input.seeds.get(i*2).unwrap();
        let len = input.seeds.get(i*2 + 1).unwrap();
        ranges.push(Range {
            start: start,
            end: start + len,
        });
    }
    let mut source = "seed";
    loop {
        let mut map = input.mappings.get(source).unwrap();
        ranges = map.map_ranges(ranges);
        source = map.to;
        if source == "location" {
            break;
        }
    }
    ranges.min().unwrap().start
}

struct Input {
    seeds: List<int>,
    mappings: Map<string, Mapping>,
}
struct Mapping {
    from: string,
    to: string,
    range_maps: List<RangeMap>,
}
impl Mapping {
    fn map(self, num: int) -> int {
        for range in self.range_maps {
            match range.map(num) {
                Option::Some(dest) => return dest,
                None => (),
            }
        }
        num
    }
    fn map_ranges(self, ranges: List<Range>) -> List<Range> {
        let mut mapped = List::new();
        for mut rest in ranges {
            for range in self.range_maps {
                let res = range.map_range(rest);
                if res.mapped.is_some() {
                    mapped.push(res.mapped.unwrap());
                }
                match res.unmapped {
                    Option::Some(unmapped) => rest = unmapped,
                    Option::None => {
                        rest = Range { start: 0, end: 0 };
                        break;
                    }
                }
            }
            if rest.len() > 0 {
                mapped.push(rest);
            }
        }
        mapped
    }
}
struct Range {
    /// inclusive
    start: int,
    /// exclusive
    end: int,
}
impl Range {
    fn len(self) -> int {
        self.end - self.start
    }
}
struct MapRangeResult {
    mapped: Option<Range>,
    unmapped: Option<Range>,
}
struct RangeMap {
    source: Range,
    destination: Range,
}
impl RangeMap {
    fn map(self, num: int) -> Option<int> {
        if self.source.start <= num && num < self.source.end {
            Option::Some(num - self.source.start + self.destination.start)
        } else {
            Option::None
        }
    }
    fn map_range(self, range: Range) -> MapRangeResult {
        let start_mapped = self.map(range.start);
        let end_mapped = self.map(range.end);
        if start_mapped.is_some() && end_mapped.is_some() {
            MapRangeResult {
                mapped: Option::Some(Range { start: start_mapped.unwrap(), end: end_mapped.unwrap() }),
                unmapped: Option::None,
            }
        } else if start_mapped.is_some() && end_mapped.is_none() {
            MapRangeResult {
                mapped: Option::Some(Range { start: start_mapped.unwrap(), end: self.destination.end }),
                unmapped: Option::Some(Range { start: self.source.end, end: range.end }),
            }
        } else if start_mapped.is_none() && end_mapped.is_some() {
            MapRangeResult {
                mapped: Option::Some(Range { start: self.destination.start, end: end_mapped.unwrap() }),
                unmapped: Option::Some(Range { start: range.start, end: self.source.start }),
            }
        } else {
            MapRangeResult {
                mapped: Option::None,
                unmapped: Option::Some(range),
            }
        }
    }
}

fn parse_input(input: string) -> Input {
    let blocks = input.split("\n\n");
    let mut seeds = List::new();
    for seed in blocks.get(0).unwrap().find_matches("\\d+") {
        seeds.push(seed.parse_int().unwrap());
    }

    let mut mappings = Map::new();
    for i in List::range(1, blocks.len()) {
        let mapping = parse_mapping(blocks.get(i).unwrap().trim());
        mappings.insert(mapping.from, mapping);
    }

    Input { seeds: seeds, mappings: mappings }
}

fn parse_mapping(map: string) -> Mapping {
    let lines = map.trim().split("\n");
    let captures = lines.get(0).unwrap().captures("^(\\w+)-to-(\\w+) map:$").unwrap();
    let from = captures.get(1).unwrap().unwrap();
    let to = captures.get(2).unwrap().unwrap();

    let mut range_maps = List::new();
    for i in List::range(1, lines.len()) {
        range_maps.push(parse_range(lines.get(i).unwrap()));
    }
    range_maps.sort();
    Mapping { from: from, to: to, range_maps: range_maps }
}

fn parse_range(line: string) -> RangeMap {
    let captures = line.captures("^(\\d+) (\\d+) (\\d+)$").unwrap();
    let dest = captures.get(1).unwrap().unwrap().parse_int().unwrap();
    let source = captures.get(2).unwrap().unwrap().parse_int().unwrap();
    let len = captures.get(3).unwrap().unwrap().parse_int().unwrap();
    RangeMap {
        destination: Range { start: dest, end: dest + len },
        source: Range { start: source, end: source + len },
    }
}
