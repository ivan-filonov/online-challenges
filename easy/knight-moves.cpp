/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void prepare();
void test();
void process_file(char*);
int main(int argc, char ** argv) {
    prepare();
//	test();
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
    process("g2");
    std::cout << "e1 e3 f4 h4"  "\n";// std::endl for flush?
    process("a1");
    std::cout << "b3 c2"  "\n";// std::endl for flush?
    process("d6");
    std::cout << "b5 b7 c4 c8 e4 e8 f5 f7"  "\n";// std::endl for flush?
    process("e5");
    std::cout << "c4 c6 d3 d7 f3 f7 g4 g6"  "\n";// std::endl for flush?
    process("b1");
    std::cout << "a3 c3 d2"  "\n";// std::endl for flush?
}

namespace {
    using md_t = std::pair<int,int>;
    std::vector<md_t> km;
}

void prepare() {
    for(int sx=-1; sx <=1; sx+=2) {
        for(int sy=-1; sy <=1; sy +=2) {
            for(int a = 0; a <=1; ++a) {
                km.push_back({ (2-a)*sx, (1+a)*sy });
            }
        }
    }
    std::sort(std::begin(km), std::end(km), [](const md_t& a, const md_t& b) {
        if(a.first < b.first) {
            return true;
        }
        if(a.first == b.first && a.second < b.second) {
            return true;
        }
        return false;
    });
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
    md_t c { s[0] - 'a', s[1] - '1' };
    for(int i = 0, mid = 0; i < km.size(); ++i) {
        auto x = c.first + km[i].first;
        auto y = c.second + km[i].second;
//        std::cout << x << y << ' ';
        if(x<0||y<0||x>7||y>7) {
            continue;
        }
        if(mid) {
            std::cout << ' ';
        } else {
            mid = 1;
        }
        std::cout << (char)('a' + x) << (char)('1' + y);
    }
    std::cout << "\n";
}
