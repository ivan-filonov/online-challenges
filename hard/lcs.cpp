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
  void process(std::string s);

#ifdef TEST
  void test() {
    std::vector<std::string> v_test {
      "XMJYAUZ;MZJAWXU"
    };
    std::vector<std::string> v_expect {
      "MJAU"
    };
    for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
      process(v_test[i]);
      std::cout << v_expect[i] << "\n";
    }
  }
#endif //#ifdef TEST

  void process_file(char* path) {
    std::ifstream stream(path);
    for(std::string line; std::getline(stream, line); ) {
      if(line.empty()) {
        continue;
      }
      process(line);
    }
  }

  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    auto delim = line.find(';');
    auto a = line.substr(0, delim);
    auto b = line.substr(delim + 1);

    const auto alen = a.length();
    const auto blen = b.length();

    std::vector<int> v((alen+1) * (blen+1), 0);
    for(int i = 0; i < alen; ++i) {
      for(int j = 0; j < blen; ++j) {
        const auto idx = i * blen + i + j;
        if(a[i] == b[j]) {
          v[idx + blen + 2] = v[idx] + 1;
        } else {
          v[idx + blen + 2] = std::max(v[idx + blen + 1], v[idx + 1]);
        }
      }
    }

    std::string res;
    for(auto i = alen, j = blen; i&&j;) {
      const auto idx = i * blen + i + j;
      const auto idx_i = idx - blen - 1;
      const auto idx_j = idx - 1;
      if(v[idx] == v[idx_i]) {
        --i;
      } else if(v[idx] == v[idx_j]) {
        --j;
      } else {
        if(a[i-1] != b[j-1]) {
          std::cout << "a[i-1] = '" << a[i-1] << "', b[j-1]  = '" << b[j-1] << "'\n";
          throw std::exception();
        }
        res += a[i-1];
        --i;
        --j;
      }
    }

    for(int i = 0; i < res.length() / 2; ++i) {
      std::swap(res[i], res[res.length() - i - 1]);
    }
    std::cout << res << "\n";
  }
}
