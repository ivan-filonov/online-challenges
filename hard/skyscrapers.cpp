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
//	test();
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("(1,2,3);(2,4,6);(4,5,5);(7,3,11);(9,2,14);(13,7,15);(14,3,17)");
	std::cout << "1 2 2 4 4 5 5 4 6 0 7 3 11 2 13 7 15 3 17 0"    "\n";
	process("(2,22,3);(6,12,10);(15,6,21)");
	std::cout << "2 22 3 0 6 12 10 0 15 6 21 0"    "\n";
	process("(1,2,6);(9,23,22);(22,6,24);(8,14,19);(23,12,30)");
	std::cout << "1 2 6 0 8 14 9 23 22 6 23 12 30 0"    "\n";
}

void process_file(char* path) {
	std::ifstream stream(path);
	for(std::string line; std::getline(stream, line); ) {
		process(line);
	}
}

void process(std::string line) {
    std::istringstream ss { line };

    int ll, hh, rr;
    std::vector<int> h;
    while(ss) {
        ss.ignore(10, '(') >> ll;
        if(!ss) {
            break;
        }
        ss.ignore(10, ',') >> hh;
        ss.ignore(10, ',') >> rr;
        while(h.size() <= rr) {
            h.push_back(0);
        }
        for(int i = ll; i < rr; ++i) {
            h[i] = std::max( h[i], hh);
        }
    }

    int ch = 0;
    for(int ci = 0, mid = 0; ci < h.size(); ++ci) {
        if(ch == h[ci]) {
            continue;
        }
        ch = h[ci];
        if(mid) {
            std::cout << ' ';
        } else {
            mid = 1;
        }
        std::cout << ci << ' ' << ch;
    }
    std::cout << "\n";
}
