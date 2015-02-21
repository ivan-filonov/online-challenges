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

void process_stream(std::istream &stream);

void test() {
    std::istringstream ss { "2\n4,6\n2,8\n3\n1,2,3\n4,5,6\n7,8,9" };
    process_stream( ss );
    std::cout << "14\n21\n";
}


void process_file(char* path) {
    std::ifstream fs { path };
	process_stream( fs );
}

void min_path_sum(int n, const std::vector<std::vector<int>>& m);

void process_stream(std::istream &stream) {
    for(std::string t; std::getline(stream, t);) {
        auto const n = std::stoi(t);
        std::vector<std::vector<int>> m ( n );
        for(int i = 0; i < n; ++i) {
            std::getline(stream, t);
            std::istringstream ss { t };
            for(std::string tt; std::getline(ss, tt, ','); ) {
                m[i].push_back(std::stoi(tt));
            }
        }
        min_path_sum(n, m);
    }
}

void min_path_sum(int n, const std::vector<std::vector<int>>& m) {
    std::vector<std::vector<int>> c(n, std::vector<int>(n, 0));
    c[0][0] = m[0][0];
    for(int i = 1; i < n; ++i) {
        c[0][i] = c[0][i-1] + m[0][i];
        c[i][0] = c[i-1][0] + m[i][0];
    }
    for(int v = 1; v < n; ++v) {
        for(int h = 1; h < n; ++h) {
            c[v][h] = m[v][h] + std::min(c[v-1][h], c[v][h-1]);
        }
    }
    std::cout << c.back().back() << "\n";
}
