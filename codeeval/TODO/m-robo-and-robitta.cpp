#include <string.h>

#include <fstream>
#include <iostream>
#include <vector>

#ifdef TEST
void test();
#endif //#ifdef TEST
void process_file(char *);

int main(int argc, char **argv) {
#ifdef TEST
  test();
#else  //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
  return 0;
}

using std::string;
// required when TEST is defined
template <typename V> using vector = std::vector<V>;

void process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  size_t ofs = 0, add;
  auto get_int = [&]() {
    while (!std::isdigit(line[ofs])) {
      ++ofs;
    }
    auto res = std::stoi(line.substr(ofs), &add);
    ofs += add;
    return res;
  };
  int width = get_int();
  int height = get_int();
  char map[50 * 50];
  memset(map, 0, 50 * 50);
  for (int v = 0; v < height; ++v) {
    memset(map + 50 * v, 1, width);
  }
  int dstx = get_int();
  int dsty = get_int();
  map[50 * dsty + dstx] = -1;
  int current[2]{0, 0};
  int dir = 0;
  int sum = 1;
  for (int i = 0; i < 20; ++i) {
    map[current[1] * 50 + current[0]] = 0;
    int next[2]{current[0], current[1]};
    int &coord = next[dir & 1];
    coord += (dir < 2) ? 1 : -1;
    if (-1 == coord || 50 == coord) {
      dir = (dir + 1) % 4;
      continue;
    }
    char v = map[next[1] * 50 + next[0]];
    if (!v) {
      dir = (dir + 1) % 4;
      continue;
    }
    if (v < 0) {
      break;
    }
    ++sum;
    current[0] = next[0];
    current[1] = next[1];
  }
  std::cout << sum << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test{
      "3x2 | 2 1", "4x4 | 3 3",
  };
  vector<string> v_expect{
      "5", "14",
  };
  for (int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
    process(v_test[i]);
    std::cout << v_expect[i] << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char *path) {
  std::ifstream stream(path);
  for (string line; std::getline(stream, line);) {
    process(line);
  }
}
