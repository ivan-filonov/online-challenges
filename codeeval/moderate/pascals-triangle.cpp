/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>

void test();
void process_file(char*);
int main(int argc, char ** argv) {
//	test();
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

int vat(int n, int r) {
  int nr = n - r;
  int res = n--;
  while(n > 1 || r > 1 || nr > 1) {
    if(r > 1 && !(res%r)) {
      res /= r--;
    }
    if(nr > 1 && !(res%nr)) {
      res /= nr--;
    }
    if(n > 1) {
      res *= n--;
    }
  }
  return res;
}

void test() {
  for(int n = 2; n < 7; ++n) {
    std::cout << "n = " << n << ", ";
    for(int r = 1; r < n; ++r) {
      std::cout << vat(n,r) << ' ';
    }
    std::cout << "\n";
  }
	process("6");
	std::cout << "1 1 1 1 2 1 1 3 3 1 1 4 6 4 1 1 5 10 10 5 1"  "\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
  auto n = std::stoi(s);
  std::cout << '1';
  if(n > 1) {
    std::cout << " 1 1";
  }
  if(n > 2) {
    for(int i = 2; i < n; ++i) {
      std::cout << " 1";
      for(int j = 1; j < i; ++j) {
        std::cout << ' ' << vat(i, j);
      }
      std::cout << " 1";
    }
  }
  std::cout << "\n";
}
