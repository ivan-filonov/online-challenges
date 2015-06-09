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

  using std::string;
  using std::vector;
  template<typename vt> using set = std::unordered_map<vt, bool>;
  template<typename kt, typename vt> using map = std::unordered_map<kt, vt>;

  struct worker;

  struct state {
    const vector<char> &word_first;
    const vector<char> &word_last;
    const map<char,vector<int>> &index_first;
    const map<char,vector<int>> &index_last;
    const int start;
   
    set<int> used;

    state(worker *w, int start);
    int run();
    int num_choices(char c) const;
    int probe(int curr);
  };

  int state::num_choices(char c) const {
    auto it = index_first.find(c);
    if(index_first.end() == it) {
      return 0;
    } else {
      return it->second.size();
    }
  }

  struct scope {
    set<int> & _set;
    const int _val;
    const bool scoped;

    scope(set<int> &_s, int _v) : _set(_s), _val(_v), scoped(_s.count(_v) == 0) {
      if(scoped) {
        _set.insert({ _val, true });
      }
    }

    ~scope() {
      if(scoped) {
        _set.erase(_val);
      }
    }

    operator bool () const {
      return scoped;
    }
  };

  int state::probe(int curr) {
    scope scope_lock { used, curr };
    if(! scope_lock ) {
      return 0;
    }
    int r = 0;
    auto c = word_last[curr];
    for(int i = 0, j = num_choices(c); i < j; ++i) {
      r = std::max(r, probe(index_first.find(c)->second[i]));
    }
    return r + 1;
  }

  struct worker {
    const string initial;
    vector<char> word_first;
    vector<char> word_last;
    map<char,vector<int>> index_first;
    map<char,vector<int>> index_last;

    worker(string &&s);
    void prepare();
    void run();
  };

  state::state(worker *w, int _start) : start(_start),
    word_first(w->word_first), word_last(w->word_last), 
    index_first(w->index_first), index_last(w->index_last) {
  }

  int state::run() {
    return probe(start);
  }

  worker::worker(string &&s) : initial(std::move(s)) {
  }

  void worker::prepare() {
    std::istringstream ss { initial };
    for(string t; std::getline(ss, t, ',');) {
      int idx = word_first.size();
      word_first.push_back(t.front());
      index_first[t.front()].push_back(idx);
      word_last.push_back(t.back());
      index_last[t.back()].push_back(idx);
    }
  }

  void worker::run() {
    prepare();
    int res = 1;
    for(int i = 0, j = word_first.size(); i < j; ++i) {
      res = std::max(res, state { this, i }.run());
    }
    if(res > 1) {
      std::cout << res << "\n";
    } else {
      std::cout << "None\n";
    }
  }

  void process(string line) {
#ifdef TEST
    std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
    worker w { std::move(line) };
    w.run();
  }
}
