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

namespace {
    std::vector<std::string> vm(10);
    int cx = 0;
    int cy = 0;
    bool ins = false;
}

void test() {
	process("^h^c");
	process("^04^^");
	process("^13/ \\^d^b  /   \\");
	process("^u^d^d^l^l^l^l^l^l^l^l^l");
	process("^r^r^l^l^d<CodeEval >^l^l^d/^b \\");
	process("^d^r^r^66/^b  \\");
	process("^b^d   \\ /");
	process("^d^l^lv^d^b===========^i^94O123456");
	process("789^94A=======^u^u^u^u^u^u^l^l\\^o^b^r/");
	

    for(auto& s : vm) {
        std::cout << s << "\n";
    }
}

void process_file(char* path) {
	std::ifstream stream(path);

    vm.clear();
    cx = cy = 0;
    ins = false;
    for(int i = 0; i < 10; ++i) {
        vm.push_back(std::string(10, ' '));
    }
	
    std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}

    for(auto& s : vm) {
        std::cout << s << "\n";
    }
}

void process(std::string s) {
    int cmd = 0;
    int cmd2 = 0;
    int d0 = 0;
    for(auto c : s) {
        if(cmd2) {
            cmd2 = 0;
            cx = c - '0';
            cy = d0;
            continue;
        }
        if(!cmd && '^' == c) {
            cmd = 1;
            continue;
        }
        if(cmd) {
            switch(c) {
                case 'c':
                    for(int i = 0; i < 10; ++i) {
                        vm[i] = std::string(10, ' ');
                    }
                    break;
                case 'h':
                    cx = cy = 0;
                    break;
                case 'b':
                    cx = 0;
                    break;
                case 'd':
                    cy = std::min(9, cy + 1);
                    break;
                case 'u':
                    cy = std::max(0, cy - 1);
                    break;
                case 'l':
                    cx = std::max(0, cx - 1);
                    break;
                case 'r':
                    cx = std::min(9, cx + 1);
                    break;
                case 'e':
                    for(int i = cx; i < 10; ++i) {
                        vm[cy][i] = ' ';
                    }
                    break;
                case 'i':
                    ins = true;
                    break;
                case 'o':
                    ins = false;
                    break;
                case '0':   case '1':   case '2':   case '3':   case '4':
                case '5':   case '6':   case '7':   case '8':   case '9':
                    cmd2 = 1;
                    d0 = c - '0';
                    break;
            }
            cmd = 0;
            if('^' != c) {
                continue;
            }
        }
        if(ins) {
            vm[cy].insert(cx, 1, c);
            vm[cy].pop_back();
        } else {
            vm[cy][cx] = c;
        }
        if( cx < 9 ) {
            ++cx;
        }
    }
}
