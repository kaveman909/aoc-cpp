#include "aoc_includes.h"

int priority(char c) {
  if (c >= 'a') {
    return c - 'a' + 1;
  }
  return c - 'A' + 27;
}

void aoc(char *f) {
  const auto rucksacks = process_input(f);
  int sum = 0;
  for (const auto &rucksack : rucksacks) {
    std::unordered_set<char> left_comp;
    const auto &half = rucksack.size() / 2;
    std::string_view left(rucksack.c_str(), half);
    std::string_view right(rucksack.c_str() + half, half);
    for (const auto item : left) {
      left_comp.insert(item);
    }
    for (const auto item : right) {
      if (left_comp.contains(item)) {
        sum += priority(item);
        break;
      }
    }
  }
  fmt::print("Part 1: {}\n", sum);
}
