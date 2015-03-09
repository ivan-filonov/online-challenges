#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
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
  void process(std::string s);

  struct worker {
    std::string initial;
    std::vector<std::string> parts;

    worker(const std::string &s);
    std::string run();
  };

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

  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    std::cout << worker { line }.run() << "\n";
  }

  worker::worker(const std::string &s) : initial(s) {
    std::istringstream ss { s };
    for(std::string t; std::getline(ss, t, '|');) {
      if(!t.empty()) {
        parts.push_back(t);
      }
    }
  }

  std::string worker::run() {
    return parts.front();
  }
}
