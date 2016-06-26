#include <iostream>
#include <sstream>
#include <vector>

template <typename V> using vector = std::vector<V>;

static inline int ofs(int x, int y) { return x + y * 4; }

static inline void turn(std::string &s) {
  std::string r;
  r.assign(s.length(), '\0');
  for (int x = 0; x < 4; ++x) {
    for (int y = 0; y < 4; ++y) {
      int x1 = 3 - y;
      int y1 = x;
      r[ofs(x1, y1)] = s[ofs(x, y)];
    }
  }
	r.swap(s);
}

int main() {
#if ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::istringstream cin{"....\n"
                         "X..X\n"
                         ".X..\n"
                         "...X\n"
                         "Pwoo\n"
                         "Khaa\n"
                         "smrs\n"
                         "odbk\n"};
#endif
  std::string mask, data, t;
  for (int i = 0; i < 4; ++i) {
    cin >> t;
    mask.append(t);
  }
  for (int i = 0; i < 4; ++i) {
    cin >> t;
    data.append(t);
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 16; ++j) {
      if ('X' == mask[j]) {
        std::cout << data[j];
      }
    }
    turn(mask);
  }
  std::cout << "\n";
  return 0;
}
