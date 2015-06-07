#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#ifdef TEST
void test();
#endif //#ifdef TEST
void process_file(char*);

int main(int argc, char ** argv) {
#ifdef TEST
  test();
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

using std::string;
template<typename V> using vector = std::vector<V>;

vector<vector<char>> m;

void add_line(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
//    std::istringstream ss { line };
  m.emplace_back(line.length());
  std::copy(line.begin(), line.end(), m.back().begin());
}

int bit(int c) {
  return 1 << (c - 'a');
}

void run() {
  const auto mhei = m.size();
  const auto mwid = m[0].size();
  vector<int> line_masks(mhei);
  int rsize = 2;
  vector<int> res;
  for(size_t left = 0; left != mwid; ++left) {
    // limit columns as [left, left + h]
    for(size_t h = 0; left + h != mwid && h != 26; ++h) {
      const auto width = h + 1;

      for(size_t line = 0; line != mhei; ++line) {
        int mask = 0;
        for(size_t c = 0; c != width; ++c) {
          int b = bit(m[line][left + c]);
          if(mask & b) {
            mask = -1;
            break;
          }
          mask |= b;
        }
        line_masks[line] = mask;
      }

      const auto max_height = 26 / width;
      for(size_t top = 0, height; top != mhei; ++top) {
        if(-1 == line_masks[top]) {
          continue;
        }
        int mask = 0;
        // limit lines as [top, top + height)
        for(height = 0; height != max_height && height + top != mhei; ++height) {
          if(mask & line_masks[top + height]) {
            break;
          }
          mask |= line_masks[top + height];
        }
        ;
        int size = width * height;
        if(size < rsize) {
          continue;
        }
        if(size > rsize) {
          rsize = size;
          res.clear();
        }
        res.push_back(left);
        res.push_back(width);
        res.push_back(top);
        res.push_back(height);
      }
    }
  }
  for(size_t i = 0; i != res.size(); i += 4) {
    auto left = res[i + 0];
    auto width = res[i + 1];
    auto top = res[i + 2];
    auto height = res[i + 3];
    for(size_t v = 0; v != height; ++v) {
      auto & line = m[top + v];
      for(size_t h = 0; h != width; ++h) {
        line[left + h] = '*';
      }
    }
  }
  for(auto & line : m) {
    for(auto c : line) {
      std::cout << c;
    }
    std::cout << "\n";
  }
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "rzqicaiiaege",
    "ccwnulljybtu",
    "jxtxupauwuah",
    "oqikzgqrzpdq",
    "vblalwdjbdwn",
    "ahjeencuclbo",
  };
  for(auto & t : v_test) {
    add_line(t);
  }
  for(int i = 0; i < 80; ++i)std::cout<<'-';std::cout<<"\n";
  run();
  for(int i = 0; i < 80; ++i)std::cout<<'-';std::cout<<"\n";
  vector<string> v_expect {
    "rzqicaiiae**",
    "ccwnulljyb**",
    "jxtx***uwu**",
    "oqik****zp**",
    "vbla****bd**",
    "ahje****cl**",
  };
  for(auto & e : v_expect) {
    std::cout << e << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char* path) {
  std::ifstream stream(path);
  for(string line; std::getline(stream, line); ) {
    add_line(line);
  }
  run();
}
