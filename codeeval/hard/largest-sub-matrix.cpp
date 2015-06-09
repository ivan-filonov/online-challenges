#include <fstream>
#include <iostream>
#include <sstream>
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

vector<vector<int>> preload;

void add_line(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  std::istringstream ss { line };
  vector<int> tv;
  for(int i; ss >> i; ) {
    tv.push_back(i);
  }
  preload.emplace_back(std::move(tv));
}

void run() {
  const int height = preload.size();
  const int width = preload[0].size();
  vector<int> t(width);
  int res = -10000;
  
  t.assign(width, 0);
  for(int top = 0; top != height; ++top) {
    for(int bottom = top; bottom != height; ++bottom) {
      for(int i = 0; i != width; ++i) {
        t[i] += preload[bottom][i];
      }
      int current = 0;
      for(int j : t) {
        if(current > 0) {
          current += j;
        } else {
          current = j;
        }
        if(current > res) {
          res = current;
        }
      }
    }
    t.assign(width, 0);
  }

  std::cout << res << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "-1 -4 -5 -4",
    "-5 8 -1 3",
    "-2 1 3 2",
    "1 5 6 -9",
  };
  for(auto & t : v_test) {
    add_line(t);
  }
  for(int i = 0; i < 80; ++i)std::cout<<'-';std::cout<<"\n";
  run();
  for(int i = 0; i < 80; ++i)std::cout<<'-';std::cout<<"\n";
  vector<string> v_expect {
    "22",
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
