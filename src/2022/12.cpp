#include "aoc_includes.h"

#define START_KEY 100
#define END_KEY 200
#define DIST_INF 4000  // max actual is 77 * 41 = 3,157

struct Location {
  int height;
  int r;
  int c;
  int distance = DIST_INF;
  bool visited = false;
  std::vector<Location *> adj;
  static Location *end;

  struct LocationCompare {
    bool operator()(const Location *lhs, const Location *rhs) const {
      if (lhs->distance == rhs->distance) {
        return lhs < rhs;
      } else {
        return lhs->distance < rhs->distance;
      }
    }
  };
  static std::set<Location *, LocationCompare> unvisited;

  Location(const int _height) : height(_height){};

  bool reachable(const Location &loc) const {
    return (loc.height - height) <= 1;
  }

  // Implement Dijkstra per
  // https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm#Algorithm
  void dijkstra(void) {
    unvisited.erase(unvisited.begin());
    for (Location *loc : adj) {
      if (loc->visited) {
        continue;
      }
      const int tentative_distance = distance + 1;
      if (tentative_distance < loc->distance) {
        loc->distance = tentative_distance;
      }
      unvisited.insert(loc);
    }
    visited = true;
    if (this == end) {
      fmt::print("Part 1: {}\n", distance);
      return;
    }
    assert(unvisited.size() > 0);
    (*unvisited.begin())->dijkstra();
  }
};
Location *Location::end;
std::set<Location *, Location::LocationCompare> Location::unvisited;

void aoc(char *f) {
  auto map = process_input(f, [](const auto &s) {
    std::vector<Location> row;
    std::transform(s.begin(), s.end(), std::back_inserter(row),
                   [](const char c) {
                     if (c == 'S') {
                       return Location(START_KEY);
                     } else if (c == 'E') {
                       return Location(END_KEY);
                     } else {
                       return Location(c - 'a');
                     }
                   });
    return row;
  });

  // finish setting up the map of Locations
  const auto ROWS = map.size();
  const auto COLS = map[0].size();
  Location *start = nullptr;
  for (size_t r = 0; r < ROWS; r++) {
    for (size_t c = 0; c < COLS; c++) {
      auto &here = map[r][c];
      here.r = r;
      here.c = c;
      if (here.height == START_KEY) {
        here.height = 'a' - 'a';
        start = &here;
      } else if (here.height == END_KEY) {
        here.height = 'z' - 'a';
        Location::end = &here;
      }  // else, no mod to height/start/end
      if (r > 0 && here.reachable(map[r - 1][c])) {
        here.adj.emplace_back(&map[r - 1][c]);
      }
      if (c > 0 && here.reachable(map[r][c - 1])) {
        here.adj.emplace_back(&map[r][c - 1]);
      }
      if (r < ROWS - 1 && here.reachable(map[r + 1][c])) {
        here.adj.emplace_back(&map[r + 1][c]);
      }
      if (c < COLS - 1 && here.reachable(map[r][c + 1])) {
        here.adj.emplace_back(&map[r][c + 1]);
      }
    }
  }

  // Starting node should have distance 0
  Location::unvisited.insert(start);
  start->distance = 0;
  start->dijkstra();
}
