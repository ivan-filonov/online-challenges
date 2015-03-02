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

  struct rec_t {
    string_t value;
    string_t left;
    string_t right;
    static size_t n;

    rec_t() {}

    rec_t(const string_t& s) : value { s } {
      left = s.substr(0, n - 1);
      right = s.substr(s.length() - (n - 1));
    }

    rec_t& operator = (rec_t && other) {
      value = std::move(other.value);
      left = std::move(other.left);
      right = std::move(other.right);
      return *this;
    }

    rec_t(rec_t&& other) : value { std::move(other.value) }, left { std::move(other.left) }, right { std::move(other.right) } {}

    rec_t operator + (const rec_t& other) {
      rec_t res;
      res.left = left;
      res.right = other.right;
      res.value = value + other.value.substr(n - 1);
      return res;
    }

    bool can_glue(const rec_t& other) const {
      return right == other.left;
    }
  };

  size_t rec_t::n = 0;
  
  size_t split_input_string(const string_t & line, vector_t<rec_t> & v) {
    size_t & n = rec_t::n;
    std::istringstream ss { line };
    for(std::string t; std::getline(ss, t, '|');) {
      if(!t.empty()) {
        if(!n) {
          n = t.length();
        } else if(t.length() != n) {
          std::cout << "bad length: '" << t << "'\n";
          throw n;
        }
        v.emplace_back(t);
      }
    }
    return n;
  }

  void process(std::string line) {
    vector_t<rec_t> v;
    split_input_string(line, v);
    auto & n = rec_t::n;

    while(v.size() > 1) {
      size_t i,j;
      vector_t<size_t> cv;
      for(i = 0; i < v.size(); ++i) {
        auto & vi = v[i];
        cv.clear();
        for(j = 0; j < v.size(); ++j) {
          if(i != j) {
            auto & vj = v[j];
            if(vi.can_glue(vj)) {
              cv.push_back(j);
            }
          }
        }
        if(cv.size() == 1) {
          break;
        }
      }
      if(cv.size() == 1) {
        j = cv.front();
        v[i] = v[i] + v[j];
        v.erase(v.begin() + j);
      }
    }
    std::cout << v.front().value << "\n";
  }
}
