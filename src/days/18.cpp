#include "aoc_includes.h"

using namespace std;

static void add_snailfish_numbers(const string &left, string &right) {
  right = "[" + left + "," + right + "]";
}

static auto get_explode_index(const string &str) {
  int ei;
  int eie;
  for (size_t i = 0, count = 0; const auto &ch : str) {
    if (ch == '[') {
      if (++count == 5) {
        ei = i;
      }
    } else if (ch == ']') {
      if (count-- == 5) {
        eie = i;
        return tuple{ei, eie};
      }
    }
    i++;
  }
  return tuple{-1, -1};
}

static void replace_last(const int left, int &ei, int &eie, string &str) {
  int original_val = 0;
  int rep_len;
  int i;
  for (i = ei - 1; i >= 0; i--) {
    if (isdigit(str[i])) {
      if (i > 0 && isdigit(str[i - 1])) {
        original_val = atoi(str.c_str() + i - 1);
        rep_len = 2;
        i--;
      } else {
        original_val = atoi(str.c_str() + i);
        rep_len = 1;
      }
      break;
    }
  }
  if (i >= 0) {
    const int new_val = left + original_val;
    if ((new_val >= 10) && (rep_len == 1)) {
      ei++;
      eie++;
    }
    str.replace(i, rep_len, to_string(new_val));
  }
}

static void replace_first(int right, const int eie, string &str) {
  int original_val = 0;
  int rep_len = 0;
  int i;
  for (i = eie + 1; i < (int)str.size(); i++) {
    if (isdigit(str[i])) {
      original_val = atoi(str.c_str() + i);
      if (i < (int)(str.size() - 1) && isdigit(str[i + 1])) {
        rep_len = 2;
      } else {
        rep_len = 1;
      }
      break;
    }
  }
  if (i < (int)str.size()) {
    const int new_val = right + original_val;
    str.replace(i, rep_len, to_string(new_val));
  }
}

static void explode_pair(int ei, int eie, string &str) {
  const auto [result, left, right] =
      scn::scan_tuple<int, int>(str.substr(ei), "[{},{}]");

  replace_last(left, ei, eie, str);
  replace_first(right, eie, str);
  str.replace(ei, eie - ei + 1, "0");
}

static bool split_into_pair(string &str) {
  for (int i = 0; i < (int)str.size(); i++) {
    if (int num = atoi(str.c_str() + i); num >= 10) {
      // found first number >= 10 starting from left
      int left;
      int right;
      if ((num % 2) == 0) {
        // even
        left = right = num / 2;
      } else {
        left = num / 2;
        right = left + 1;
      }
      str.replace(i, 2, fmt::format("[{},{}]", left, right));
      return true;
    }
  }
  return false;
}

void aoc(char *f) {
  scn::owning_file file{f, "r"};
  vector<string> homework;
  scn::scan_list(file, homework, '\n');

  for (size_t i = 0; i < homework.size() - 1; i++) {
    auto &to_reduce = homework[i + 1];
    add_snailfish_numbers(homework[i], to_reduce);
    while (1) {
      auto [ei, eie] = get_explode_index(to_reduce);
      if (ei != -1) {
        explode_pair(ei, eie, to_reduce);
        continue;
      }
      if (split_into_pair(to_reduce)) {
        continue;
      }
      break;
    }
  }
  auto &hw_sum = homework.back();
  while (auto match = ctre::search<R"(\[(\d+),(\d+)\])">(hw_sum)) {
    const int magnitude =
        3 * atoi(&*match.get<1>().begin()) + 2 * atoi(&*match.get<2>().begin());
    hw_sum.replace(match.begin(), match.end(), to_string(magnitude));
  }
  fmt::print("Part 1: {}\n", hw_sum);
}
