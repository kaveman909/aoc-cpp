#include "aoc_includes.h"

size_t get_marker(const std::string &signal, size_t sz) {
  for (size_t i = 0; i < signal.size() - sz; i++) {
    std::string_view header(signal.begin() + i, signal.begin() + i + sz);
    std::unordered_set<char> set;
    for (const char c : header) {
      set.emplace(c);
    }
    if (set.size() == sz) {
      return i + sz;
    }
  }
  return 0;
}

void aoc(char *f) {
  const auto signal = process_input(f)[0];
  fmt::print("Part 1: {}\n", get_marker(signal, 4));
  fmt::print("Part 2: {}\n", get_marker(signal, 14));
}
