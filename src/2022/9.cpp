#include "aoc_includes.h"
#include "boost/functional/hash.hpp"

typedef std::pair<int, int> Coord;
struct Instruction {
  Coord direction;
  int spaces;
};

static int sign(const int in) { return (in > 0) ? 1 : ((in < 0) ? -1 : 0); }

void aoc(char *f) {
  auto instructions = process_input(f, [](const std::string &s) {
    const auto [result, direction, spaces] =
        scn::scan_tuple<char, int>(s, "{} {}");
    assert(result);
    const std::unordered_map<char, Coord> m = {
        {'L', {-1, 0}}, {'R', {1, 0}}, {'U', {0, -1}}, {'D', {0, 1}}};
    return Instruction{m.at(direction), spaces};
  });

  std::array<Coord, 10> knots;  // will be default-constructed with 0s
  std::unordered_set<Coord, boost::hash<Coord>> visited1 = {knots[1]};
  std::unordered_set<Coord, boost::hash<Coord>> visited2 = {knots.back()};

  {
    MeasureTime m{"Both Parts"};
    for (const auto &instruction : instructions) {
      for (int i = 0; i < instruction.spaces; i++) {
        // update head knot
        knots.front().first += instruction.direction.first;
        knots.front().second += instruction.direction.second;
        // propogate movement to the other 9 knots
        for (size_t j = 1; j < knots.size(); j++) {
          auto &head = knots[j - 1];
          auto &tail = knots[j];
          const auto x_diff = head.first - tail.first;
          const auto y_diff = head.second - tail.second;
          // check if nearest tail knot should move
          if (std::abs(x_diff) <= 1 && std::abs(y_diff) <= 1) {
            break;
          } else {
            tail.first += sign(x_diff);
            tail.second += sign(y_diff);
          }
        }
        // try adding tail location to set
        visited1.insert(knots[1]);
        visited2.insert(knots.back());
      }
    }
  }
  fmt::print("Part 1: {}\n", visited1.size());
  fmt::print("Part 2: {}\n", visited2.size());
}
