#include <fstream>
#include <iostream>
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
#else
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

namespace {
  void process(std::string s);

#ifdef TEST
  void test() {
    std::vector<std::string> v_test {
      "soup,sugar,peas,rice",
      "ljhqi,nrtxgiu,jdtphez,wosqm",
      "cjz,tojiv,sgxf,awonm,fcv",
    };
    std::vector<std::string> v_expect {
      "4",
      "None",
      "2",
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
    std::cout << "s = '" << line << "'\n";
    std::istringstream ss { line };
    std::vector<char> c_l;
    std::vector<char> c_r;
    for(std::string t; std::getline(ss, t, ',');) {
      c_l.push_back(t.front());
      c_r.push_back(t.back());
    }
    
    std::cout << "c_l.size() = " << c_l.size() << ", c_r.size() = " << c_r.size() << "\n";
    std::unordered_map<char,std::vector<int>> m_l;
    std::unordered_map<char,std::vector<int>> m_r;
    for(int i = 0; i < c_l.size(); ++i) {
      m_l[c_l[i]].push_back(i);
      m_r[c_r[i]].push_back(i);
    }
    std::cout << "m_l.size() = " << m_l.size() << ", m_r.size() = " << m_r.size() << "\n";
  }
}
