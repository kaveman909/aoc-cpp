#include "aoc_includes.h"

struct Cave {
  std::string name;
  bool is_large;
  bool visited;
};

class CaveSystem {
 public:
  CaveSystem(char *f);
  void explore(const std::string &name);
  int found = 0;

 private:
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
  caves[name].visited = true;
  for (auto &neighbor : neighbors[name]) {
    if (neighbor->name == "end") {
      found++;
    } else if (neighbor->visited && !neighbor->is_large) {
    } else {
      explore(neighbor->name);
    }
  }
  caves[name].visited = false;
}

void aoc(char *f) {
  CaveSystem cs{f};
  cs.explore("start");
  fmt::print("Part 1: {}\n", cs.found);
}
