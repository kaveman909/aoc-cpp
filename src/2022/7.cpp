#include "aoc_includes.h"

class Dir {
 private:
  std::list<Dir> children;

 public:
  std::string name;
  Dir *parent;
  size_t total_size = 0;
  static size_t global_sum;

  Dir(const std::string &_name, Dir *_parent) : name(_name), parent(_parent) {}

  Dir *get_child(const std::string &child_name) {
    auto child_location = std::find_if(
        children.begin(), children.end(),
        [&](const Dir &child) { return child.name == child_name; });
    if (child_location != children.end()) {
      return &(*child_location);
    }
    return nullptr;
  }

  Dir *add_child(const std::string &child_name) {
    children.emplace_back(Dir(child_name, this));
    return &children.back();
  }

  // Prints info via DFS
  void print_tree(const int level) const {
    fmt::print("{}\t{}\t{}\n", name, total_size, level);
    for (auto &child : children) {
      child.print_tree(level + 1);
    }
  }

  size_t sum_files(void) {
    size_t local_sum = total_size;
    for (auto &child : children) {
      local_sum += child.sum_files();
    }
    if (local_sum <= 100'000) {
      global_sum += local_sum;
    }
    return local_sum;
  }
};
size_t Dir::global_sum;

void aoc(char *f) {
  const auto cmds = process_input(f);
  Dir root("/", nullptr);
  Dir *active_dir = &root;
  // buil the dir tree
  for (const auto &cmd : cmds) {
    // -- cd
    if (const auto &[result, dir_name] =
            scn::scan_tuple<std::string>(cmd, "$ cd {}");
        result) {
      if (dir_name == "/") {
        // special case, do nothing
        continue;
      } else if (dir_name == "..") {
        // go up one level to the parent
        active_dir = active_dir->parent;
      } else {
        // go down one level into new dir
        active_dir = active_dir->get_child(dir_name);
        assert(active_dir != nullptr);
      }
      // -- ls
    } else if (cmd.starts_with("$")) {
      // --- dir
    } else if (const auto &[result, dir_name] =
                   scn::scan_tuple<std::string>(cmd, "dir {}");
               result) {
      active_dir->add_child(dir_name);
      // --- file
    } else if (const auto &[result, file_size, file_name] =
                   scn::scan_tuple<size_t, std::string>(cmd, "{} {}");
               result) {
      // (ignore the file_name for now, doesn't apply to pt 1)
      active_dir->total_size += file_size;
    }
  }

  root.sum_files();
  fmt::print("Part 1: {}\n", Dir::global_sum);
}
