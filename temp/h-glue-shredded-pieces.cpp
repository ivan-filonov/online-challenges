//#include <exception>
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

  size_t split_input_string(const string_t & line, vector_t<string_t> & v) {
    size_t n = 0;
    std::istringstream ss { line };
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
    return n;
  }

  bool can_glue(const string_t& s1, const string_t& s2, const size_t n) {
    bool equal = true;
    for(auto it1 = s2.begin(), it2 = (s2.begin() + (n-1)), it3 = s1.begin() + (s1.length() - (n-1)); it1 != it2 && equal; ++it1, ++it3) {
      equal &= (*it1 == *it3);
    }
    return equal;
  }

  void glue_step(vector_t<string_t> & v, const size_t n) {
    size_t i, j;
    vector_t<int> cv;
    
    for(i = 0; i < v.size(); ++i) {
      cv.clear();
      auto & vi = v[i];
      if(vi.empty()) {
        continue;
      }
      for(j = 0; j < v.size(); ++j) {
        if(i != j) {
          // compare last n-1 chars of v[i] with the beginning of v[j]
          auto & vj = v[j];
          if(vj.empty()) {
            continue;
          }
          if(can_glue(vi, vj, n)) {
            cv.push_back(j);
          }
        } else {
          continue;
        }
      }
      if(1 == cv.size()) {
        break;
      }
    }
    if(1 == cv.size()) {
      j = cv.front();
      auto ns = v[i] + v[j].substr(n - 1);

      v[i] = ns;
      v.erase(v.begin() + j);
    }
  }

  void process(std::string line) {
    vector_t<string_t> v;
    auto n = split_input_string(line, v);
    while(v.size() > 1) {
      glue_step(v, n);
    }
    for(auto & s : v) {
      std::cout << s;
    }
    std::cout << "\n";
  }
}
