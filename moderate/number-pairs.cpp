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

void test();
void process_file(char*);
int main(int argc, char ** argv) {
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
    process("1,2,3,4,6;5");
    std::cout << "1,4;2,3"    "\n";
    
    process("2,4,5,6,9,11,15;20");
    std::cout << "5,15;9,11"    "\n";
    
    process("1,2,3,4;50");
    std::cout << "NULL"    "\n";
    // std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	for(std::string line; std::getline(stream, line); ) {
		process(line);
	}
}

void process(std::string line) {
    auto sem = line.find(';');
    const int d = std::stoi(line.substr(sem + 1));
    line.erase(sem, line.length());

    std::istringstream ss { line };
    std::vector<int> v;
    for(std::string t; std::getline(ss, t, ','); ) {
        v.push_back(std::stoi(t));
    }

    std::sort(std::begin(v), std::end(v));
    
    int i = 0;
    int j = v.size() - 1;
    bool mid = false;
    while( i < j ) {
        const int ts = v[i] + v[j];
        if(ts == d) {
            if(mid) {
                std::cout << ';';
            } else {
                mid = true;
            }
            std::cout << v[i] << ',' << v[j];
            ++i;
        } else if(ts < d) {
            ++i;
        } else {
            --j;
        }
    }
    std::cout << (mid ? "\n" : "NULL\n");
}
