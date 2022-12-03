#include "aoc_includes.h"

int priority(char c) {
  if (c >= 'a') {
    return c - 'a' + 1;
  }
  return c - 'A' + 27;
}

std::string rucksack_compare(const std::string_view ruck_a,
                             const std::string_view ruck_b) {
  std::unordered_set<char> ruck_a_set;
  std::unordered_set<char> overlap_set;
  std::string overlap;
  for (const auto item : ruck_a) {
    ruck_a_set.insert(item);
  }
  for (const auto item : ruck_b) {
    if (ruck_a_set.contains(item)) {
      overlap_set.insert(item);
    }
  }
  for (const auto item : overlap_set) {
    overlap += item;
  }
  return overlap;
}

void aoc(char *f) {
  const auto rucksacks = process_input(f);
  int sum = 0;
  {
    MeasureTime m("Part 1");
    for (const auto &rucksack : rucksacks) {
      const auto &half = rucksack.size() / 2;
      const std::string_view left(rucksack.c_str(), half);
      const std::string_view right(rucksack.c_str() + half, half);
      sum += priority(rucksack_compare(left, right)[0]);
    }
  }
  fmt::print("Part 1: {}\n", sum);

  int sum2 = 0;
  assert(rucksacks.size() % 3 == 0);
  {
    MeasureTime m("Part 2");
    for (size_t i = 0; i < rucksacks.size(); i += 3) {
      const auto comp1 = rucksack_compare(rucksacks[i], rucksacks[i + 1]);
      const auto comp2 = rucksack_compare(comp1, rucksacks[i + 2]);
      sum2 += priority(comp2[0]);
    }
  }
  fmt::print("Part 2: {}\n", sum2);
}
