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
    return false;
  }
};

int main() {
  trie_t t, t2;
  for(auto s : { "hello", "help", "world", "word", "work", }) {
    t2.put(s);
  }
  t2.merge_to(t);
  for(auto s : {"hello", "hell", "helo", "help", "hellox", }) {
    std::cout << "t.exact(\"" << s << "\") -> " << t.exact(s) << "\n";
  }
  t.print();
  for(auto s : { "whelp", "hellooo" }) {
    std::cout << "t.lev1(\"" << s << "\") -> " << t.lev1(s) << "\n";
  }
  return 0;
}

