#include <fstream>
#include <iostream>
#include <memory>
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
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

namespace {
  using std::string;
  using std::vector;
  using std::shared_ptr;
  template<typename K,typename V> using map = std::unordered_map<K,V>;

  void process(string s);

#ifdef TEST
  void test() {
    vector<string> v_test {
"|deEva|lan t|to ha|evil |ankin|il-ev|o hac| to h|vil p|an to|The e|CodeE| evil|plan |hack |Eval |ack C|l ran|king.|l-evi|evil-|-evil|l pla|il pl| hack|al ra|vil-e|odeEv|he ev|n to |ck Co|eEval|nking| rank| Code|e evi|ranki|k Cod| plan|val r|"
    };
    vector<string> v_expect {
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
    for(string line; std::getline(stream, line); ) {
      process(line);
    }
  }

  struct item_t {
    string s;
    int left;
    int right;
    item_t(string &&s, int lm, int rm);
  };

  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    
    vector<shared_ptr<item_t>> items;
  
    {
      std::istringstream ss { line };
      map<string,int> markers;

      for(string t; std::getline(ss, t, '|'); ) {
        if(t.empty()) {
          continue;
        }
        auto tr = t.substr(1);
        auto tl = t.substr(0, t.length() - 1);
//        std::cout << "t = '" << t << "', tl = '" << tl << "', tr = '" << tr << "'\n";

        if(markers.find(tl) == markers.end()) {
          int idx = markers.size();
          markers[tl] = idx;
        }

        if(markers.find(tr) == markers.end()) {
          int idx = markers.size();
          markers[tr] = idx;
        }

        items.push_back(std::make_shared<item_t>(std::move(t), markers[tl], markers[tr]));
      }
    }
    std::cout << string(60,'-') << "\n";
    const int n = items.front()->s.length() - 1;
    for(int z = 0; z < 1; ++z) {
    }
  }

  item_t::item_t(string &&_s, int lm, int rm) : s(std::move(_s)), left(lm), right(rm) {
  }
}
