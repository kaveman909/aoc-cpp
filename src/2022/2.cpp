#include "aoc_includes.h"

struct Moves {
  int ours;
  int theirs;
};

static int get_points(const Moves &moves) {
  constexpr std::array<std::array<int, 3>, 3> point_map = {
      {{3, 0, 6}, {6, 3, 0}, {0, 6, 3}}};

  const auto match_points = point_map[moves.ours][moves.theirs];
  const auto choice_points = moves.ours + 1;

  return match_points + choice_points;
}

void aoc(char *f) {
  (void)f;

  const auto guide = process_input(f, [](const std::string &s) {
    const auto [result, theirs, ours] = scn::scan_tuple<char, char>(s, "{} {}");
    return Moves{ours - 'X', theirs - 'A'};
  });

  const auto part1 =
      std::transform_reduce(guide.begin(), guide.end(), 0, std::plus<>(),
                            [](const Moves &m) { return get_points(m); });

  fmt::print("Part 1: {}\n", part1);
}
