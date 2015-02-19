/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
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
    process("2,3,1,0,-4,-1");
    std::cout << "2"  "\n";// std::endl for flush?
    process("0,-1,3,-2");
    std::cout << "1"  "\n";// std::endl for flush?
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
    std::vector<int> v;
    for(std::string t; std::getline(ss, t, ',');) {
        v.push_back(std::stoi(t));
    }
    int c = 0;
    for(int i = 0; i < v.size() - 3; ++i) {
        for(int j = i + 1; j < v.size() - 2; ++j) {
            for(int k = j + 1; k < v.size() - 1; ++k) {
                for(int l = k + 1; l < v.size(); ++l) {
                    if(0 == v[i] + v[j] + v[k] + v[l]) {
                        ++c;
                    }
                }
            }
        }
    }
    std::cout << c << "\n";
}
