#include <fstream>
#include <iostream>
#include <map>
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

using std::move;

vector<string> seed_words;
const string SEED_END { "END OF INPUT" };

// exact words
vector<string> words;
// word stats
vector<int> wstats;
// maps: bigrams -> lengths -> ids
map<int,map<int,int>> mat;

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
    // convert word to ngrams and add to index (mat)
    int nv = '$';
    const int id = words.size();
    const int ll = line.length();
    for(auto c : line) {
      nv = 0xFFFF & ((nv << 8) | c);
      mat[nv][ll] = id;
    }
    nv = 0xFFFF & ((nv << 8) | '$');
    mat[nv][ll] = id;
    
    words.emplace_back(move(line));
    wstats.push_back(0);
  }
}

void run() {
  std::cout << "seed_words.size() = " << seed_words.size() << "\n";
  std::cout << "words.size() = " << words.size() << "\n";
  std::cout << "wstats.size() = " << wstats.size() << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "recursiveness",
    "elastic",
    "macrographies",
    "END OF INPUT",
    "aa",
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
