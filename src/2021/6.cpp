#include "aoc_includes.h"

long simulate_days(int days, std::vector<int> fish_list) {
  // figure out the behavior of 1 fish
  // (uses a lot of memory, someday improve)
  std::vector<char> fishes(1, 1);
  std::vector<long> fish_num;

  for (int t = 0; t < days; t++) {
    size_t len = fishes.size();
    for (size_t i = 0; i < len; i++) {
      if (fishes[i] == 0) {
        fishes[i] = 6;
        fishes.emplace_back(8);
      } else {
        fishes[i]--;
      }
    }
    fish_num.emplace_back(fishes.size());
  }
  long sum = 0;
  for (const auto fish : fish_list) {
    sum += fish_num[days - fish];
  }
  return sum;
}

void aoc(char *f) {
  const auto timer_list = process_input(f);

  std::vector<std::string> timer_parsed;
  boost::split(timer_parsed, timer_list[0], boost::is_any_of(","));
  std::vector<int> fish_list;
  std::transform(timer_parsed.begin(), timer_parsed.end(),
                 std::back_inserter(fish_list),
                 [](std::string s) { return std::stoi(s); });

  MeasureTime m("Total");
  fmt::print("Part 1: {}\n", simulate_days(80, fish_list));
  fmt::print("Part 2: {}\n", simulate_days(256, fish_list));
}
