#include "aoc_includes.h"

using namespace advent;

enum class Fold { X, Y };

int parse_fold_location(const std::string &s) {
  std::vector<std::string> parsed;
  boost::split(parsed, s, boost::algorithm::is_any_of("="));
  return std::stoi(parsed[1]);
}

auto local_process_input(char *fpath) {
  std::vector<Point> points;
  std::vector<Fold> folds;
  Point first_fold{};
  bool folds_start = false;

  std::ifstream in(fpath);

  for (char buf[MAX_LEN]; in.getline(buf, MAX_LEN);) {
    std::string s{buf};
    if (folds_start) {
      if (s[11] == 'x') {
        folds.emplace_back(Fold::X);
        if (!first_fold.x) {
          first_fold.x = parse_fold_location(s);
        }
      } else {
        folds.emplace_back(Fold::Y);
        if (!first_fold.y) {
          first_fold.y = parse_fold_location(s);
        }
      }
    } else if (s.empty()) {
      folds_start = true;
    } else {
      std::vector<std::string> parsed;
      boost::split(parsed, s, boost::algorithm::is_any_of(","));
      points.emplace_back(Point{std::stoi(parsed[0]), std::stoi(parsed[1])});
    }
  }

  return std::tuple{first_fold, points, folds};
}

void aoc(char *f) {
  auto [current_fold, points, folds] = local_process_input(f);
  bool pt1 = true;

  for (auto &fold : folds) {
    for (auto &point : points) {
      if ((fold == Fold::X) && (point.x > current_fold.x)) {
        point.x -= 2 * (point.x - current_fold.x);
      } else if ((fold == Fold::Y) && (point.y > current_fold.y)) {
        point.y -= 2 * (point.y - current_fold.y);
      }
    }
    // remove duplicates
    std::sort(points.begin(), points.end());
    points.erase(std::unique(points.begin(), points.end()), points.end());
    if (fold == Fold::X) {
      current_fold.x /= 2;
    } else {
      current_fold.y /= 2;
    }
    if (pt1) {
      pt1 = false;
      fmt::print("Part 1: {}\n", points.size());
    }
  }
  std::vector<std::vector<char>> grid(
      current_fold.y * 2 + 1, std::vector<char>(current_fold.x * 2 + 1, ' '));
  for (auto &point : points) {
    grid[point.y][point.x] = '#';
  }
  fmt::print("Part 2:\n");
  for (auto &row : grid) {
    for (auto &el : row) {
      fmt::print("{}", el);
    }
    fmt::print("\n");
  }
}
