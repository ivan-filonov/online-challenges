#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#ifdef TEST
void test();
#endif //#ifdef TEST
void process_file(char*);

int main(int argc, char ** argv) {
#ifdef TEST
  test();
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

using std::string;
template<typename V> using vector = std::vector<V>;

using std::move;
using std::swap;

vector<string> seed_words;

struct word_t {
  string s;
  int b;

  word_t(string && _s) : s(_s), b(0) {}
  bool operator < (const word_t & other) const {
    return s.length() < other.s.length();
  }
};

vector<word_t> words;

const string SEED_END { "END OF INPUT" };
bool reading_seeds = true;
void add_line(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  if(reading_seeds) {
    if(SEED_END != line) {
      seed_words.emplace_back(move(line));
    } else {
      reading_seeds = false;
    }
  } else {
    words.emplace_back(move(line));
  }
}

bool friendz(const string & s1, const string & s2) {
  size_t i1 = 0;
  size_t i2 = 0;
  size_t e1 = s1.size();
  size_t e2 = s2.size();
  if(e1 == e2) {
    int c = 0;
    while(i1 != e1) {
      if(s1[i1] != s2[i1]) {
        ++c;
        if(c > 1) {
          break;
        }
      }
    }
    return c < 2;
  }
  if(e1 - e2 != 1 && e2 - e1 != 1) {
    return false;
  }
  while(i1 != e1 && i2 != e2 && s1[i1] == s2[i2]) {
    ++i1;
    ++i2;
  }
  if(e1 < e2) {
    ++i2;
  } else {
    ++i1;
  }
  while(i1 != e1 && i2 != e2) {
    if(s1[i1] != s2[i2]) {
      return false;
    }
  }
  return true;
}

void run() {
  std::sort(words.begin(), words.end());
  int wbit = 1;
  for(auto & sw : seed_words) {
    vector<string> nw { sw };
    while(!nw.empty()) {
      auto cw = move(nw.back());
      nw.pop_back();
      
      auto cl = cw.length();
      size_t pos = 0;
      while(pos != words.size() && words[pos].s.length() < cl - 1) {
        ++pos;
      }
      if(words.size() == pos) {
        continue;
      }
      for(;pos != words.size() && words[pos].s.length() == cl - 1; ++pos) {
        auto & w = words[pos];
        if(w.b & wbit) {
          continue;
        }
        if(friendz(w.s, cw)) {
          w.b |= wbit;
          nw.push_back(w.s);
        }
      }
      for(;pos != words.size() && words[pos].s.length() == cl; ++pos) {
        auto & w = words[pos];
        if(w.b & wbit) {
          continue;
        }
        if(friendz(w.s, cw)) {
          w.b |= wbit;
          nw.push_back(w.s);
        }
      }
      for(;pos != words.size() && words[pos].s.length() == cl + 1; ++pos) {
        auto & w = words[pos];
        if(w.b & wbit) {
          continue;
        }
        if(friendz(w.s, cw)) {
          w.b |= wbit;
          nw.push_back(w.s);
        }
      }
    }
    std::cout << std::count_if(words.begin(), words.end(), [wbit](const word_t & w) { return w.b & wbit; }) << "\n";
    wbit <<= 1;
  }
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "recursiveness",
    "elastic",
    "macrographies",
    "END OF INPUT",
    "aa",
    "elastics",
    "elasticss",
    "aahed",
    "aahs",
    "aalii",
    "zymoses",
    "zymosimeters",
  };
  for(auto & t : v_test) {
    add_line(t);
  }
  run();
  for(int i = 0; i < 80; ++i)std::cout<<'-';std::cout<<"\n";
  vector<string> v_expect {
    "1",
    "3",
    "1",
  };
  for(auto & e : v_expect) {
    std::cout << e << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char* path) {
  std::ifstream stream(path);
  for(string line; std::getline(stream, line); ) {
    add_line(line);
  }
  run();
}
