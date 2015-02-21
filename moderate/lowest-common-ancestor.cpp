/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace {
    std::vector<int> tree { 30, 8, 52, 3, 20, -1, -1, -1, -1, 10, 29 };
}

void test();
void process_file(char*);
int main(int argc, char ** argv) {
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("8 52");
	std::cout << "30"  "\n";
	process("3 29");
	std::cout << "8"  "\n";
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

std::vector<int> p(int x) {
    std::vector<int> res { x };
    auto i = std::find(std::begin(tree), std::end(tree), x) - std::begin(tree);
    while(i > 0) {
        i = (i-1)/2;
        res.push_back(tree[i]);
    }
    return res;
}

void process(std::string line) {
    std::istringstream ss { line };
    int a, b;
    ss >> a >> b;
    
    auto p1 = p(a);
    auto p2 = p(b);

    std::reverse(std::begin(p1), std::end(p1));
    std::reverse(std::begin(p2), std::end(p2));

    int i = 0;
    while(i < p1.size() && i < p2.size()) {
        if(p1[i] == p2[i]) {
            ++i;
        } else {
            break;
        }
    }
    std::cout << p1[i-1] << "\n";
}
