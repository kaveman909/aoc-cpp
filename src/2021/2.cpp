#include "aoc_includes.h"

void aoc(char *f) {
  const auto directions = process_input(f);

  int hz = 0;
  int aim_dp = 0;
  int dp = 0;

  for (const auto &dir : directions) {
    const int x = dir.back() - '0';
    const auto instr = dir.front();

    if (instr == 'f') {  // forward
      hz += x;
      dp += x * aim_dp;
    } else if (instr == 'u') {  // up
      aim_dp -= x;
    } else {  // down
      aim_dp += x;
    }
  }

  fmt::print("Part 1: {}\n", hz * aim_dp);
  fmt::print("Part 2: {}\n", hz * dp);
}
