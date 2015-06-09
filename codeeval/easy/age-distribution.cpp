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
	process("0");
	process("19");
//	std::cout << ""  "\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

namespace {
    std::vector<std::string> v {
        "Still in Mama's arms",
        "Preschool Maniac",
        "Elementary school",
        "Middle school",
        "High school",
        "College",
        "Working for the man",
        "The Golden Years",
        "This program is for humans",
    };
}

void process(std::string s) {
//	std::cout << "s = '" << s << "'\n";
    auto i = std::stoi(s);
    int idx = 0;
    if(i < 0) i = 101;
    if(i > 2) ++idx;
    if(i > 4) ++idx;
    if(i > 11) ++idx;
    if(i > 14) ++idx;
    if(i > 18) ++idx;
    if(i > 22) ++idx;
    if(i > 65) ++idx;
    if(i > 100) ++idx;
    std::cout << v[idx] << "\n";
}
