/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void test();
void process_file(char*);
int main(int argc, char ** argv) {
//	test();
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("159");
	std::cout << "CLIX"  "\n";// std::endl for flush?
	process("296");
	std::cout << "CCXCVI"  "\n";// std::endl for flush?
	process("3992");
	std::cout << "MMMCMXCII"  "\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

namespace {
  std::vector<int> nums { 1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1 };
  std::vector<std::string> roms { "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I" };
};

void process(std::string s) {
  auto v = std::stoi(s);
  for(int i = 0; i < roms.size(); ++i) {
    while(v >= nums[i]) {
      v -= nums[i];
      std::cout << roms[i];
    }
  }
  std::cout << "\n";
}
