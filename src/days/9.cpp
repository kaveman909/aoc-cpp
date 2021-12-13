#include "aoc_includes.h"

// These globals could be avoided but make accessing from find_valid_neighbors()
// more straightforward.
static int x_max;
static int y_max;
static std::vector<std::vector<int>> grid;

struct Point {
  int x;
  int y;
};

// Neighbor parameters
struct NP {
  bool condition;
  int x;
  int y;
};

int find_valid_neighbors(const int x, const int y) {
  int valid_neighbors = 1;
  // clear the current location to make sure we never revisit; as we
  // traverse the grid, things can get crazy.
  grid[y][x] = 0;
  std::array<NP, 4> nps{NP{x > 0, x - 1, y}, NP{x < (x_max - 1), x + 1, y},
                        NP{y > 0, x, y - 1}, NP{y < (y_max - 1), x, y + 1}};
  for (const auto &np : nps) {
    if (np.condition) {
      if ((grid[np.y][np.x] > grid[y][x]) && (grid[np.y][np.x] != 9)) {
        valid_neighbors += find_valid_neighbors(np.x, np.y);
      }
    }
  }
  return valid_neighbors;
}

void aoc(char *f) {
  grid = process_input(f, [](std::string s) {
    std::vector<int> v;
    std::transform(s.begin(), s.end(), std::back_inserter(v),
                   [](char c) { return (int)(c - '0'); });
    return v;
  });

  int pt1;
  int pt2;
  {
    MeasureTime m("Total");
    y_max = grid.size();
    x_max = grid.front().size();
    std::vector<int> risk_levels;
    std::vector<Point> basins;

    for (int y = 0; y < y_max; y++) {
      for (int x = 0; x < x_max; x++) {
        bool check = true;
        std::array<NP, 4> nps{
            NP{x > 0, x - 1, y}, NP{x < (x_max - 1), x + 1, y},
            NP{y > 0, x, y - 1}, NP{y < (y_max - 1), x, y + 1}};

        for (const auto &np : nps) {
          if (np.condition) {
            if (grid[y][x] >= grid[np.y][np.x]) {
              check = false;
            }
          }
        }

        if (check) {
          // found a low point
          risk_levels.emplace_back(grid[y][x] + 1);
          basins.emplace_back(Point{x, y});
        }
      }
    }
    pt1 = std::accumulate(risk_levels.begin(), risk_levels.end(), 0);

    std::vector<int> basin_sizes;
    for (const auto &basin : basins) {
      basin_sizes.emplace_back(find_valid_neighbors(basin.x, basin.y));
    }
    std::sort(basin_sizes.begin(), basin_sizes.end());
    pt2 = std::accumulate(basin_sizes.end() - 3, basin_sizes.end(), 1,
                          std::multiplies<>());
  }

  fmt::print("Part 1: {}\n", pt1);
  fmt::print("Part 2: {}\n", pt2);
}
