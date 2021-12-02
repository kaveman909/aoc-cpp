#include <cassert>
#include <string>

#include "aoc_util.h"
#include "fmt/core.h"

int main(int argc, char *argv[]) {
  assert(argc == 2);

  const auto directions = process_input(argv[1]);

  int hz = 0;
  int aim_dp = 0;
  int dp = 0;

  for (const auto &dir : directions) {
    int x = *(dir.end() - 1) - '0';

    if (dir.front() == 'f') {  // *f*orward
      hz += x;
      dp += x * aim_dp;
    } else if (dir.front() == 'u') {  // *u*p
      aim_dp -= x;
    } else {  // down
      aim_dp += x;
    }
  }

  fmt::print("Part 1: {}\n", hz * aim_dp);
  fmt::print("Part 2: {}\n", hz * dp);
}
