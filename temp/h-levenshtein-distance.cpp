#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include <string.h>

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
template<typename V> using set = std::set<V>;

using std::move;
using std::swap;

struct trie_t {
  int is_word : 1;
  std::string W;
  char tc;
  vector<trie_t> tbuf;

  trie_t() : is_word(0) {
  }

  trie_t * _find_c(char c) {
//    std::cout << __FUNCTION__ << " " << this << "\n";
    for(auto & t : tbuf) {
      if(t.tc == c) {
        return &t;
      }
    }
    return nullptr;
  }

  void put(const char * word) {
    auto pword = word;
    trie_t * t = this;
    while(*word) {
      trie_t * n = t->_find_c(*word);
      if(nullptr == n) {
        t->tbuf.emplace_back();
        t->tbuf.back().tc = *word;
        n = &t->tbuf.back();
      }
      t = n;
      ++word;
    }
    t->is_word = 1;
    t->W = pword;
  }

  bool exact(const char * w) {
    trie_t * t = this;
    for(; *w && t; ++w) {
      t = t->_find_c(*w);
    }
    return t ? t->is_word : false;
  }

  trie_t * exact_ptr(const char * w) {
    trie_t * t = this;
    for(; *w && t; ++w) {
      t = t->_find_c(*w);
    }
    return (t && t->is_word) ? t : nullptr;
  }

  bool lev1(const char * w) {
    trie_t * t = this, * pt = nullptr;
    // 1. skip all known characters of word
    for(; *w; ++w) {
      pt = t;
      t = t->_find_c(*w);
      if(!t) {
        break;
      }
    }
    if(t && t->is_word) {
      // exact match
      return true;
    }
    // 2. handle differences
    if(nullptr == t && w[0] && !w[1]) {
      // w should point to last character of word
      return true;
    }
    if(!*w) {
      // any leaf of t must be a word
      for(auto & tt : t->tbuf) {
        if(tt.is_word) {
          return true;
        }
      }
      return false;
    }

    // 2.1 test if we can skip *w:
    if(pt->exact(w + 1)) {
      return true;
    }

    // 2.2 test if we have one typo:
    // 2.3 test if any leaf of pt can match w:
    for(auto & tt : pt->tbuf) {
      if(tt.exact(w+1)) {
        return true;
      }
      if(tt.exact(w)) {
        return true;
      }
    }

    return false;
  }

  void words(vector<string> & dst, string cur = "") {
    if(is_word) {
      dst.emplace_back(cur);
    }
    for(auto & t : tbuf) {
      t.words(dst, cur + t.tc);
    }
  }
};

const string SEED_END { "END OF INPUT" };
bool reading_seeds = true;

vector<string> seed_words;
vector<char> wbuf;
vector<size_t> wofs;

void add_line(string line) {
  if(reading_seeds) {
    if(SEED_END != line) {
      seed_words.emplace_back(move(line));
    } else {
      reading_seeds = false;
    }
  } else {
    const auto npos = wbuf.size();
    wbuf.resize(npos + line.length() + 1);
    strcpy(wbuf.data() + npos, line.c_str());
    const auto widx = wofs.size();
    wofs.push_back(npos);
  }
}

void run() {
  vector<trie_t*> clusters;
  for(auto ofs : wofs) {
    char * word = wbuf.data() + ofs;
    vector<trie_t*> nc;
    for(auto pc : clusters) {
      if(pc->lev1(word)) {
        nc.push_back(pc);
      }
    }
    if(nc.empty()) {
      trie_t * nt = new trie_t;
      nt->put(word);
      clusters.push_back(nt);
    } else {
      auto dst = nc[0];
      dst->put(word);
      for(size_t i = nc.size() - 1; i; --i) {
        auto src = nc[i];
        for(size_t j = 0; j != clusters.size(); ++j) {
          if(clusters[j] == src) {
            clusters.erase(clusters.begin() + j);
            break;
          }
        }
        vector<string> temp;
        src->words(temp);
        delete src;
        for(auto & tw : temp) {
          dst->put(tw.c_str());
        }
      }
    }
  }
  for(auto & sw : seed_words) {
    vector<string> t;
    trie_t * c = nullptr;
    for(auto cc : clusters) {
      if(cc->lev1(sw.c_str())) {
        c = cc;
        break;
      }
    }
    if(nullptr != c) {
      c->words(t);
    }
    std::cout << (t.size() + 0) << "\n";
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
