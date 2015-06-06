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

struct trie_t :
  public std::enable_shared_from_this<trie_t> {
  using ptr = shared_ptr<trie_t>;
  
  weak_ptr<trie_t> parent;
  char c;
  int is_word;
  vector<ptr> leafs;
  
  trie_t() : c(0), is_word(0) {}

  void _print2_r(bool * mid) {
    if(is_word) {
      vector<ptr> v;
      auto t = shared_from_this();
      while(t) {
        v.push_back(t);
        t = t->parent.lock();
      }
      std::cout << (*mid ? "', '" : "{ '");
      for(size_t i = v.size() - 1; i; --i) {
        std::cout << v[i]->c;
      }
      std::cout << c;
      *mid = true;
    }
    for(auto l : leafs) {
      l->_print2_r(mid);
    }
  }

  void print2() {
    bool mid = false;
    _print2_r(&mid);
    if(mid) {
      std::cout << "' }\n";
    }
  }

  void put(const char * w) {
    auto t = shared_from_this();
    while(*w) {
      trie_t::ptr nt;
      for(auto it : t->leafs) {
        if(*w == it->c) {
          nt = it;
          break;
        }
      }
      if(!nt) {
        nt = make_shared<trie_t>();
        nt->parent = t;
        nt->c = *w;
        t->leafs.push_back(nt);
      }
      t = nt;
      ++w;
    }
    t->is_word = 1;
  }

  void merge(ptr other) {
    if(other->c != c) {
      throw other;
    }
    if(other->is_word) {
      is_word = 1;
    }
    for(auto ol : other->leafs) {
      ptr l = _find_c(ol->c);
      if(!l) {
        ol->parent = shared_from_this();
        leafs.push_back(ol);
      } else {
        l->merge(ol);
      }
    }
  }

  ptr _find_c (char fc) {
    for(auto l : leafs) {
      if(l->c == fc) {
        return l;
      }
    }
    return ptr();
  }

  bool exact(const char * w) {
    ptr t = shared_from_this();
    while(*w && t) {
      t = t->_find_c(*w);
      ++w;
    }
    return t && t->is_word;
  }

  // check if trie contains words that have levenstein distance 1 to w;
  bool lev1(const char * w) {
    ptr t = shared_from_this(), pt;
    while(*w && t) {
      pt = t;
      t = t->_find_c(*w);
      if(t) {
        ++w;
      }
    }
    if(!*w) {
      if(t && t->is_word) {
//        std::cout << "@" << __LINE__ << " exact match!\n";
        return true;
      }
      for(auto l : t->leafs) {
        if(l->is_word) {
//          std::cout << "@" << __LINE__ << " l1 match by adding '" << l->c << "' in the end\n";
          return true;
        }
      }
//      std::cout << "@" << __LINE__ << " no matches\n";
      return false;
    }
    // 1. check if we have a character inserted:
    if(pt->exact(w+1)) {
//      std::cout << "@" << __LINE__ << " l1 match, excess character: '" << *w << "'\n";
      return true;
    }
    for(auto l : pt->leafs) {
      // 2. check if we have a typo:
      if(l->exact(w+1)) {
//        std::cout << "@" << __LINE__ << " l1 typo: '" << *w << "'\n";
        return true;
      }
      // 3. check if we have a missing character:
      if(l->exact(w)) {
//        std::cout << "@" << __LINE__ << " l1 missing char\n";
        return true;
      }
    }
    return false;
  }

  int size() {
    int s = is_word;
    for(auto l : leafs) {
      s += l->size();
    }
    return s;
  }
};

vector<string> seed_words;
list<trie_t::ptr> clusters;

void add_line(string line) {
  if(reading_seeds) {
    if(SEED_END != line) {
      seed_words.emplace_back(move(line));
    } else {
      reading_seeds = false;
    }
  } else {
    vector<trie_t::ptr> cv;
    for(auto c : clusters) {
      if(c->lev1(line.c_str())) {
        cv.push_back(c);
      }
    }
    if(cv.empty()) {
      auto t = make_shared<trie_t>();
      t->put(line.c_str());
      clusters.push_back(t);
    } else {
      cv.front()->put(line.c_str());
      for(size_t i = 1; i != cv.size(); ++i) {
        cv.front()->merge(cv[i]);
        clusters.remove(cv[i]);
      }
    }
  }
}

void run() {
  for(auto & sw : seed_words) {
    int s = 0;
    for(auto c : clusters) {
      if(c->lev1(sw.c_str())) {
        s = c->size();
        break;
      }
    }
    std::cout << s << "\n";
  }
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
