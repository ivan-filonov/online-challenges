#include <iostream>
#include <memory>
#include <set>
#include <vector>

// stl types
using std::string;
template<typename V> using shared_ptr = std::shared_ptr<V>;
template<typename V> using weak_ptr = std::weak_ptr<V>;
template<typename V> using set = std::set<V>;
template<typename V> using vector = std::vector<V>;

// stl functions
using std::make_shared;
using std::move;
using std::swap;

struct trie_t :
  public std::enable_shared_from_this<trie_t> {
  using ptr = shared_ptr<trie_t>;
  
  weak_ptr<trie_t> parent;
  char c;
  int is_word;
  vector<ptr> leafs;
  
  trie_t() : c(0), is_word(0) {}

  void _print_r() {
    std::cout << "('" << c << "'" << (is_word ? "!" : "");
    bool mid = false;
    for(auto p : leafs) {
      std::cout << (mid ? ", " : " ");
      p->_print_r();
      mid = true;
    }
    std::cout << (mid ? " " : "") << ")";
  }

  void print() {
    _print_r();
    std::cout << "\n";
  }

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

  void tlev1(const char * w, bool e) {
    bool r = lev1(w);
    std::cout << "lev1('" << w << "') -> " << (r?"true":"false") << ", expected " << (e?"true":"false") << ((e==r)?"":" - FAIL") << "\n";
  }
};

void base_trie_tests() {
  trie_t::ptr c1 = make_shared<trie_t>();
  c1->put("word");
  c1->put("work");
  c1->put("world");
  std::cout << "c1:\n";
  c1->print2();

  trie_t::ptr c2 = make_shared<trie_t>();
  c2->put("cord");
  c2->put("word");
  c2->put("words");
  std::cout << "c2:\n";
  c2->print2();

  trie_t::ptr c3 = make_shared<trie_t>();
  c3->put("world");
  c3->put("worlds");
  std::cout << "c3:\n";
  c3->print2();

  std::cout << "c1->merge(c2);\n";
  c1->merge(c2);
  c1->print2();

  std::cout << "c1->merge(c3);\n";
  c1->merge(c3);
  c1->print2();

  c1->tlev1("word", true);
  c1->tlev1("wor", true);
  c1->tlev1("wo", false);
  c1->tlev1("sword", true);
  c1->tlev1("zord", true);
  c1->tlev1("ward", true);
  c1->tlev1("worx", true);
  c1->tlev1("walk", false);
  c1->tlev1("wod", true);
}

int main() {
  set<trie_t::ptr> clusters;
  const char * words[] { "word", "work", "sword", "worlds", "worldz", "world" };
  for(auto word : words) {
    std::cout << "word = '" << word << "'\n";
    vector<trie_t::ptr> cv;
    for(auto c : clusters) {
      if(c->lev1(word)) {
        cv.push_back(c);
      }
    }
    if(cv.empty()) {
      trie_t::ptr t = make_shared<trie_t>();
      t->put(word);
      clusters.insert(t);
      std::cout << "added new cluster\n";
    } else {
      cv.front()->put(word);
      for(size_t i = 1; i != cv.size(); ++i) {
        std::cout << "merging: ";
        cv[i]->print2();
        cv.front()->merge(cv[i]);
        clusters.erase(cv[i]);
      }
      std::cout << "outcome: ";
      cv.front()->print2();
    }
  }
  for(int i = 0; i < 80; ++i) std::cout << "-"; std::cout << "\n";
  size_t i = 0;
  for(auto c : clusters) {
    std::cout << "#" << c << " ";
    c->print2();
  }
  return 0;
}
