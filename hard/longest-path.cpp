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
      "qttiwkajeerhdgpikkeaaabwl",
      "vavprkykiloeizzt",
      "skwajgaaxqpfcxmadpwaraksnkbgcaukbgli",
      "kaja",
      "bjzanjikh",
    };
    std::vector<std::string> v_expect {
      "15",
      "11",
      "16",
      "3",
      "7",
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
      process(line);
    }
  }

  int trace(const std::vector<int> &v, int ms, int pos, int state) {
    if(state & v[pos]) {
      return 0;
    }
    int ret = 1;
    state |= v[pos];
    if(pos > ms) {
      ret = std::max(ret, 1 + trace(v, ms, pos - ms, state));
    }
    if(v.size() - pos > ms) {
      ret = std::max(ret, 1 + trace(v, ms, pos + ms, state));
    }
    auto col = pos % ms;
    if(col) {
      ret = std::max(ret, 1 + trace(v, ms, pos - 1, state));
    }
    if(col + 1 != ms) {
      ret = std::max(ret, 1 + trace(v, ms, pos + 1, state));
    }
    return ret;
  }

  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "', line.length() = " << line.length() << "\n";
#endif //#ifdef TEST
    // could use sqrt here
    int ms = 1;
    for(;ms*ms != line.length(); ++ms){}

    std::vector<int> mat(line.length());
    for(int i = 0; i < line.length(); ++i) {
      mat[i] = 1 << (line[i] - 'a');
      if(!mat[i]) {
        throw std::exception();
      }
    }

    int max = 1;
    for(int i = 0; i < mat.size(); ++i) {
      max = std::max(max, trace(mat, ms, i, 0));
    }
    std::cout << max << "\n";
  }
}
