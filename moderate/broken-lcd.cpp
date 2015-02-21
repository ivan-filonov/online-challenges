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
	process("10110001 11111000 11111110 11111111 11111111 11111111 11111111 11101101 11111111 01111111 11110010 10100111;84.525784");
	std::cout << "1"    "\n";
    
    process("11111111 11110110 11101111 11110111 10111110 11110110 10111011 10100111 11111100 01100100 11111101 01011110;5.57");
	std::cout << "1"    "\n";
    
    process("11000010 00001111 11111111 10111111 11101011 11110011 01111110 11011111 11111111 11111111 11111001 01101110;857.71284");
	std::cout << "1"    "\n";
    
    process("11111111 01110111 10111011 11001101 11111011 11101010 11110100 01001101 11011111 11111010 10010110 10111111;66.92");
	std::cout << "0"    "\n";
    
    process("11111011 10010001 11111011 11111101 10011111 10111110 01111100 11011101 10111001 11111110 11101111 11110110;188.87");
	std::cout << "0"    "\n";
    // std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	for(std::string line; std::getline(stream, line); ) {
		process(line);
	}
}

namespace {
    const std::vector<int> digits { 63, 6, 91, 79, 102, 109, 125, 7, 127, 111 };
}

void process(std::string line) {
    std::istringstream ss { line };
    // read lcd state
    std::vector<int> lcd(12);
    for(auto dst = std::begin(lcd), end = std::end(lcd); end != dst; ++dst) {
        int v = 0;
        int b = 1;
        for(int i = 0; i < 8; ++i) {
            if('1' == ss.get()) {
                v += b;
            }
            b *= 2;
        }
        *dst = v;
        ss.get();
    }
    // read digits
    std::vector<int> val;
    for(auto c = ss.get(); ss; c = ss.get()) {
        if(std::isdigit(c)) {
            val.push_back(digits[c-'0']);
        } else {
            val.back() += 128;
        }
    }
    // do compares
    for(int ofs = 0; ofs < lcd.size() - val.size() + 1 ; ++ofs) {
        int diff = 0;
        for(int i = 0; i < val.size() && !diff; ++i) {
            diff |= val[i] ^ (val[i] & lcd[ofs + i]);
        }
        if(!diff) {
            std::cout << "1\n";
            return;
        }
    }
    std::cout << "0\n";
}
