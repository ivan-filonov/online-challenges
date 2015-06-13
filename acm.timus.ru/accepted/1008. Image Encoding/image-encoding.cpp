#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

vector<vector<int>> board;

int main() {
  string is[3];
  std::cin >> is[0] >> is[1] >> is[2];

  board.resize(12);
  for(auto & line : board) {
    line.resize(12);
    line.assign(12, 0);
  }

  if('.' == is[2].back() || ',' == is[2].back()) {
    // initial pixel + relates
    vector<int> x, y;
    x.push_back(std::stoi(is[0]));
    y.push_back(std::stoi(is[1]));
    int i = 0;
    for(string s = is[2]; s.back() != '.'; std::cin >> s) {
      if(s.find('R') != string::npos) {
        x.push_back(x[i] + 1);
        y.push_back(y[i] + 0);
      }
      if(s.find('T') != string::npos) {
        x.push_back(x[i] + 0);
        y.push_back(y[i] + 1);
      }
      if(s.find('L') != string::npos) {
        x.push_back(x[i] - 1);
        y.push_back(y[i] + 0);
      }
      if(s.find('B') != string::npos) {
        x.push_back(x[i] + 0);
        y.push_back(y[i] - 1);
      }
      ++i;
    }

    for(i = 0; i != x.size(); ++i) {
      board[y[i]][x[i]] = 1;
    }

    std::cout << x.size() << "\n";
    for(int h = 0; h < 11; ++h) {
      for(int v = 0; v < 11; ++v) {
        if(board[v][h]) {
          std::cout << h << ' ' << v << "\n";
        }
      }
    }
  } else {
    // pixel coords
    board[std::stoi(is[2])][std::stoi(is[1])] = 1;
    for(int i = 1, j = std::stoi(is[0]); i < j; ++i) {
      int x, y;
      std::cin >> x >> y;
      board[y][x] = 1;
    }

    vector<int> x(1), y(1);
    for(x[0] = 0; x[0] < 11; ++x[0]) {
      for(y[0] = 0; y[0] < 11; ++y[0]) {
        if(board[y[0]][x[0]]) {
          break;
        }
      }
      if(y[0] < 11) {
        break;
      }
    }
    std::cout << x[0] << ' ' << y[0] << "\n";
    board[y[0]][x[0]] = 0;

    for(int i = 0; i != x.size(); ++i) {
      x.push_back(x[i] + 1);
      y.push_back(y[i] + 0);
      if(!board[y.back()][x.back()]) {
        x.pop_back();
        y.pop_back();
      } else {
        board[y.back()][x.back()] = 0;
        std::cout << 'R';
      }
      x.push_back(x[i] + 0);
      y.push_back(y[i] + 1);
      if(!board[y.back()][x.back()]) {
        x.pop_back();
        y.pop_back();
      } else {
        board[y.back()][x.back()] = 0;
        std::cout << 'T';
      }
      x.push_back(x[i] - 1);
      y.push_back(y[i] + 0);
      if(!board[y.back()][x.back()]) {
        x.pop_back();
        y.pop_back();
      } else {
        board[y.back()][x.back()] = 0;
        std::cout << 'L';
      }
      x.push_back(x[i] + 0);
      y.push_back(y[i] - 1);
      if(!board[y.back()][x.back()]) {
        x.pop_back();
        y.pop_back();
      } else {
        board[y.back()][x.back()] = 0;
        std::cout << 'B';
      }
      std::cout << (i == x.size() - 1 ? '.' : ',') << "\n";
    }

  }

  return 0;
}
