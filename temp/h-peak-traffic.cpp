#include <fstream>
#include <iostream>
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
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

namespace {
  struct state {
    using string = std::string;
    template<typename K, typename V> using map = std::unordered_map<K,V>;
    template<typename V> using vector = std::vector<V>;

    void add(const string &line) {
#ifdef TEST
      std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  //    std::istringstream ss { line };
      const auto tab1 = line.find('\t');
      const auto tab2 = line.find('\t', tab1 + 1);
      auto u1 = line.substr(tab1 + 1, tab2 - tab1 - 1);
      auto u2 = line.substr(tab2 + 1);
      add(std::move(u1), std::move(u2));
    }

    map<string,map<string,int>> rels;
    void add(string &&u1, string &&u2) {
      rels[u1][u2] = 1;
    }
    
    void run() {
    }
  };

#ifdef TEST
  void test() {
    std::vector<std::string> v_test {
      "Thu Dec 11 17:53:01 PST 2008	a@facebook.com	b@facebook.com",
      "Thu Dec 11 17:53:02 PST 2008	b@facebook.com	a@facebook.com",
      "Thu Dec 11 17:53:03 PST 2008	a@facebook.com	c@facebook.com",
      "Thu Dec 11 17:53:04 PST 2008	c@facebook.com	a@facebook.com",
      "Thu Dec 11 17:53:05 PST 2008	b@facebook.com	c@facebook.com",
      "Thu Dec 11 17:53:06 PST 2008	c@facebook.com	b@facebook.com",
      "Thu Dec 11 17:53:07 PST 2008	d@facebook.com	e@facebook.com",
      "Thu Dec 11 17:53:08 PST 2008	e@facebook.com	d@facebook.com",
      "Thu Dec 11 17:53:09 PST 2008	d@facebook.com	f@facebook.com",
      "Thu Dec 11 17:53:10 PST 2008	f@facebook.com	d@facebook.com",
      "Thu Dec 11 17:53:11 PST 2008	e@facebook.com	f@facebook.com",
      "Thu Dec 11 17:53:12 PST 2008	f@facebook.com	e@facebook.com",
    };
    std::vector<std::string> v_expect {
      "a@facebook.com, b@facebook.com, c@facebook.com",
      "d@facebook.com, e@facebook.com, f@facebook.com",
    };

    state st;
    for(auto &s : v_test) {
      st.add(s);
    }

    st.run();

    for(auto &s : v_expect) {
      std::cout << s << "\n";
    }
  }
#endif //#ifdef TEST

  void process_file(char* path) {
    std::ifstream stream(path);
    state st;
    for(std::string line; std::getline(stream, line); ) {
      st.add(line);
    }
    st.run();
  }
}
