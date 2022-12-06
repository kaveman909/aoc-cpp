#include "aoc_includes.h"

struct Instruction {
  int move;
  int from;
  int to;
};

static auto local_process_input(char *fpath) {
  std::ifstream in(fpath);
  char buf[MAX_LEN];

  // (1) top of input file describes the initial stack states.
  std::vector<std::string> stack_input;
  while (in.getline(buf, MAX_LEN)) {
    if (strlen(buf) == 0) {
      break;
    }
    stack_input.emplace_back(buf);
  }
  std::vector<std::string> split_input;
  boost::split(split_input, stack_input.back(), boost::is_any_of(" "),
               boost::token_compress_on);
  auto iter = split_input.rbegin();
  while (iter->empty()) iter++;
  const int num_of_stacks = std::stoi(*iter);
  std::vector<std::stack<char>> stacks(num_of_stacks);
  const auto max_layer_idx = stack_input.size() - 2;

  for (int i = max_layer_idx; i >= 0; i--) {
    for (size_t j = 1; j < stack_input[i].size(); j += 4) {
      if (char c = stack_input[i][j]; c != ' ') {
        stacks[(j - 1) / 4].push(c);
      }
    }
  }

  // (2) remainder of file describes the stacking instructions.
  // put these into a data structure
  std::vector<Instruction> instructions;
  while (in.getline(buf, MAX_LEN)) {
    const auto [result, move, from, to] =
        scn::scan_tuple<int, int, int>(buf, "move {} from {} to {}");
    assert(result);
    instructions.emplace_back(Instruction{move, from - 1, to - 1});
  }
  return std::pair{stacks, instructions};
}

void aoc(char *f) {
  std::string tops;
  std::string tops1;
  {
    MeasureTime m{"Both Parts"};
    auto [stacks, instructions] = local_process_input(f);
    auto stacks1 = stacks;
    for (const auto &inst : instructions) {
      std::stack<char> temp;
      for (int i = 0; i < inst.move; i++) {
        stacks1[inst.to].push(stacks1[inst.from].top());
        stacks1[inst.from].pop();

        temp.push(stacks[inst.from].top());
        stacks[inst.from].pop();
      }
      for (int i = 0; i < inst.move; i++) {
        stacks[inst.to].push(temp.top());
        temp.pop();
      }
    }
    for (const auto &st : stacks) {
      tops += st.top();
    }
    for (const auto &st : stacks1) {
      tops1 += st.top();
    }
  }
  fmt::print("Part 1: {}\n", tops1);
  fmt::print("Part 2: {}\n", tops);
}
