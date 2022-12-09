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
  const size_t ROWS = fvec.size();
  const size_t COLS = fvec[0].size();

  ForestMatrix forest(ROWS, COLS);
  for (size_t r = 0; r < ROWS; r++) {
    for (size_t c = 0; c < COLS; c++) {
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

  int max_scenic_score = 0;
  for (int r = 0; r < (int)ROWS; r++) {
    for (int c = 0; c < (int)COLS; c++) {
      const int tree = forest(r, c).height;
      std::array<int, 4> sides = {0, 0, 0, 0};
      // down
      for (int rf = r + 1; rf < (int)ROWS; rf++) {
        sides[0]++;
        if (forest(rf, c).height >= tree) {
          break;
        }
      }
      // up
      for (int rb = r - 1; rb >= 0; rb--) {
        sides[1]++;
        if (forest(rb, c).height >= tree) {
          break;
        }
      }
      // right
      for (int cf = c + 1; cf < (int)COLS; cf++) {
        sides[2]++;
        if (forest(r, cf).height >= tree) {
          break;
        }
      }
      // left
      for (int cb = c - 1; cb >= 0; cb--) {
        sides[3]++;
        if (forest(r, cb).height >= tree) {
          break;
        }
      }
      const int scenic_score =
          std::accumulate(sides.begin(), sides.end(), 1, std::multiplies<>());
      max_scenic_score = std::max(scenic_score, max_scenic_score);
    }
  }
  fmt::print("Part 2: {}\n", max_scenic_score);
}
