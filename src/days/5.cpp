#include <algorithm>  // count_if
#include <cassert>
#include <numeric>  // transform_reduce
#include <regex>
#include <string>
#include <vector>

#include "aoc_util.h"
#include "fmt/core.h"
#include "fmt/ranges.h"
#include "itertools.hpp"

struct Point {
  int x;
  int y;
};

struct Line {
  Point begin;
  Point end;
};

int main(int argc, char *argv[]) {
  assert(argc == 2);

  int x_max = 0;
  int y_max = 0;

  const auto line_list = process_input(argv[1], [&](const std::string s) {
    // e.g. 674,489 -> 745,489
    const std::regex r(R"((\d+),(\d+) -> (\d+),(\d+))");
    std::smatch m;
    assert(std::regex_search(s, m, r));
    Line l;
    l.begin.x = stoi(m.str(1));
    if (l.begin.x > x_max) {
      x_max = l.begin.x;
    }
    l.begin.y = stoi(m.str(2));
    if (l.begin.y > y_max) {
      y_max = l.begin.y;
    }
    l.end.x = stoi(m.str(3));
    if (l.end.x > x_max) {
      x_max = l.end.x;
    }
    l.end.y = stoi(m.str(4));
    if (l.end.y > y_max) {
      y_max = l.end.y;
    }
    return l;
  });

  std::vector<std::vector<int>> grid(y_max + 1, std::vector<int>(x_max + 1, 0));

  for (const auto &line : line_list) {
    if (line.begin.x == line.end.x) {
      // found a vt line
      const int &x = line.begin.x;
      const int len = std::abs(line.end.y - line.begin.y) + 1;
      const int start = std::min(line.end.y, line.begin.y);
      for (int y = start; y < (start + len); y++) {
        grid[y][x]++;
      }
    }

    if (line.begin.y == line.end.y) {
      // found a hz line
      const int &y = line.begin.y;
      const int len = std::abs(line.end.x - line.begin.x) + 1;
      const int start = std::min(line.end.x, line.begin.x);
      for (int x = start; x < (start + len); x++) {
        grid[y][x]++;
      }
    }
  }

  fmt::print("Part 1: {}\n",
             std::transform_reduce(grid.begin(), grid.end(), 0, std::plus<>(),
                                   [](const std::vector<int> row) {
                                     return std::count_if(
                                         row.begin(), row.end(),
                                         [](const int el) { return el > 1; });
                                   }));

  return 0;
}
