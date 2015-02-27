#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#define TEST

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
      "|deEva|lan t|to ha|evil |ankin|il-ev|o hac| to h|vil p|an to|The e|CodeE| evil|plan |hack |Eval |ack C|l ran|king.|l-evi|evil-|-evil|l pla|il pl| hack|al ra|vil-e|odeEv|he ev|n to |ck Co|eEval|nking| rank| Code|e evi|ranki|k Cod| plan|val r|"
    };
    std::vector<std::string> v_expect {
      "The evil-evil plan to hack CodeEval ranking."
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

  template<typename _k, typename _v> using map_t = std::unordered_map<_k,_v>;
  using string_t = std::string;
  template<typename _v> using vector_t = std::vector<_v>;

  void process(std::string line) {
    std::istringstream ss { line };
    size_t n = 0;
    int i = 1;
    vector_t<string_t> v;
    for(std::string t; std::getline(ss, t, '|');) {
      if(!t.empty()) {
        if(!n) {
          n = t.length();
        } else if(t.length() != n) {
          std::cout << "bad length: '" << t << "'\n";
          throw n;
        }
        v.push_back(t);
      }
    }
    while(v.size() > 1) {
      vector_t<int> cv;
      int i;
      int j;
      for(i = 0; i < v.size(); ++i) {
        cv.clear();
        for(j = 0; j < v.size(); ++j) {
          if(j == i) {
            continue;
          }
          const auto& vi = v[i];
          const auto& vj = v[j];
          const auto ir = vi.substr(vi.length() - n + 1);
          const auto jl = vj.substr(0, n - 1);
          if(ir == jl) {
            cv.push_back(j);
          }
        }
        if(1 == cv.size()) {
          break;
        }
      }
      j = cv.front();
      v[i] += v[j].substr(n - 1);
      v.erase(v.begin() + j);
    }
    std::cout << v.front() << "\n";
  }
}
