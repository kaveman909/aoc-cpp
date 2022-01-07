#include "aoc_includes.h"

void aoc(char *f) {
  scn::owning_file file{f, "r"};
  // e.g. target area: x=269..292, y=-68..-44
  const auto [result, x_min, x_max, y_min, y_max] =
      scn::scan_tuple<int, int, int, int>(file,
                                          "target area: x={}..{}, y={}..{}");

  int y_max_cand = 0;
  for (int y_veli = 1; y_veli < 100; y_veli++) {
    int y = 0;
    int y_max2 = 0;
    int y_vel = y_veli;
    while (y >= y_min) {
      y += y_vel;
      if (y > y_max2) {
        y_max2 = y;
      }
      y_vel -= 1;
      if ((y <= y_max) && (y >= y_min)) {
        y_max_cand = y_max2;
        break;
      }
    }
  }
  fmt::print("Part 1: {}\n", y_max_cand);
}
