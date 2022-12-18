#include "aoc_includes.h"

long add(long x, long y) { return x + y; }

long mul(long x, long y) { return x * y; }

struct Operation {
  long (*op)(long, long);
  int rhs;
};

struct Monkey {
  int id;
  std::list<int> items;
  Operation op;
  int div_by;
  std::array<int, 2> throw_to;
};

auto local_process_input(char *f) {
  std::vector<Monkey> monkeys;
  std::ifstream in(f);
  char buf[MAX_LEN];
  int current_monkey = 0;

  while (in.getline(buf, MAX_LEN)) {  // Monkey X
    Monkey monkey;
    monkey.id = current_monkey++;
    in.getline(buf, MAX_LEN);  // Starting items: ...
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

long simulate_monkeys(std::vector<Monkey> monkeys, const int num_rounds,
                      const int div) {
  std::vector<long> inspection_counts(monkeys.size(), 0);
  const long mod_reduce = std::transform_reduce(
      monkeys.begin(), monkeys.end(), 1, std::multiplies<>(),
      [](const auto &a) { return a.div_by; });

  for (int rounds = 0; rounds < num_rounds; rounds++) {
    for (auto &monkey : monkeys) {
      inspection_counts[monkey.id] += monkey.items.size();
      for (auto &item : monkey.items) {
        long worry_level = item;
        const long rhs = monkey.op.rhs ? monkey.op.rhs : worry_level;
        worry_level = monkey.op.op(worry_level, rhs);
        worry_level /= div;
        if (worry_level % monkey.div_by == 0) {
          monkeys[monkey.throw_to[0]].items.emplace_back(worry_level %
                                                         mod_reduce);
        } else {
          monkeys[monkey.throw_to[1]].items.emplace_back(worry_level %
                                                         mod_reduce);
        }
      }
      // after all items are thrown, monkey is holding nothing, therefore
      // clear the list
      monkey.items.clear();
    }
  }
  std::sort(inspection_counts.begin(), inspection_counts.end());
  return inspection_counts.back() * *(inspection_counts.rbegin() + 1);
}

void aoc(char *f) {
  auto monkeys = local_process_input(f);

  fmt::print("Part 1: {}\n", simulate_monkeys(monkeys, 20, 3));
  fmt::print("Part 2: {}\n", simulate_monkeys(monkeys, 10'000, 1));
}
