#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>
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
      "O draconia;conian devil! Oh la;h lame sa;saint!",
      "m quaerat voluptatem.;pora incidunt ut labore et d;, consectetur, adipisci velit;olore magnam aliqua;idunt ut labore et dolore magn;uptatem.;i dolorem ipsum qu;iquam quaerat vol;psum quia dolor sit amet, consectetur, a;ia dolor sit amet, conse;squam est, qui do;Neque porro quisquam est, qu;aerat voluptatem.;m eius modi tem;Neque porro qui;, sed quia non numquam ei;lorem ipsum quia dolor sit amet;ctetur, adipisci velit, sed quia non numq;unt ut labore et dolore magnam aliquam qu;dipisci velit, sed quia non numqua;us modi tempora incid;Neque porro quisquam est, qui dolorem i;uam eius modi tem;pora inc;am al",
    };
    std::vector<std::string> v_expect {
      "O draconian devil! Oh lame saint!",
      "Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem.",
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

  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    std::vector<std::string> parts;
    
    int ms = 0;
    std::istringstream ss { line };
    for(std::string t; std::getline(ss, t, ';');) {
      ms = std::max(ms, (int)t.length());
      parts.emplace_back(std::move(t));
    }

    --ms;
    while(ms && parts.size() > 1){
      std::unordered_map<std::string,int> right;
      for(int i = 0; i < parts.size(); ++i) {
        auto &s = parts[i];
        if(s.length() <= ms) {
          continue;
        }
        right[s.substr(s.length() - ms)] = i;
      }
      if(right.size() < 2) {
        --ms;
        continue;
      }
      int vl = -1, vr = -1;
      for(int i = 0; i < parts.size(); ++i) {
        auto &s = parts[i];
        if(s.length() <= ms) {
          continue;
        }
        auto it = right.find(s.substr(0, ms));
        if(right.end() == it) {
          continue;
        }
        if(i == it->second) {
          continue;
        }
        vr = i;
        vl = it->second;
        break;
      }
      if(vl < 0) {
        --ms;
        continue;
      }
      parts.push_back(parts[vl] + parts[vr].substr(ms));
      parts.erase(parts.begin() + std::max(vl, vr));
      parts.erase(parts.begin() + std::min(vl, vr));
    }
    std::sort(parts.begin(), parts.end(), [](const std::string &s1, const std::string &s2){ return s2.length() < s1.length(); });
    std::cout << parts.front() << "\n";
  }
}
