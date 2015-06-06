#include <iostream>
#include <vector>

using string = std::string;
template<typename V> using vector = std::vector<V>;

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

  trie_t() : stat(26000), tail(26000), capacity(1000), next_base(1) {
  }

  void grow (int new_cap) {
    stat.resize(new_cap * 26);
    tail.resize(new_cap * 26, 0);
    capacity = new_cap;
  }

  void put(const char * w) {
    int base = 0, ofs;
    while(*w) {
      ofs = base * 26 + (*w - 'a');
      std::cout << "*w = '" << *w << "', base = " << base << ", ofs = " << ofs << "\n";
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

  bool exact(const char * w) {
    int base = 0, ofs;
    while(*w) {
      ofs = base * 26 + (*w - 'a');

      ++w;
    }
    return false;
  }
};

int main() {
  trie_t t;
  t.put("hello");
  t.put("help");
  return 0;
}

