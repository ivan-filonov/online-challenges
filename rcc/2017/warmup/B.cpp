#include <iostream>
#include <set>
#include <vector>
int main() {
  using N = int32_t;
  std::cin.sync_with_stdio(false);

  N rows, passengers;
  std::cin >> rows >> passengers;

  std::set<N> left, right;
  for (N i = 1; i <= rows; ++i) {
    left.insert(i);
    right.insert(i);
  }

  const char RED = 1;
  const char BLUE = 2;
  const char BLACK = 4;
  const char YELLOW = 8;

  std::vector<uint8_t> p;
  p.assign(passengers, 0);

  N nred = 0, nblue = 0, nblack = 0, nyellow = 0;
  for (N i = 0; i < passengers; ++i) {
    N row, side;
    std::cin >> row >> side;
    bool free_before = row > *left.begin() || row > *right.begin();
    bool free_after = row < *left.rbegin() || row < *right.rbegin();
    // 1. RED or BLUE: no free spaces before:
    if (!free_before) {
      if (1 == side || !left.count(row)) {
        p[i] |= RED;
        ++nred;
      }
      if (2 == side || !right.count(row)) {
        p[i] |= BLUE;
        ++nblue;
      }
    }
    // 2. BLACK or YELLOW: no free spaces after
    if (!free_after) {
      if (1 == side || !left.count(row)) {
        p[i] |= BLACK;
        ++nblack;
      }
      if (2 == side || !right.count(row)) {
        p[i] |= YELLOW;
        ++nyellow;
      }
    }
    (side == 1 ? left : right).erase(row);
  }
  // RED
  std::cout << nred;
  if (nred) {
    for (N i = 1; i <= passengers; ++i) {
      if (p[i - 1] & RED) {
        std::cout << ' ' << i;
      }
    }
  }
  std::cout << "\n";
  // BLUE
  std::cout << nblue;
  if (nblue) {
    for (N i = 1; i <= passengers; ++i) {
      if (p[i - 1] & BLUE) {
        std::cout << ' ' << i;
      }
    }
  }
  std::cout << "\n";
  // BLACK
  std::cout << nblack;
  if (nblack) {
    for (N i = 1; i <= passengers; ++i) {
      if (p[i - 1] & BLACK) {
        std::cout << ' ' << i;
      }
    }
  }
  std::cout << "\n";
  // YELLOW
  std::cout << nyellow;
  if (nyellow) {
    for (N i = 1; i <= passengers; ++i) {
      if (p[i - 1] & YELLOW) {
        std::cout << ' ' << i;
      }
    }
  }
  std::cout << "\n";
  // PINK
  std::cout << passengers;
  for (N i = 1; i <= passengers; ++i) {
    std::cout << ' ' << i;
  }
  std::cout << "\n";
}
