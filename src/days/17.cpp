#include "aoc_includes.h"

void aoc(char *f) {
  scn::owning_file file{f, "r"};
  // e.g. target area: x=269..292, y=-68..-44
  const auto [result, x_min, x_max, y_min, y_max] =
      scn::scan_tuple<int, int, int, int>(file,
                                          "target area: x={}..{}, y={}..{}");

  int y_max_cand = 0;
  int y_veli_max;
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
        y_veli_max = y_veli;
        break;
      }
    }
  }
  fmt::print("Part 1: {}\n", y_max_cand);

  // Range for x
  // fastest we can go would be the far right edge after 1 step
  const int &x_vel_upper = x_max;
  // slowest we can go is if we barely make it to left edge before stopping
  // given that x_vel decreases by one each step, we can think of this as
  // n + (n - 1) + ... + 1 >= x_min, or, restated:
  // 1 + 2 + ... + n >= x_min
  int x_sum = 0;
  int n = 0;
  while (x_sum < x_min) {
    n++;
    x_sum = (n * (n + 1)) / 2;
  }
  const int &x_vel_lower = n;

  // Range for y
  // we already know the upper bound from part 1
  const int &y_vel_upper = y_veli_max;
  // lowest we can go is lower edge after 1 step
  const int &y_vel_lower = y_min;
  int total_hits = 0;
  // it is obvious that not every single initial vel is a candidate in the
  // range, however the optimization here isn't worth it as it already runs
  // fast.
  for (int yvi = y_vel_lower; yvi <= y_vel_upper; yvi++) {
    for (int xvi = x_vel_lower; xvi <= x_vel_upper; xvi++) {
      int y_vel = yvi;
      int x_vel = xvi;
      int y = 0;
      int x = 0;
      while (x < x_max && y > y_min) {
        x += x_vel;
        y += y_vel;
        if (x <= x_max && x >= x_min && y <= y_max && y >= y_min) {
          total_hits++;
          break;
        }
        // update velocities
        y_vel -= 1;
        if (x_vel > 0) {
          x_vel -= 1;
        }
      }
    }
  }
  fmt::print("Part 2: {}\n", total_hits);
}
