#include <fstream>
#include <iostream>
#include <vector>

namespace {
#ifdef TEST
  void test();
#endif //#ifdef TEST
  void process_file(char*);
}

int main(int argc, char ** argv) {
#ifdef TEST
  test();
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

namespace {
  using std::string;
  using std::vector;

  void process(string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    auto sc = line.find(';');
    auto dir_name = line.substr(0, sc);
    auto sc2 = line.find(';', ++sc);
    auto dim = std::stoi( line.substr(sc, sc2 - sc) );
    auto pos = sc2 + 1;
    vector<int> m (dim * dim);
    for(auto & dst : m) {
      size_t ofs;
      dst = std::stoi(line.substr(pos), &ofs);
      pos += ofs + 1;
    }
    // transform
    const auto flip = 'R' == dir_name[0] || 'D' == dir_name[0];
    const auto turn = 'U' == dir_name[0] || 'D' == dir_name[0];
    if(turn) {
      for(int row = 0; row < dim; ++row) {
        for(int col = row; col < dim; ++col) {
          int idx1 = row * dim + col;
          int idx2 = row + dim * col;
          std::swap(m[idx1], m[idx2]);
        }
      }
    }
    if(flip) {
      for(int row = 0; row < dim; ++row) {
        for(int left = 0, right = dim - 1; left < right; ++left, --right) {
          int idx1 = row * dim + left;
          int idx2 = row * dim + right;
          std::swap(m[idx1], m[idx2]);
        }
      }
    }
    // now process assuming move direction is 'LEFT'
    for(int rownum = 0, base = 0; rownum < dim; ++rownum, base += dim) {
      // pass 1: move non-zero items left
      int dcol = 0;
      for(int col = 0; col < dim; ++col) {
        if(m[base + col]) {
          m[base + dcol] = m[base + col];
          ++dcol;
        }
      }
      for(; dcol < dim; ++dcol) {
        m[base + dcol] = 0;
      }
      // pass 2: combine equal non-zero items
      int scol = 0;
      for(dcol = 0; dcol < dim; ++dcol, ++scol) {
        if(scol >= dim) {
          m[base + dcol] = 0;
        } else {
          if(scol < dim - 1 && m[base + scol] == m[base + scol + 1]) {
            m[base + dcol] = 2 * m[base + scol];
            ++scol;
          } else {
            m[base + dcol] = m[base + scol];
          }
        }
      }
    }
    // reverse transform
    if(flip) {
      for(int row = 0; row < dim; ++row) {
        for(int left = 0, right = dim - 1; left < right; ++left, --right) {
          int idx1 = row * dim + left;
          int idx2 = row * dim + right;
          std::swap(m[idx1], m[idx2]);
        }
      }
    }
    if(turn) {
      for(int row = 0; row < dim; ++row) {
        for(int col = row; col < dim; ++col) {
          int idx1 = row * dim + col;
          int idx2 = row + dim * col;
          std::swap(m[idx1], m[idx2]);
        }
      }
    }
    // print results
    for(int i = 0; i < dim * dim; ++i) {
      if(i) {
        std::cout << (i%dim ? ' ' : '|');
      }
      std::cout << m[i];
    }
    std::cout << "\n";
  }

#ifdef TEST
  void test() {
    vector<string> v_test {
      "RIGHT; 4; 4 0 2 0|0 0 0 8|4 0 2 4|2 4 2 2",
      "UP; 4; 2 0 2 0|0 2 0 4|2 8 0 8|0 8 0 16",
    };
    vector<string> v_expect {
      "0 0 4 2|0 0 0 8|0 4 2 4|0 2 4 4",
      "4 2 2 4|0 16 0 8|0 0 0 16|0 0 0 0",
    };
    for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
      process(v_test[i]);
      std::cout << v_expect[i] << "\n";
    }
  }
#endif //#ifdef TEST

  void process_file(char* path) {
    std::ifstream stream(path);
    for(string line; std::getline(stream, line); ) {
      process(line);
    }
  }
}
