#include <algorithm>
#include <array>
#include <deque>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>

bool test_move (const std::string& state, int x, int y, char dir, std::string& new_state)
{
  const int  xx = x;
  const int  yy = y;
  const char bot = state[x + 5 * y];
  const bool can_win = bot == 'X';
  const int  dx = dir == 'L' ? -1 : dir == 'R' ? 1 : 0;
  const int  dy = dir == 'U' ? -1 : dir == 'D' ? 1 : 0;
  int        num_space = 0;
  bool       have_obstacle = false;
  for (;;) {
    x += dx;
    y += dy;
    if (x < 0 || y < 0 || x >= 5 || y >= 5) {
      break;
    }
    const char cell = state[x + y * 5];
    //    if (can_win && cell == '.' && x == 2 && y == 2) {
    //      new_state = state;
    //      new_state.at (xx + yy * 5) = '.';
    //      new_state.at (x + y * 5) = bot;
    //      return true;
    //    }
    if (cell == '.') {
      num_space += 1;
    } else {
      have_obstacle = true;
      break;
    }
  }
  if (!have_obstacle || num_space == 0) {
    return false;
  }
  new_state = state;
  new_state.at (xx + yy * 5) = '.';
  new_state.at (x - dx + (y - dy) * 5) = bot;
  return true;
}

static const std::string directions = "DLRU";

int main ()
{
  std::string field[5];
#ifndef TEST
  for (int i = 0; i < 5; ++i) {
    std::getline (std::cin, field[i]);
  }
#else
  field[0] = ".....";
  field[1] = "A.C..";
  field[2] = "..D..";
  field[3] = "..X..";
  field[4] = "B....";
#endif

  for (auto& s : field) {
    std::cerr << s << std::endl;
  }

  const auto initial_state = field[0] + field[1] + field[2] + field[3] + field[4];
  std::cerr << initial_state << std::endl;

  std::unordered_map<std::string, std::string> prev;
  std::unordered_map<std::string, std::string> move;
  std::deque<std::string>                      queue;

  prev[initial_state] = "";
  queue.push_back (initial_state);

  while (!queue.empty ()) {
    const auto current = queue.front ();
    queue.pop_front ();
    if (current[2 + 2 * 5] == 'X') {
      // victory
      std::cerr << "WIN: " << current << std::endl;

      auto                    c = current;
      std::deque<std::string> commands;
      while (!c.empty ()) {
        commands.push_front (move[c]);
        c = prev[c];
      }

      for (int i = 1; i < commands.size (); ++i) {
        std::cout << (i > 1 ? " " : "") << commands[i];
      }
      std::cout << std::endl;
      for (int i = 0; i < 5; ++i) {
        std::cout << current.substr (i * 5, 5) << std::endl;
      }
      break;
    }

    std::array<int, 6> x, y;
    x.fill (-1);
    for (int i = 0; i < 25; ++i) {
      if (current[i] == '.') {
        continue;
      }
      int index = 5;
      switch (current[i]) {
      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'E': index = current[i] - 'A'; break;
      }
      x[index] = i % 5;
      y[index] = i / 5;
    }
    std::string new_state = current;
    if (x[5] == 2) {
      const char dir = y[5] > 2 ? 'U' : 'D';
      if (test_move (current, x[5], y[5], dir, new_state)) {
        if (!prev.count (new_state)) {
          queue.push_back (new_state);
          using namespace std::string_literals;
          prev[new_state] = current;
          move[new_state] = "X"s + dir;
        }
      }
    }
    if (y[5] == 2) {
      const char dir = x[5] > 2 ? 'L' : 'R';
      if (test_move (current, x[5], y[5], dir, new_state)) {
        if (!prev.count (new_state)) {
          queue.push_back (new_state);
          using namespace std::string_literals;
          prev[new_state] = current;
          move[new_state] = "X"s + dir;
        }
      }
    }
    for (int i = 0; i <= 5; ++i) {
      if (x[i] < 0) {
        continue;
      }
      for (char dir : directions) {
        if (test_move (current, x[i], y[i], dir, new_state)) {
          if (!prev.count (new_state)) {
            queue.push_back (new_state);
            using namespace std::string_literals;
            prev[new_state] = current;
            move[new_state] = ""s + current[x[i] + 5 * y[i]] + dir;
          }
        }
      }
    }
  }

  return 0;
}
