#include "aoc_includes.h"

struct Cave {
  std::string name;
  bool is_large;
  int visited;
};

class CaveSystem {
 public:
  CaveSystem(char *fpath);
  void explore(const std::string &name);
  int found = 0;
  int found2 = 0;

 private:
  bool twice = false;
  std::unordered_map<std::string, Cave> caves;
  std::unordered_map<std::string, std::vector<Cave *>> neighbors;
};

CaveSystem::CaveSystem(char *fpath) {
  std::ifstream in(fpath);

  for (char buf[MAX_LEN]; in.getline(buf, MAX_LEN);) {
    std::string s{buf};
    std::vector<std::string> connection;
    boost::split(connection, s, boost::algorithm::is_any_of("-"));
    for (auto &&cave_perms : iter::permutations(connection)) {
      if (!caves.contains(cave_perms[1])) {
        caves[cave_perms[1]] =
            Cave{cave_perms[1], cave_perms[1][0] < 'a', false};
      }
      neighbors[cave_perms[0]].emplace_back(&caves[cave_perms[1]]);
    }
  }
}

void CaveSystem::explore(const std::string &name) {
  if (++caves[name].visited > 1 && !caves[name].is_large) {
    twice = true;
  }
  for (auto &neighbor : neighbors[name]) {
    if (neighbor->name == "end") {
      if (!twice) {
        found++;
      }
      found2++;
    } else if (neighbor->name == "start") {
    } else if (neighbor->visited && !neighbor->is_large && twice) {
    } else {
      explore(neighbor->name);
    }
  }
  if (--caves[name].visited > 0 && !caves[name].is_large) {
    twice = false;
  }
}

void aoc(char *f) {
  CaveSystem cs{f};
  {
    MeasureTime m{"Total"};
    cs.explore("start");
  }
  fmt::print("Part 1: {}\n", cs.found);
  fmt::print("Part 2: {}\n", cs.found2);
}
