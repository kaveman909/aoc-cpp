#include <algorithm>
#include <cassert>
#include <numeric>
#include <string>
#include <vector>

#include "aoc_util.h"
#include "fmt/core.h"
#include "fmt/ranges.h"

int main(int argc, char *argv[]) {
  assert(argc == 2);

  const auto depths =
      process_input(argv[1], [](const std::string &x) { return std::stoi(x); });
  auto diffs = depths;
  std::adjacent_difference(diffs.begin(), diffs.end(), diffs.begin());
  const auto out = std::count_if(diffs.begin() + 1, diffs.end(),
                                 [](int e) { return e > 0; });
  fmt::print("Part 1: {}\n", out);
}
