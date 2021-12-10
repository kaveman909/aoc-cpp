#include "aoc_includes.h"

struct Point {
  int x;
  int y;
};

struct Line {
  Point begin;
  Point end;
};

int find_overlapping(std::vector<Line> line_list, Point max, bool pt2) {
  std::vector<std::vector<int>> grid(max.y + 1, std::vector<int>(max.x + 1, 0));

  for (const auto &line : line_list) {
    const Point diff{std::abs(line.end.x - line.begin.x),
                     std::abs(line.end.y - line.begin.y)};
    if (diff.x == 0) {
      // found a vt line
      const int &x = line.begin.x;
      const int len = diff.y + 1;
      const int start = std::min(line.end.y, line.begin.y);
      for (int y = start; y < (start + len); y++) {
        grid[y][x]++;
      }
    } else if (diff.y == 0) {
      // found a hz line
      const int &y = line.begin.y;
      const int len = diff.x + 1;
      const int start = std::min(line.end.x, line.begin.x);
      for (int x = start; x < (start + len); x++) {
        grid[y][x]++;
      }
    } else if ((diff.x == diff.y) && pt2) {
      // found a diag line
      const int len = diff.x + 1;
      const Point dir{(line.end.x > line.begin.x) ? 1 : -1,
                      (line.end.y > line.begin.y) ? 1 : -1};
      for (int i = 0, x = line.begin.x, y = line.begin.y; i < len; i++) {
        grid[y][x]++;
        x += dir.x;
        y += dir.y;
      }
    }
  }

  return std::transform_reduce(grid.begin(), grid.end(), 0, std::plus<>(),
                               [](const std::vector<int> row) {
                                 return std::count_if(
                                     row.begin(), row.end(),
                                     [](const int el) { return el > 1; });
                               });
}

void aoc(char *f) {
  Point max{0, 0};

  const auto line_list = process_input(f, [&](const std::string s) {
    // e.g. 674,489 -> 745,489
    const std::regex r(R"((\d+),(\d+) -> (\d+),(\d+))");
    std::smatch m;
    assert(std::regex_search(s, m, r));
    Line l;
    l.begin.x = stoi(m.str(1));
    if (l.begin.x > max.x) {
      max.x = l.begin.x;
    }
    l.begin.y = stoi(m.str(2));
    if (l.begin.y > max.y) {
      max.y = l.begin.y;
    }
    l.end.x = stoi(m.str(3));
    if (l.end.x > max.x) {
      max.x = l.end.x;
    }
    l.end.y = stoi(m.str(4));
    if (l.end.y > max.y) {
      max.y = l.end.y;
    }
    return l;
  });

  fmt::print("Part 1: {}\n", find_overlapping(line_list, max, false));
  fmt::print("Part 2: {}\n", find_overlapping(line_list, max, true));
}
