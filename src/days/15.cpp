#include "aoc_includes.h"
using namespace advent;

struct Loc {
  int risk;
  int risk_sum;
  std::vector<Loc *> nbrs;
};

void aoc(char *f) {
  auto grid = process_input(f, [](auto &s) {
    std::vector<Loc> row;
    std::for_each(s.begin(), s.end(), [&](auto ch) {
      row.emplace_back(Loc{.risk = ch - '0', .risk_sum = INT_MAX});
    });
    return row;
  });

  // Expand the grid per Part 2
  const int EXPAND_SIZE = 5;
  const int y_max_orig = grid.size();
  const int x_max_orig = grid.front().size();
  // First, expand in the x-direction
  for (auto &row : grid) {
    for (int i = 1; i < EXPAND_SIZE; i++) {
      std::transform(row.begin(), row.begin() + x_max_orig,
                     std::back_inserter(row), [&](auto el) {
                       el.risk = ((el.risk - 1 + i) % 9) + 1;
                       return el;
                     });
    }
  }
  // Next, expand in the y-direction
  for (int i = 1; i < EXPAND_SIZE; i++) {
    for (int y = 0; y < y_max_orig; y++) {
      std::vector<Loc> new_row;
      std::transform(grid[y].begin(), grid[y].end(),
                     std::back_inserter(new_row), [&](auto el) {
                       el.risk = ((el.risk - 1 + i) % 9) + 1;
                       return el;
                     });
      grid.emplace_back(new_row);
    }
  }

  std::deque<Loc *> unvisited;
  const int y_max = grid.size();
  const int x_max = grid.front().size();
  const auto loc_end = &grid[y_max_orig - 1][x_max_orig - 1];
  const auto loc_end2 = &grid[y_max - 1][x_max - 1];

  // Determine neighboring points and cache these for later traversal
  for (int y = 0; y < y_max; y++) {
    for (int x = 0; x < x_max; x++) {
      std::array<NP, 4> nps{NP{x > 0, x - 1, y}, NP{x < (x_max - 1), x + 1, y},
                            NP{y > 0, x, y - 1}, NP{y < (y_max - 1), x, y + 1}};
      for (const auto &np : nps) {
        if (np.condition) {
          grid[y][x].nbrs.emplace_back(&grid[np.y][np.x]);
        }
      }
    }
  }

  // Prime the algorithm by setting cost to the starting node to 0
  grid[0][0].risk_sum = 0;
  unvisited.emplace_back(&grid[0][0]);

  while (1) {
    // (1) set current location to lowest tentative risk
    std::sort(unvisited.begin(), unvisited.end(),
              [](const auto &loc1, const auto &loc2) {
                return loc1->risk_sum < loc2->risk_sum;
              });
    const auto current_loc = unvisited.front();
    if (current_loc == loc_end) {
      fmt::print("Part 1: {}\n", current_loc->risk_sum);
    } else if (current_loc == loc_end2) {
      fmt::print("Part 2: {}\n", current_loc->risk_sum);
      break;
    }

    // (2) update the neighbor risk sums from our current location
    for (auto &nbr : current_loc->nbrs) {
      if (const auto cost_to_nbr = current_loc->risk_sum + nbr->risk;
          cost_to_nbr < nbr->risk_sum) {
        nbr->risk_sum = cost_to_nbr;
        if (std::find(unvisited.begin(), unvisited.end(), nbr) ==
            unvisited.end()) {
          unvisited.emplace_back(nbr);
        }
      }
    }

    // (3) pop the visited location
    unvisited.pop_front();
  }
}
