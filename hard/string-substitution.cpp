#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>

//#define TEST

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
"10011011001;0110,1001,1001,0,10,11"
    };
    std::vector<std::string> v_expect {
"11100110"
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

  struct state_t {
    std::list<std::string> s;
    std::list<bool> a;
   
    std::vector<std::string> F;
    std::vector<std::string> R;

    state_t (const std::string& line) : s(1), a(1,true) {
      std::istringstream ss { line };

      std::getline(ss, s.front(), ';');
      for(std::string t1, t2; std::getline(ss, t1, ',') && std::getline(ss, t2, ',');) {
        F.push_back(t1);
        R.push_back(t2);
      }
    }

    void run() {
      for(size_t i = 0, j = F.size(); i < j; ++i) {
        while(step(i));
      }
      for(auto & v : s) {
        std::cout << v;
      }
      std::cout << "\n";
    }

    bool step(size_t i) {
      const auto &f = F[i];
      const auto &r = R[i];

      auto sit = s.begin();
      auto ait = a.begin();

      for(; s.end() != sit; ++ait, ++sit) {
        if(!*ait) {
          continue;
        }
        auto found = sit->find(f);
        if(std::string::npos == found) {
          continue;
        }

        std::string left, right;
        if(found) {
          left = sit->substr(0, found);
        }
        if(found + f.length() < sit->length()) {
          right = sit->substr(found + f.length());
        }

        if(!left.empty()) {
          s.insert(sit, left);
          a.insert(ait, true);
        }

        s.insert(sit, r);
        a.insert(ait, false);

        if(!right.empty()) {
          s.insert(sit, right);
          a.insert(ait, true);
        }
        
        s.erase(sit);
        a.erase(ait);

        return true;
      }

      //nothing found
      return false;
    }
  };
  
  void process(std::string line) {
    state_t st { line };
    st.run();
  }
}
