#include <algorithm>
#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

string l2d { "22233344115566070778889990" };
char enc(char l) {
  return l2d.at(l - 'a');
//  return l2d[l - 'a'];
}

// digits-only trie:
struct trie_t {
  struct stat_s {
    char  word : 1;
    int word_id;

    stat_s() : word(0) {}
  };
  // [base * TWENTY_SIX + char]
  const int TWENTY_SIX = 10;
  const int BASE_CHAR = '0';

  vector<stat_s> stat;
  vector<int>  tail;
  int capacity;
  int next_base;

  static const int INITIAL_CAPACITY = 1000;

  trie_t() : stat(TWENTY_SIX * INITIAL_CAPACITY), tail(TWENTY_SIX * INITIAL_CAPACITY), capacity(INITIAL_CAPACITY), next_base(1) { }

  void grow (int new_cap) {
    stat.resize(new_cap * TWENTY_SIX);
    tail.resize(new_cap * TWENTY_SIX, 0);
    capacity = new_cap;
  }

  void put(const char * w, int word_id) {
    int base = 0, ofs;
    while(*w) {
      ofs = base * TWENTY_SIX + (*w - BASE_CHAR);
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
    stat[ofs].word_id = word_id;
  }

  bool _exact(const char * w, int base, int * word_id) {
    int ofs;
    while(*w) {
      ofs = base * TWENTY_SIX + (*w - BASE_CHAR);
      base = tail[ofs];
      if(!base) {
        break;
      }
      ++w;
    }
    if(base && stat[ofs].word) {
      *word_id = stat[ofs].word_id;
    }
    return base && stat[ofs].word;
  }

  bool exact(const char * w, int * word_id) {
    return _exact(w, 0, word_id);
  }
};

int main() {
  std::cin.sync_with_stdio(false);
  for(string pnum; std::cin >> pnum && pnum != "-1";) {
    int nwords;
    std::cin >> nwords;
    vector<string> v;
    v.reserve(nwords);
    size_t maxword = 0;
    trie_t t;
    for(string w; nwords-->0 && std::cin >> w;) {
      maxword = std::max(maxword, w.length());
      string ew = w;
      for(auto & c : ew) {
        c = enc(c);
      }
      t.put(ew.c_str(), v.size());
      v.emplace_back(std::move(w));
    }

    // [i] = минимальное количество слов для получения префикса длинны i 
    vector<int> prefix_nwords (pnum.length() + 1, 1000);
    // [i] - варианты склейки для длинны префикса i
    vector<vector<string>> prefix_answers (pnum.length() + 1);

    prefix_nwords[0] = 0;
    for(size_t prefix_len = 0; prefix_len != prefix_nwords.size(); ++prefix_len) {
      if(1000 == prefix_nwords[prefix_len]) {
        // префикс такой длинны собрать невозможно
        continue;
      }

      vector<char> buf(maxword + 1, 0);
      for(size_t word_len = 1; word_len <= maxword && prefix_len + word_len <= pnum.length(); ++word_len) {
        buf[word_len - 1] = pnum[prefix_len + word_len - 1];
        int word_id;
        if(t.exact(buf.data(), &word_id)) {
          if(prefix_nwords[prefix_len + word_len] > prefix_nwords[prefix_len] + 1) {
            prefix_nwords[prefix_len + word_len] = prefix_nwords[prefix_len] + 1;
            ;
            prefix_answers[prefix_len + word_len] = prefix_answers[prefix_len];
            prefix_answers[prefix_len + word_len].push_back(v[word_id]);
          }
        }
      }
    }

    if(1000 == prefix_nwords.back()) {
      std::cout << "No solution.\n";
    } else {
      bool mid = false;
      for(auto & s : prefix_answers.back()) {
        std::cout << (mid ? " " : "") << s;
        mid = true;
      }
      std::cout << "\n";
    }
  }
  return 0;
}
