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
      "Hello,ell",
      "This is good, is",
      "CodeEval,C*Eval",
      "CodeEval,C\\*Eval",
      "Old,Young",
    };
    std::vector<std::string> v_expect {
      "true",
      "true",
      "true",
      "false",
      "false",
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

  size_t lookup_impl(const std::string& where, const size_t from, const std::string &s) {
    // brute force approach first
    if(from + s.length() > where.length()) {
      return std::string::npos;
    }
    for(size_t i = from, j = where.length() - s.length() + 1; i < j; ++i) {
      bool flag = true;
      for(size_t pos = 0; pos < s.length() && flag; ++pos) {
        flag &= s[pos] == where[i + pos];
      }
      if(flag) {
        return i;
      }
    }
    return std::string::npos;
  }

  bool rlookup(const std::string& where, const std::vector<std::string> &v, const size_t i) {
    if(v.size() <= i) {
      return true;
    }
    const auto &s = v[i];
    for(size_t from = lookup_impl(where, 0, s); std::string::npos != from; from = lookup_impl(where, from + 1, s)) {
      if( from + s.length() > where.length()) {
        break;
      }
      if(rlookup(where.substr(from+s.length()), v, i + 1)) {
        return true;
      }
    }
    return false;
  }

  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    const int comma_pos = line.find(',');
    std::string str = line.substr(0, comma_pos);
    std::string pattern = line.substr(comma_pos + 1);

    std::vector<std::string> parts;
    line = pattern;
    while(!line.empty()) {
      bool flag = false;
      bool escape = false;
      for(size_t i = 0; i != line.length(); ++i) {
        const auto c = line[i];
        if('*' == c && !escape) {
          parts.push_back(line.substr(0,i));
          if(parts.back().empty()) {
            parts.pop_back();
          }
          line.erase(0, i + 1);
          flag = true;
          break;
        }
        escape = '\\' == c;
      }
      if(!flag) {
        parts.emplace_back(std::move(line));
      }
    }

    for(auto & s : parts) {
      for(auto p = lookup_impl(s, 0, "\\*"); std::string::npos != p; p = lookup_impl(s, p, "\\*")) {
        s.erase(p, 1);
      }
    }
    
    std::cout << (rlookup(str, parts, 0) ? "true" : "false")  << "\n";
  }
}
