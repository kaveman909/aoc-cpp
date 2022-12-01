#include "aoc_includes.h"

static inline auto find_open(const char test_char) {
  return std::string{"([{<"}.find(test_char);
}

static inline auto find_closed(const char test_char) {
  return std::string{")]}>"}.find(test_char);
}

void aoc(char *f) {
  const auto in = process_input(f);
  std::array<int, 4> points_map{3, 57, 1197, 25137};
  int syntax_error_score = 0;
  std::vector<long> auto_complete_scores;

  for (const auto &line : in) {
    std::stack<size_t> brack_stack;
    bool corrupted = false;

    for (const auto &ch : line) {
      if (const auto open_char = find_open(ch);
          open_char != std::string::npos) {
        brack_stack.push(open_char);
      } else {
        const auto close_char = find_closed(ch);
        const auto last_open_char = brack_stack.top();
        if (close_char != last_open_char) {
          corrupted = true;
          syntax_error_score += points_map[close_char];
          break;
        }
        brack_stack.pop();
      }
    }
    if (!corrupted) {
      // incomplete lines
      long score = 0;
      while (!brack_stack.empty()) {
        const auto current_char = brack_stack.top();
        score = (score * 5) + current_char + 1;
        brack_stack.pop();
      }
      auto_complete_scores.emplace_back(score);
    }
  }

  fmt::print("Part 1: {}\n", syntax_error_score);

  std::sort(auto_complete_scores.begin(), auto_complete_scores.end());
  fmt::print("Part 2: {}\n",
             auto_complete_scores[auto_complete_scores.size() / 2]);
}
