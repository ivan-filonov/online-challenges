#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

bool test(int x, int y) {
  return x >= 0 && y >= 0 && x <= 7 && y <= 7;
}

int main() {
  std::cin.sync_with_stdio(false);

  int n;
  std::cin >> n;

  for(string spos; n-->0 && std::cin >> spos; ) {
    int col = spos[0] - 'a';
    int line = spos[1] - '1';
    int res = 0;
    for(int sx = -1; sx <= 1; sx += 2) {
      for(int sy = -1; sy <= 1; sy += 2) {
        if(test(col + 2 * sx, line + sy)) {
          ++res;
        }
        if(test(col + sx, line + 2 * sy)) {
          ++res;
        }
      }
    }
    std::cout << res << "\n";
  }

  return 0;
}
