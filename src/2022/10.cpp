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
  fmt::print("Part 2:\n");
  for (const auto instr : instructions) {
    const int cycles = (0 == instr) ? 1 : 2;
    for (int i = 0; i < cycles; i++) {
      if (abs(x - (cycle_count % 40)) <= 1) {
        fmt::print("#");
      } else {
        fmt::print(" ");
      }
      cycle_count++;
      if (0 == (cycle_count % 40)) {
        fmt::print("\n");
      }
    }
    if (cycle_count >= next_threshold) {
      signal_strength_sum += (x * next_threshold);
      next_threshold += 40;
    }
    x += instr;
  }
  fmt::print("Part 1: {}\n", signal_strength_sum);
}
