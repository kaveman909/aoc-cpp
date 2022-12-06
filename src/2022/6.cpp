#include "aoc_includes.h"
using namespace std::string_literals;

size_t get_marker(const std::string &signal, const size_t sz) {
  {
    MeasureTime m{"Marker Size = "s + std::to_string(sz)};
    for (size_t i = 0; i < signal.size() - sz; i++) {
      const std::string_view header(signal.begin() + i,
                                    signal.begin() + i + sz);
      std::unordered_set<char> set;
      for (const char c : header) {
        if (!set.emplace(c).second) {
          goto bottom;
        }
      }
      return i + sz;
    bottom:
      continue;
    }
  }
  return 0;
}

void aoc(char *f) {
  const auto signal = process_input(f)[0];
  fmt::print("Part 1: {}\n", get_marker(signal, 4));
  fmt::print("Part 2: {}\n", get_marker(signal, 14));
}
