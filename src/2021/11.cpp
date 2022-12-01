#include "aoc_includes.h"

using namespace advent;

void aoc(char *f) {
  auto grid = process_input(f, [](std::string s) {
    std::vector<int> v;
    std::transform(s.begin(), s.end(), std::back_inserter(v),
                   [](char c) { return (int)(c - '0'); });
    return v;
  });

  int flashed_sum = 0;
  int i = 0;
  while (1) {
    i++;
    // first increment all by 1
    std::for_each(grid.begin(), grid.end(), [](std::vector<int> &row) {
      std::for_each(row.begin(), row.end(), [](int &el) { el += 1; });
    });

    bool flashed;
    int flashed_this_step = 0;
    do {
      flashed = false;
      for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
          if (grid[y][x] > 9) {
            grid[y][x] = 0;
            flashed_sum++;
            flashed_this_step++;
            std::array nps{NP{x > 0, x - 1, y},
                           NP{x < 9, x + 1, y},
                           NP{y > 0, x, y - 1},
                           NP{y < 9, x, y + 1},
                           NP{x > 0 && y > 0, x - 1, y - 1},
                           NP{x > 0 && y < 9, x - 1, y + 1},
                           NP{x < 9 && y > 0, x + 1, y - 1},
                           NP{x < 9 && y < 9, x + 1, y + 1}};
            for (const auto &np : nps) {
              if (np.condition) {
                if (grid[np.y][np.x] != 0) {
                  (grid[np.y][np.x]++);
                  flashed = true;
                }
              }
            }
          }
        }
      }
    } while (flashed == true);
    if (i == 100) {
      fmt::print("Part 1: {}\n", flashed_sum);
    }
    if (flashed_this_step == 100) {
      fmt::print("Part 2: {}\n", i);
      break;
    }
  }
}
