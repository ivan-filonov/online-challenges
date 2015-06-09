/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void test();
void process_file(char*);
int main(int argc, char ** argv) {
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("-10,2,3,-2,0,5,-15");
	std::cout << "8"  "\n";// std::endl for flush?
	process("2,3,-2,-1,10");
	std::cout << "12"  "\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
    std::vector<int> v;
    
    std::istringstream ss { s };
    for(std::string t; std::getline(ss, t, ',');) {
        auto x = std::stoi(t);
        v.push_back(x);
    }

    auto msum = v[0];
    for(int i = 0; i < v.size(); ++i) {
        for(int j = i, csum = 0; j < v.size(); ++j) {
            csum += v[j];
            msum = std::max(csum, msum);
        }
    }
    std::cout << msum << "\n";
}
