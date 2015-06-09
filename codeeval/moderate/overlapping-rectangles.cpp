/*
 * Common base C++ source for codeeval solutions.
 * */
//#include <algorithm>
#include <fstream>
//#include <iomanip>
#include <iostream>
//#include <iterator>
#include <sstream>
//#include <string>
#include <vector>

void test();
void process_file(char*);
int main(int argc, char ** argv) {
    if( !1 ) {
        test();
    } else {
        process_file(argv[1]);
    }
	return 0;
}

void process(std::string s);

void test() {
  process("-3,3,-1,1,1,-1,3,-3");
  std::cout << "False"    "\n";

  process("-3,3,-1,1,-2,4,2,2");
  std::cout << "True"    "\n";
}

void process_file(char* path) {
	std::ifstream stream(path);
	for(std::string line; std::getline(stream, line); ) {
		process(line);
	}
}

void process(std::string line) {
  std::istringstream ss { line };
  std::vector<int> v;
  for(std::string t; std::getline(ss, t, ',');) {
    v.push_back(std::stoi(t));
  }
  /* 0 A left, 
   * 1 A top, 
   * 2 A right, 
   * 3 A bottom, 
   * 4 B left, 
   * 5 B top, 
   * 6 B right, 
   * 7 B bottom */
  if(v[0] > v[6] || v[4] > v[2] || v[1] < v[7] || v[5] < v[3]) {
    std::cout << "False\n";
  } else {
    std::cout << "True\n";
  }
}
