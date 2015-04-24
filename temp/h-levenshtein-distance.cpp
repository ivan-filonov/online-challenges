#include <fstream>
#include <iostream>
#include <map>
#include <memory>
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
template<typename K, typename V> using map = std::map<K,V>;
template<typename V> using shared_ptr = std::shared_ptr<V>;

using std::make_shared;
using std::move;
using std::swap;

vector<string> seed_words;
const string SEED_END { "END OF INPUT" };

struct word_t {
  shared_ptr<string> word;
  int bits;

  word_t(shared_ptr<string> _word) : word(_word), bits(0) {}
};

int wkey(int bigram, size_t wlen) {
  if((bigram & 0xFFFF0000) || (wlen & 0xFFFF0000)) {
    throw 2;
  }
  return (bigram << 16) ^ wlen;
}

map<string,shared_ptr<word_t>> dict;
map<int,vector<shared_ptr<word_t>>> data;

void add_word_to_bigram(int bigram, size_t wlen, shared_ptr<string> pword) {
  auto pwt = dict.find(*pword);
  if(dict.end() == pwt) {
    dict[*pword] = make_shared<word_t>(pword);
    pwt = dict.find(*pword);
  }
  data[wkey(bigram,wlen)].push_back(pwt->second);
}

void add_network_word(string && w) {
  // will need multiple references to each word
  const auto wlen = w.length();
  auto pw = make_shared<string>(move(w));
  // convert word to ngrams and add to index (mat)
  int nv = '$';
  map<int,int> bmap;
  for(auto c : *pw) {
    nv = 0xFFFF & ((nv << 8) | c);

    if(bmap.count(nv)) {
      continue;
    }
    bmap[nv] = 1;

    add_word_to_bigram(nv, wlen, pw);
  }
  nv = 0xFFFF & ((nv << 8) | '$');
  add_word_to_bigram(nv, wlen, pw);
}

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
    add_network_word(move(line));
  }
}

bool test_pair(shared_ptr<string> w1, shared_ptr<string> w2) {
  size_t l1 = w1->length();
  size_t l2 = w2->length();
  if(l1 == l2) {
    int n = 0;
    for(auto i1 = w1->begin(), e1 = w1->end(), i2 = w2->begin(); i1 != e1; ++i1, ++i2) {
      if(*i1 != *i2) {
        ++n;
        if(n > 1) {
          break;
        }
      }
    }
    return n < 2;
  }
  auto i1 = w1->begin(), i2 = w2->begin(), e1 = w1->end(), e2 = w2->end();
  if(l1 < l2) {
    swap(i1, i2);
    swap(e1, e2);
  }
  bool flag = true;
  while(i1 != e1 && i2 != e2) {
    if(*i1 == *i2) {
      ++i1;
      ++i2;
    } else {
      if(flag) {
        flag = false;
        ++i1;
      } else {
        return false;
      }
    }
  }
  return true;
}

void run() {
  int wbit = 1;
  for(auto & sw : seed_words) {
    vector<shared_ptr<string>> nw;
    nw.push_back(make_shared<string>(sw));
    while(!nw.empty()) {
      auto curw = nw.back();
      nw.pop_back();
      //...
      int nv = '$';
      const auto clen = curw->length();
      map<int,int> bmap;
      for(auto c : *curw) {
        nv = 0xFFFF & ((nv << 8) | c);
        if(bmap.count(nv)) {
          continue;
        }
        bmap[nv] = 1;
        const auto k = wkey(nv, clen);
        for(auto kk = k - 1; kk != k + 2; ++kk) {
          if(!data.count(kk)) {
            continue;
          }
          for(auto & ww : data[kk]) {
            if(ww->bits & wbit) {
              continue;
            }
            if(test_pair(ww->word, curw)) {
              ww->bits |= wbit;
              nw.push_back(ww->word);
            }
          }
        }
      }
      nv = 0xFFFF & ((nv << 8) | '$');
      const auto k = wkey(nv, clen);
      for(auto kk = k - 1; kk != k + 2; ++kk) {
        if(!data.count(kk)) {
          continue;
        }
        for(auto & ww : data[kk]) {
          if(ww->bits & wbit) {
            continue;
          }
          if(test_pair(ww->word, curw)) {
            ww->bits |= wbit;
            nw.push_back(ww->word);
          }
        }
      }
    }
    int c = 1;
    for(auto & p : dict) {
      if(p.second->bits & wbit) {
        ++c;
      }
    }
    std::cout << c << "\n";
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
