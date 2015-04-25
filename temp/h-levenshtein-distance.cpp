#include <fstream>
#include <iostream>
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

using std::move;
using std::swap;

vector<string> seed_words;

const string SEED_END { "END OF INPUT" };
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
  }
}

void run() {
  for(auto & sw : seed_words) {
    std::cout << "1\n";
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
