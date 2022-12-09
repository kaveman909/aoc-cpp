#include "aoc_includes.h"
#include "boost/functional/hash.hpp"

typedef std::pair<int, int> Coord;
struct Instruction {
  Coord direction;
  int spaces;
};

void aoc(char *f) {
  auto instructions = process_input(f, [](const std::string &s) {
    const auto [result, direction, spaces] =
        scn::scan_tuple<char, int>(s, "{} {}");
    assert(result);
    const std::unordered_map<char, Coord> m = {
        {'L', {-1, 0}}, {'R', {1, 0}}, {'U', {0, -1}}, {'D', {0, 1}}};
    return Instruction{m.at(direction), spaces};
  });

  Coord head = {0, 0};
  Coord tail = {0, 0};
  std::unordered_set<Coord, boost::hash<Coord>> visited = {tail};

  for (const auto &instruction : instructions) {
    for (int i = 0; i < instruction.spaces; i++) {
      // update head
      head.first += instruction.direction.first;
      head.second += instruction.direction.second;
      // check if tail must move
      if (std::abs(head.first - tail.first) > 1 ||
          std::abs(head.second - tail.second) > 1) {
        tail.first = head.first - instruction.direction.first;
        tail.second = head.second - instruction.direction.second;
      }
      // try adding tail location to set
      visited.insert(tail);
    }
  }
  fmt::print("Part 1: {}\n", visited.size());
}
