#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <list>
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

// stl types
using std::string;
template<typename V> using shared_ptr = std::shared_ptr<V>;
template<typename V> using weak_ptr = std::weak_ptr<V>;
template<typename V> using set = std::set<V>;
template<typename V> using vector = std::vector<V>;
template<typename V> using list = std::list<V>;

// stl functions
using std::make_shared;
using std::move;
using std::swap;

const string SEED_END { "END OF INPUT" };
bool reading_seeds = true;

// lowercase-english-letters-only trie:
struct trie_t {
  struct stat_s {
    char  word : 1;

    stat_s() : word(0) {}
  };
  // [base * 26 + char]
  vector<stat_s> stat;
  vector<int>  tail;
  int capacity;
  int next_base;

  static const int INITIAL_CAPACITY = 10;

  trie_t() : stat(26 * INITIAL_CAPACITY), tail(26 * INITIAL_CAPACITY), capacity(INITIAL_CAPACITY), next_base(1) { }

  void grow (int new_cap) {
    stat.resize(new_cap * 26);
    tail.resize(new_cap * 26, 0);
    capacity = new_cap;
  }

  void put(const char * w) {
    int base = 0, ofs;
    while(*w) {
      ofs = base * 26 + (*w - 'a');
      if(!tail[ofs]) {
        if(next_base == capacity) {
          grow(capacity * 2);
        }
        tail[ofs] = next_base;
        ++next_base;
      }
      base = tail[ofs];
      ++w;
    }
    stat[ofs].word = 1;
  }

  bool _exact(const char * w, int base) {
    int ofs;
    while(*w) {
      ofs = base * 26 + (*w - 'a');
      base = tail[ofs];
      if(!base) {
        break;
      }
      ++w;
    }
    return base && stat[ofs].word;
  }

  bool exact(const char * w) {
    return _exact(w, 0);
  }

  void print_from(int base, vector<char> & buf) {
    int ofs = base * 26;
    for(int i = 0; i != 26; ++i) {
      if(tail[ofs + i]) {
        buf.push_back('a' + i);
        print_from(tail[ofs + i], buf);
        buf.pop_back();
      }
      if(stat[ofs + i].word) {
        for(auto c : buf) std::cout << c;
        std::cout << (char)('a' + i) << "\n";
      }
    }
  }

  void _merge_to(int base, vector<char> & buf, trie_t & dst) {
    int ofs = base * 26;
    for(int i = 0; i != 26; ++i) {
      if(tail[ofs + i]) {
        buf.push_back('a' + i);
        _merge_to(tail[ofs + i], buf, dst);
        buf.pop_back();
      }
      if(stat[ofs + i].word) {
        buf.push_back('a' + i);
        buf.push_back(0);
        dst.put(buf.data());
        buf.pop_back();
        buf.pop_back();
      }
    }
  }

  void merge_to (trie_t & dst) {
    vector<char> buf;
    _merge_to(0, buf, dst);
  }

  void print() {
    vector<char> buf;
    print_from(0, buf);
  }

  bool lev1(const char * w) {
    int base = 0, ofs, prev_base = 0, prev_ofs = 0;
    while(*w){
      prev_ofs = ofs;
      ofs = base * 26 + (*w - 'a');
      if(!tail[ofs]) {
        break;
      }
      prev_base = base;
      base = tail[ofs];
      ++w;
    }
    if(*w) {
      std::cout << "w->'" << w << "', base = " << base << ", prev_base = " << prev_base << "\n";
      if(prev_ofs && !w[1] && stat[prev_ofs].word) {
        return true;
      }
      if(base && _exact(w, prev_base)) {
        return true;
      } else {
        for(int i = 0, j = prev_base * 26; i != 26; ++i, ++j) {
          if(!tail[j]) {
            continue;
          }
          if(_exact(w, tail[j])) {
            return true;
          }
          if(_exact(w + 1, tail[j])) {
            return true;
          }
        }
      }
    } else {
      if(stat[ofs].word) {
        return true;
      } else {
        for(int i = 0, j = base * 26; i != 26; ++i, ++j) {
          if(stat[j].word) {
            return true;
          }
        }
      }
    }
    return false;
  }
};

void add_line(string line) {
  if(reading_seeds) {
    if(SEED_END != line) {
    } else {
      reading_seeds = false;
    }
  } else {
  }
}

void run() {
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "recursiveness",    "elastic",
    "macrographies",    
    
    "END OF INPUT",
    
    "aa",    "aah",    "aahed",    "aahing",
    "aahs",    "aal",    "aalii",    "aaliis",
    "aals",    "aardvark",    "aardvarks",    "aardwolf",
    "elaborations",    "elaborative",    "elaborator",    "elaboratories",
    "elaborators",    "elaboratory",    "elaeolite",    "elaeolites",
    "elain",    "elains",    "elan",    "elance",
    "elands",    "elanet",    "elanets",    "elans",
    "elaphine",    "elapid",    "elapids",    "elapine",
    "elapse",    "elapsed",    "elapses",    "elapsing",
    "elasmobranch",    "elasmobranches",    "elasmobranchs",    "elastance",
    "elastances",    "elastase",    "elastases",    "elastic",
    "elastically",    "elasticate",    "elasticated",    "elasticates",
    "elasticating",    "elasticise",    "elasticised",    "elasticises",
    "elasticising",    "elasticities",    "elasticity",    "elasticize",
    "elasticized",    "elasticizes",    "elasticizing",    "elasticness",
    "elasticnesses",    "elastics",    "elastin",    "elastins",
    "elastomer",    "elastomeric",    "elastomers",    "elate",
    "electromyography",    "electron",    "electronegative",    "electronegativities",
    "electronegativity",    "electronic",    "electronically",    "electronics",
    "electrooculography",    "electrooptics",    "electroosmoses",    "electroosmosis",
    "electrophiles",    "electrophilic",    "electrophilicities",    "electrophilicity",
    "electrophoresis",    "electrophoretic",    "electrophoretically",    "electrophoretogram",
    "electrophotographic",    "electrophotographies",    "electrophotography",    "electrophysiologic",
    "electrophysiologists",    "electrophysiology",    "electroplate",    "electroplated",
    "zymosan",    "zymosans",    "zymoses",    "zymosimeter",
    "zymotechnics",    "zymotic",    "zymotically",    "zymotics",
    "zythums",    "zyzzyva",    "zyzzyvas",
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
