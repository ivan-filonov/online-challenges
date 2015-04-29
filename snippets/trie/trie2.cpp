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
      std::cout << (*mid ? "', '" : "words: '");
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
      std::cout << "'\n";
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
      ptr l;
      for(auto ll : leafs) {
        if(ll->c == ol->c) {
          l = ll;
          break;
        }
      }
      if(!l) {
        leafs.push_back(ol);
      } else {
        l->merge(ol);
      }
    }
  }
};

int main() {
  trie_t::ptr c1 = make_shared<trie_t>();
  c1->put("word");
  c1->put("world");
  std::cout << "c1:\n";
  c1->print2();

  trie_t::ptr c2 = make_shared<trie_t>();
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

  return 0;
}
