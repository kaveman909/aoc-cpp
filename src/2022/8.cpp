#include "Eigen/Dense"
#include "aoc_includes.h"

using namespace Eigen;

struct Location {
  int height;
  bool visible;
};

typedef Matrix<Location, Dynamic, Dynamic> ForestMatrix;

void determine_visible(auto forest_slice) {
  for (auto slice : forest_slice) {
    int max_height_seen = -1;  // always see the edges
    for (Location &tree : slice) {
      if (tree.height > max_height_seen) {
        tree.visible = true;
        max_height_seen = tree.height;
      }  // else, do nothing
      if (max_height_seen == 9) {
        // no way to see past a '9' building
        break;
      }
    }
  }
}

void aoc(char *f) {
  const auto fvec = process_input(f, [](const std::string &s) {
    std::vector<int> trees;
    std::transform(s.begin(), s.end(), std::back_inserter(trees),
                   [](const char c) { return (int)(c - '0'); });
    return trees;
  });

  ForestMatrix forest(fvec.size(), fvec[0].size());
  for (size_t r = 0; r < fvec.size(); r++) {
    for (size_t c = 0; c < fvec[0].size(); c++) {
      forest(r, c) = Location{fvec[r][c], false};
    }
  }

  determine_visible(forest.rowwise());
  determine_visible(forest.colwise());
  determine_visible(forest.reverse().rowwise());
  determine_visible(forest.reverse().colwise());

  int total_visible = 0;
  for (auto row : forest.rowwise()) {
    total_visible +=
        std::transform_reduce(row.begin(), row.end(), 0, std::plus<>(),
                              [](const Location &a) { return a.visible; });
  }
  fmt::print("Part 1: {}\n", total_visible);
}
