#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

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

  void print() {
    std::cout << "('" << tc << "'";
    bool mid = false;
    for(auto & t : tbuf) {
      std::cout << (mid ? ", " : "");
      mid = true;
      t.print();
    }
    std::cout << ")";
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
      std::cout << "@" << __LINE__ << " matched '" << (w) << "'\n";
      return true;
    }
    // 2. handle differences
    if(nullptr == t && w[0] && !w[1]) {
      // w should point to last character of word
      std::cout << "@" << __LINE__ << " matched '" << (w) << "'\n";
      return true;
    }
    if(!*w) {
      // any leaf of t must be a word
      for(auto & tt : t->tbuf) {
        if(tt.is_word) {
          std::cout << "@" << __LINE__ << " matched '" << (w) << "'\n";
          return true;
        }
      }
      return false;
    }

    // 2.1 test if we can skip *w:
    if(pt->exact(w + 1)) {
      std::cout << "@" << __LINE__ << " matched '" << (w+1) << "'\n";
      return true;
    }

    // 2.2 test if we have one typo:
    // 2.3 test if any leaf of pt can match w:
    for(auto & tt : pt->tbuf) {
      if(tt.exact(w+1)) {
        std::cout << "@" << __LINE__ << " matched '" << (w+1) << "'\n";
        return true;
      }
      if(tt.exact(w)) {
        std::cout << "@" << __LINE__ << " matched '" << (w) << "'\n";
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

trie_t T;

void t(trie_t * tr, const char * w, bool e) {
  bool r = tr->lev1(w);
  std::cout << "lev1('" << w << "') -> " << (r?"true":"false") << ", expected " << (e?"true":"false") << ((e==r) ? "" : " FAIL") << "\n";
}

int main() {
  T.put("hello");
  T.put("freedom");
  T.put("feed");
  T.put("by");
  T.put("blood");
  T.put("of");
  T.put("patriots");

  t(&T, "", false);
  t(&T, "hello", true);
  t(&T, "helzo", true);
  t(&T, "heizo", false);
  t(&T, "shello", true);
  t(&T, "heello", true);
  t(&T, "hallo", true);
  t(&T, "hllo", true);
  t(&T, "helloz", true);
  t(&T, "hell", true);
  t(&T, "hel", false);

  vector<string> wl;
  T.words(wl);
  bool mid = false;
  for(auto & s : wl) {
    std::cout << (mid ? ", " : "words: ") << s;
    mid= true;
  }
  if(mid) {
    std::cout << "\n";
  }
  return 0;
}
