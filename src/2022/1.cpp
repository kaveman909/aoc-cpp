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
  const auto max = std::reduce(
      weights.begin(), weights.end(), std::vector<int>{},
      [&](const auto a, const auto b) {
        const auto max_ab = std::max(std::accumulate(a.begin(), a.end(), 0),
                                     std::accumulate(b.begin(), b.end(), 0));
        return std::vector{max_ab};
      });
  fmt::print("Part 1: {}\n", max[0]);
}
