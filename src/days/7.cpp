#include "aoc_includes.h"

int minimize_fuel(int (*fn)(int), std::vector<int> crab_positions, int min_pos,
                  int max_pos) {
  auto min_fuel = std::numeric_limits<int>::max();

  for (int line = min_pos; line <= max_pos; line++) {
    const auto fuel = std::transform_reduce(
        crab_positions.begin(), crab_positions.end(), 0, std::plus<>(),
        [&](int crab_pos) { return fn(std::abs(crab_pos - line)); });
    if (fuel < min_fuel) {
      min_fuel = fuel;
    }
  }
  return min_fuel;
}

void aoc(char *f) {
  const auto input = process_input(f);

  std::vector<std::string> input_parsed;
  boost::split(input_parsed, input[0], boost::is_any_of(","));
  std::vector<int> crab_positions;
  std::transform(input_parsed.begin(), input_parsed.end(),
                 std::back_inserter(crab_positions),
                 [](std::string s) { return std::stoi(s); });

  const auto min_pos =
      *std::min_element(crab_positions.begin(), crab_positions.end());
  const auto max_pos =
      *std::max_element(crab_positions.begin(), crab_positions.end());

  fmt::print("Part 1: {}\n", minimize_fuel([](int dist) { return dist; },
                                           crab_positions, min_pos, max_pos));
  fmt::print("Part 2: {}\n",
             minimize_fuel([](int dist) { return (dist * (dist + 1)) / 2; },
                           crab_positions, min_pos, max_pos));
}
