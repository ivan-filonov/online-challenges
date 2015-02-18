/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

void test();
void process_file(char*);
int main(int argc, char ** argv) {
//	test();
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("100 Lower Lower Higher Lower Lower Lower Yay!");
	std::cout << "13"  "\n";
	process("948 Higher Lower Yay!");
	std::cout << "593"  "\n";
  // std::endl or std::flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
  std::istringstream ss { s };
  int rmin = 0;
  int rmax;
  ss >> rmax;
  ss.ignore(10, ' ');
  int mid;
  for(std::string t; std::getline(ss, t, ' ');) {
    mid = (rmin + rmax + 1) / 2;
    if( t == "Lower" ) {
      std::tie(rmin, rmax) = std::tuple<int,int>(rmin, mid - 1);
    } else if( t == "Higher" ) {
      std::tie(rmin, rmax) = std::tuple<int,int>(mid + 1, rmax);
    }
  }
  std::cout << mid << "\n";
}
