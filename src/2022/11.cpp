#include "aoc_includes.h"

int add(int x, int y) { return x + y; }

int mul(int x, int y) { return x * y; }

struct Operation {
  int (*op)(int, int);
  int rhs;
};

struct Monkey {
  std::vector<int> items;
  Operation op;
  int div_by;
  std::array<int, 2> throw_to;
};

auto local_process_input(char *f) {
  std::vector<Monkey> monkeys;
  std::ifstream in(f);
  char buf[MAX_LEN];

  while (in.getline(buf, MAX_LEN)) {  // Monkey X
    in.getline(buf, MAX_LEN);         // Starting items: ...
    Monkey monkey;
    std::string s(&buf[0] + strlen("  Starting Items: "));
    const auto r1 =
        scn::scan_list_ex(s, monkey.items, scn::list_separator(','));
    assert(r1);
    in.getline(buf, MAX_LEN);  // Operation: ...
    const auto &[r2, op, rhs] =
        scn::scan_tuple<char, std::string>(buf, "  Operation: new = old {} {}");
    assert(r2);
    if (op == '*') {
      monkey.op.op = mul;
    } else if (op == '+') {
      monkey.op.op = add;
    } else {
      assert(false);
    }
    try {
      monkey.op.rhs = std::stoi(rhs);
    } catch (std::invalid_argument &e) {
      // rhs = "old", for some reason compare to str "old" didn't work
      monkey.op.rhs = 0;
    }
    in.getline(buf, MAX_LEN);  // Test: divisible ...
    const auto r3 = scn::scan(buf, "  Test: divisible by {}", monkey.div_by);
    assert(r3);
    in.getline(buf, MAX_LEN);  // If true: ...
    const auto r4 =
        scn::scan(buf, "    If true: throw to monkey {}", monkey.throw_to[0]);
    assert(r4);
    in.getline(buf, MAX_LEN);  // If false: ...
    const auto r5 =
        scn::scan(buf, "    If false: throw to monkey {}", monkey.throw_to[1]);
    assert(r5);
    in.getline(buf, MAX_LEN);  // empty line
    monkeys.emplace_back(monkey);
  }

  return monkeys;
}

void aoc(char *f) {
  auto monkeys = local_process_input(f);
#if 0
  for (const auto &monkey : monkeys) {
    fmt::print(
        "items={}, op={}:{}, div_by={}, throw_to={}\n", monkey.items,
        (monkey.op.op == add ? "add" : (monkey.op.op == mul ? "mul" : "?")),
        monkey.op.rhs, monkey.div_by, monkey.throw_to);
  }
#endif

}
