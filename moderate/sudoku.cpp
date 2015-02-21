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
	process("4;1,4,2,3,2,3,1,4,4,2,3,1,3,1,4,2");
	std::cout << "True"    "\n";
	process("4;2,1,3,2,3,2,1,4,1,4,2,3,2,3,4,1");
	std::cout << "False"    "\n";
    // std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	for(std::string line; std::getline(stream, line); ) {
		process(line);
	}
}

void process(std::string line) {
    auto N = std::stoi(line);

    std::istringstream ss { line.substr(line.find(';') + 1) };
    std::vector<int> m;
    m.reserve(N * N);
    for(std::string t; std::getline(ss, t, ','); ) {
        m.push_back(std::stoi(t));
    }
    auto get = [&m, N](int h, int v) { return m[h + N * v]; };
    for(int v = 0; v < N; ++v) {
        std::vector<int> t (N, 0);
        std::vector<int> t1 (N, 0);
        for(int h = 0; h < N; ++h) {
            t[get(h,v) - 1] = 1;
            t1[get(v,h) - 1] = 1;
        }
        if(std::accumulate(std::begin(t), std::end(t), 0) != N || std::accumulate(std::begin(t1), std::end(t1), 0) != N) {
            std::cout << "False\n";
            return;
        }
    }
    if(9 == N) {
        for(int v = 0; v < 9; v += 3) {
            for(int h = 0; h < 9; h += 3) {
                std::vector<int> t (9, 0);
                for(int vv = 0; vv < 3; ++vv) {
                    for(int hh = 0; hh < 3; ++hh) {
                        t[get(h+hh,v+vv)-1]=1;
                    }
                }
                if( std::accumulate(std::begin(t), std::end(t), 0) != N ) {
                    std::cout << "False\n";
                    return;
                }
            }
        }
    }
    std::cout << "True\n";
}
