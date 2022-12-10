#include "aoc_includes.h"

void aoc(char *f) {
  const auto instructions = process_input(f, [](const auto &s) {
    const auto &[result, _, val] =
        scn::scan_tuple<std::string, int>(s, "{} {}");
    return result ? val : 0;
  });
  int cycle_count = 0;
  int x = 1;
  int next_threshold = 20;
  int signal_strength_sum = 0;
  for (const auto instr : instructions) {
    if (0 == instr) {
      cycle_count++;
    } else {
      cycle_count += 2;
    }
    if (cycle_count >= next_threshold) {
      signal_strength_sum += (x * next_threshold);
      next_threshold += 40;
    }
    x += instr;
  }
  fmt::print("Part 1: {}\n", signal_strength_sum);
}
