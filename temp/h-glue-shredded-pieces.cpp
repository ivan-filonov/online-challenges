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
  template<typename K,typename V> using multimap = std::unordered_multimap<K,V>;

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
    const string s;
    const int left;
    const int right;
    item_t(string &&_s, int lm, int rm) : s(std::move(_s)), left(lm), right(rm) {}
  };

  struct worker {
    int n;
    const string initial;

    worker(const string& s) : initial(s) {}
  
    void run();
    
    void prepare();
    bool step();
    string result() const;
  };

  int map_marker(map<string,int> &m, string &&s) {
    auto it = m.find(s);
    if(m.end() != it) {
      return it->second;
    }
    int res = m.size();
    m.insert({ std::move(s), res });
    return res;
  }

  void worker::prepare() {
    string t;
    std::istringstream ss { initial };
    map<string,int> markers;
    while(std::getline(ss, t, '|')) {
      if(t.empty()) {
        continue;
      }
      auto tr = t.substr(1);
      auto tl = t.substr(0, n = t.length() - 1);
      auto mr = map_marker(markers, std::move(tr));
      auto ml = map_marker(markers, std::move(tl));
      auto item = std::make_shared<item_t>(std::move(t), ml, mr);
    }
  }

  bool worker::step() {
    return false;
  }

  string worker::result() const {
    return string(initial.length(), '?');
  }

  void worker::run() {
    prepare();
    while(step());
    std::cout << result() << "\n";
  }
  
  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    worker w { line };
    w.run();
  }
}
