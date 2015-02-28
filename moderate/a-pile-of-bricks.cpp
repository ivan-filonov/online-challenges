#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace {
  void test();
  void process_file(char*);
}

int main(int argc, char ** argv) {
    if( 0 ) {
        test();
    } else {
        process_file(argv[1]);
    }
	return 0;
}

namespace {
  void process(std::string s);

  void test() {
    const std::vector<std::string> v_test { 
      "[4,3] [3,-3]|(1 [10,9,4] [9,4,2])",
      "[-1,-5] [5,-2]|(1 [4,7,8] [2,9,0]);(2 [0,7,1] [5,9,8])",
      "[-4,-5] [-5,-3]|(1 [4,8,6] [0,9,2]);(2 [8,-1,3] [0,5,4])",
    };
    const std::vector<std::string> v_expect { 
      "1",
      "1,2",
      "-",
    };
    for(auto t = v_test.begin(), e = v_expect.begin(); v_test.end() != t; ++t, ++e) {
      process(*t);
      std::cout << *e << "\n";
    }
  }

  void process_file(char* path) {
    std::ifstream stream(path);
    for(std::string line; std::getline(stream, line); ) {
      process(line);
    }
  }

  void process(std::string line) {
    std::istringstream ss { line };

    int hx1, hx2, hy1, hy2;
    ss.ignore(255, '[');
    ss >> hx1;
    ss.ignore(255, ',');
    ss >> hy1;
    ss.ignore(255, ']');
    
    ss.ignore(255, '[');
    ss >> hx2;
    ss.ignore(255, ',');
    ss >> hy2;
    ss.ignore(255, ']');

    int hd1 = std::abs(hx1 - hx2);
    int hd2 = std::abs(hy1 - hy2);
    if(hd1 > hd2) {
      std::swap(hd1, hd2);
    }
  
    char semicolon;
    std::vector<int> r;
    do {
      int id;
      int v1[3] {};
      int v2[3] {};
     
      ss.ignore(25, '(');
      ss >> id;
      ss.ignore(25, '[');
      ss >> v1[0];
      ss.ignore(25, ',');
      ss >> v1[1];
      ss.ignore(25, ',');
      ss >> v1[2];
      ss.ignore(25, ']');
     
      ss.ignore(25, '[');
      ss >> v2[0];
      ss.ignore(25, ',');
      ss >> v2[1];
      ss.ignore(25, ',');
      ss >> v2[2];
      ss.ignore(25, ']');
      ss.ignore(25, ')');

      for(int i = 0; i < 3; ++i) {
        v1[i] = std::abs(v1[i] - v2[i]);
      }
      std::sort(std::begin(v1), std::end(v1));
      if(hd1 >= v1[0] && hd2 >= v1[1]) {
        r.push_back(id);
      }
    } while(ss >> semicolon && ';' == semicolon);
    if(!r.empty()) {
      std::sort(r.begin(), r.end());
      bool mid = false;
      for(auto i = r.begin(); r.end() != i; ++i) {
        if(mid) {
          std::cout << ',';
        } else {
          mid = true;
        }
        std::cout << *i;
      }
    } else {
      std::cout << '-';
    }
    std::cout << "\n";
  }
}
