#include <cassert>
#include <string>

#include "aoc_util.h"
#include "fmt/core.h"

static inline int dtoi(const std::string &str) {
  return *(str.end() - 1) - '0';
}

int main(int argc, char *argv[]) {
  assert(argc == 2);

  const auto directions = process_input(argv[1]);

  auto &npos = std::string::npos;
  int hz = 0;
  int aim_dp = 0;
  int dp = 0;

  for (const auto &dir : directions) {
    int x = dtoi(dir);

    if (dir[0] == 'f') {  // *f*orward
      hz += x;
      dp += x * aim_dp;
    } else if (dir[0] == 'u') {  // *u*p
      aim_dp -= x;
    } else {  // down
      aim_dp += x;
    }
  }

  fmt::print("Part 1: {}\n", hz * aim_dp);
  fmt::print("Part 2: {}\n", hz * dp);
}
