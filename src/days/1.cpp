#include <algorithm>
#include <cassert>
#include <numeric>
#include <string>
#include <vector>

#include "aoc_util.h"
#include "fmt/core.h"
#include "fmt/ranges.h"

auto diff_and_count(const std::vector<int> &in) {
  std::vector<int> diffs;
  std::adjacent_difference(in.begin(), in.end(), std::back_inserter(diffs));
  const auto out = std::count_if(diffs.begin() + 1, diffs.end(),
                                 [](int e) { return e > 0; });
  return out;
}

int main(int argc, char *argv[]) {
  assert(argc == 2);

  // Part 1
  const auto depths =
      process_input(argv[1], [](const std::string &x) { return std::stoi(x); });
  fmt::print("Part 1: {}\n", diff_and_count(depths));

  // Part 2
  std::vector<int> sums;
  // Add first and second
  std::transform(depths.begin(), depths.end() - 2, depths.begin() + 1,
                 std::back_inserter(sums), std::plus<int>());
  // now add the third to the existing sum
  std::transform(sums.begin(), sums.end(), depths.begin() + 2, sums.begin(),
                 std::plus<int>());
  fmt::print("Part 2: {}\n", diff_and_count(sums));
}
