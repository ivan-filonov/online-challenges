/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("3,5;**.........*...");
	std::cout << "**100332001*100"  "\n";// std::endl for flush?
	process("4,4;*........*......");
	std::cout << "*10022101*101110"  "\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	std::istringstream ss(s);
	std::string m;
	std::getline(ss, m, ',');
    const auto hei = std::stoi(m);
	std::getline(ss, m, ';');
    const auto wid = std::stoi(m);
	std::getline(ss, m);
    auto is_cell = [=](int h,int v){ return (h >= 0) && (v >= 0) && (h < wid) && (v < hei); };
    auto test_cell = [&m, &is_cell, wid, hei](int h, int v){
        if( !is_cell(h,v) ) {
            return 0;
        }
        return ('*' == m[h + v * wid]) ? 1 : 0;
    };
    for(int v = 0; v < hei; ++v) {
        for(int h = 0; h < wid; ++h) {
            if( '*' == m[h + v * wid] ) {
                continue;
            }
            int c = 0;
            for(int dv = -1; dv <= 1; ++dv) {
                for(int dh = -1; dh <= 1; ++dh) {
                    if(0 == dh && 0 == dv) {
                        continue;
                    }
                    c += test_cell(h + dh, v + dv);
                }
            }
            m[h + v * wid] = c + '0';
        }
    }
    std::cout << m << "\n";
}
