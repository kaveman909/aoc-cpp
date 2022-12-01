#include "aoc_includes.h"

void process_input(char *fpath, std::vector<int> &draws,
                   std::vector<std::vector<int>> &boards) {
  std::ifstream in(fpath);
  const int MAX_LEN = 300;
  std::vector<int> board;

  for (auto [buf, i] = std::tuple<char[MAX_LEN], int>{};
       in.getline(buf, MAX_LEN); i++) {
    auto buf_s = std::string{buf};
    boost::trim(buf_s);
    if (i == 0) {
      std::vector<std::string> draws_s;
      boost::split(draws_s, buf_s, boost::is_any_of(","));
      std::transform(draws_s.begin(), draws_s.end(), std::back_inserter(draws),
                     [](std::string s) { return std::stoi(s); });
    } else {
      if (!buf_s.empty()) {
        std::vector<std::string> board_row;
        boost::split(board_row, buf_s, boost::is_any_of(" "),
                     boost::algorithm::token_compress_on);
        std::transform(board_row.begin(), board_row.end(),
                       std::back_inserter(board),
                       [](std::string s) { return std::stoi(s); });
      } else {
        if (!board.empty()) {
          boards.emplace_back(board);
          board.clear();
        }
      }
    }
  }
  // get the last board since there's no empty line at the end of the file
  boards.emplace_back(board);  // no need to clear 'board' since we're done
}

struct Board {
  std::array<int, 5> rows;
  std::array<int, 5> cols;
};

void aoc(char *f) {
  MeasureTime m{"Total"};

  std::vector<int> draws;
  std::vector<std::vector<int>> boards;
  process_input(f, draws, boards);

  std::vector<Board> trackers;
  trackers.resize(boards.size());
  std::vector<std::array<int, 25>> called;
  called.resize(boards.size());
  std::vector<int> winners;
  winners.resize(boards.size());
  bool found = false;
  bool last_found = false;

  for (const auto &draw : draws) {
    for (auto &&[board, tracker, call, winner] :
         iter::zip(boards, trackers, called, winners)) {
      const auto find_res = std::find(board.begin(), board.end(), draw);
      if (find_res != board.end()) {
        const auto pos = find_res - board.begin();
        call[pos] = 1;
        const auto row = pos / 5;
        const auto col = pos % 5;
        tracker.rows[row]++;
        tracker.cols[col]++;
        if (!winner && (tracker.rows[row] == 5 || tracker.cols[col] == 5)) {
          // found the winner
          winner = 1;
          const auto winner_sum =
              std::accumulate(winners.begin(), winners.end(), 0);
          if (static_cast<size_t>(winner_sum) == boards.size()) {
            last_found = true;
          }
          if (!found || last_found) {
            int unmarked_sum = 0;
            for (auto &&[call_el, board_el] : iter::zip(call, board)) {
              if (call_el == 0) {
                unmarked_sum += board_el;
              }
            }
            if (!found) {
              fmt::print("Part 1: {}\n", unmarked_sum * draw);
              found = true;
            } else if (last_found) {
              fmt::print("Part 2: {}\n", unmarked_sum * draw);
              goto exit;
            }
          }
        }
      }
    }
  }

exit:
  (void)0;
}