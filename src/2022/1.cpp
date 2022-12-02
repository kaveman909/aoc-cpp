#include "aoc_includes.h"

static auto local_process_input(char *fpath) {
  std::vector<std::vector<int>> weights(1);
  std::ifstream in(fpath);
  char buf[MAX_LEN];

  while (in.getline(buf, MAX_LEN)) {
    if (const auto [result, weight] = scn::scan_tuple<int>(buf, "{}"); result) {
      weights.back().emplace_back(weight);
    } else {
      weights.emplace_back(std::vector<int>{});
    }
  }

  return weights;
}

void aoc(char *f) {
  const auto weights = local_process_input(f);

  std::vector<int> sums;
  std::transform(
      weights.begin(), weights.end(), std::back_inserter(sums),
      [](auto in) { return std::accumulate(in.begin(), in.end(), 0); });
  std::sort(sums.begin(), sums.end());
  fmt::print("Part 1: {}\n", sums.back());
  fmt::print("Part 2: {}\n",
             std::accumulate(sums.rbegin(), sums.rbegin() + 3, 0));
}
