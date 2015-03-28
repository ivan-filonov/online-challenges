#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <tuple>
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

  std::string learn_from { R"raw(Mary had a little lamb its fleece was white as snow;)raw"
    R"raw(And everywhere that Mary went, the lamb was sure to go.)raw"
    R"raw(It followed her to school one day, which was against the rule;)raw"
    R"raw(It made the children laugh and play, to see a lamb at school.)raw"
    R"raw(And so the teacher turned it out, but still it lingered near,)raw"
    R"raw(And waited patiently about till Mary did appear.)raw"
    R"raw("Why does the lamb love Mary so?" the eager children cry; "Why, Mary loves the lamb, you know" the teacher did reply.")raw"
  };
  std::vector<std::string> words;

  void prepare() {
    std::string w;
    for(auto c : learn_from) {
      if(std::isalpha(c)) {
        w.append(1, c);
      } else {
        if(!w.empty()) {
          words.push_back(w);
          w.clear();
        }
      }
    }
  }

#ifdef TEST
  void test() {
    prepare();
    std::vector<std::string> v_test {
"2,the"
    };
    std::vector<std::string> v_expect {
"lamb,0.375;teacher,0.250;children,0.125;eager,0.125;rule,0.125"
    };
    for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
      process(v_test[i]);
      std::cout << v_expect[i] << "\n";
    }
  }
#endif //#ifdef TEST

  void process_file(char* path) {
    prepare();
    std::ifstream stream(path);
    for(std::string line; std::getline(stream, line); ) {
      process(line);
    }
  }

  void process(std::string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    size_t idx = 0;
    auto ng_size = std::stoi(line, &idx);
    auto text = line.substr(idx + 1);

    std::map<std::string, int> dict; 
    for(size_t i = 0, imax = words.size() - ng_size + 1; i != imax; ++i) {
      std::string ng;
      for(int j = 0; j < ng_size; ++j) {
        if(j) {
          ng += ' ';
        }
        ng += words[i+j];
      }
      ++dict[ng];
    }

    std::vector<std::tuple<std::string,double>> v;
    double total = 0;
    for(auto& p : dict) {
      if(p.first.compare(0, text.size(), text) || ' ' != p.first[text.size()]) {
        continue;
      }
      total += p.second;
      v.push_back(std::make_tuple(p.first, (double)p.second));
    }

    std::cout.precision(3);
    std::cout.setf(std::cout.fixed);

    using std::get;
    std::sort(v.begin(), v.end(), [](const std::tuple<std::string,double> &v1, const std::tuple<std::string,double> &v2) { 
      if(get<1>(v1) == get<1>(v2)) { 
        return get<0>(v1) < get<0>(v2);
      } 
      return get<1>(v1) > get<1>(v2); 
    });

    bool mid = false;
    for(auto &t : v) {
      if(mid) {
        std::cout << ";";
      } else {
        mid = true;
      }
      std::cout << get<0>(t).substr(text.size() + 1) << "," << (get<1>(t)/total);
    }
    std::cout << "\n";
  }
}
