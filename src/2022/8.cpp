#include "aoc_includes.h"

void aoc(char *f) {
  const auto forest = process_input(f, [](const std::string &s) {
    std::vector<int> trees;
    std::transform(s.begin(), s.end(), std::back_inserter(trees),
                   [](const char c) { return (int)(c - '0'); });
    return trees;
  });
  // use vector<bool> for forest seen map, as it is specialized
  // to use bitpacking
  std::vector seen(forest.size(), std::vector<bool>(forest[0].size()));
  // left side
  for (int row = 0; row < forest.size(); row++) {
    
  }
}
