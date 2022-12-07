#include "aoc_includes.h"

class Dir {
 private:
  std::list<Dir> children;

 public:
  std::string name;
  Dir *parent;
  size_t this_dir_size = 0;
  size_t total_size = 0;
  static size_t global_sum;
  static size_t delete_target;

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

  size_t sum_files() {
    total_size = this_dir_size;
    for (auto &child : children) {
      total_size += child.sum_files();
    }
    if (total_size <= 100'000) {
      global_sum += total_size;
    }
    return total_size;
  }

  size_t find_delete_file(size_t current_min) {
    if (total_size >= delete_target) {
      // viable candiate for deletion
      current_min = std::min(current_min, total_size);
    }
    for (auto &child : children) {
      current_min = std::min(current_min, child.find_delete_file(current_min));
    }
    return current_min;
  }
};
size_t Dir::global_sum;
size_t Dir::delete_target;

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
      active_dir->this_dir_size += file_size;
    }
  }

  const auto top_level_size = root.sum_files();
  fmt::print("Part 1: {}\n", Dir::global_sum);

  const auto free_space_current = 70'000'000 - top_level_size;
  const auto delete_target = 30'000'000 - free_space_current;
  Dir::delete_target = delete_target;
  const auto size_delete_file =
      root.find_delete_file(std::numeric_limits<size_t>::max());
  fmt::print("Part 2: {}\n", size_delete_file);
}
