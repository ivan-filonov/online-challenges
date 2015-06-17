#include <iostream>
#include <vector>
/*
  Challenge Description:

  A robot is located in the upper-left corner of a 4×4 grid. The robot can move either up, down, left, or right, but cannot go to the same location twice. The robot is trying to reach the lower-right corner of the grid. Your task is to find out the number of unique ways to reach the destination.
  Input sample:

  There is no input for this program.
  Output sample:

  Print out the number of unique ways for the robot to reach its destination. The number should be printed out as an integer ≥0. 
*/
template<typename V> using vector = std::vector<V>;

int main() {
  vector<int> v { 1, 0, 0 };
  int count = 0;
  while(!v.empty()) {
    const size_t base = v.size() - 3;
    int mask = v[base];
    int x = v[base + 1];
    int y = v[base + 2];
    v.resize(base);

    if(3 == x && 3 == y) {
      ++count;
      continue;
    }

    if(x > 0) {
      int bit = 1 << (x + 4 * y - 1);
      if(!(bit&mask)) {
        v.push_back(bit | mask);
        v.push_back(x - 1);
        v.push_back(y);
      }
    }

    if(y > 0) {
      int bit = 1 << (x + 4 * y - 4);
      if(!(bit&mask)) {
        v.push_back(bit | mask);
        v.push_back(x);
        v.push_back(y - 1);
      }
    }

    if(x < 3) {
      int bit = 1 << (x + 4 * y + 1);
      if(!(bit&mask)) {
        v.push_back(bit | mask);
        v.push_back(x + 1);
        v.push_back(y);
      }
    }

    if(y < 3) {
      int bit = 1 << (x + 4 * y + 4);
      if(!(bit&mask)) {
        v.push_back(bit | mask);
        v.push_back(x);
        v.push_back(y + 1);
      }
    }
  }
  std::cout << count;
  return 0;
}

