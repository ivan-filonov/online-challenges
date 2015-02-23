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
    if( !1 ) {
        test();
    } else {
        process_file(argv[1]);
    }
	return 0;
}

void process(std::string s);

void test() {
    process("15.94;16.00");
    std::cout << "NICKEL,PENNY"    "\n";
    
    process("17;16");
    std::cout << "ERROR"    "\n";
    
    process("35;35");
    std::cout << "ZERO"    "\n";
    
    process("45;50");
    std::cout << "FIVE"    "\n";
}

void process_file(char* path) {
	std::ifstream stream(path);
	for(std::string line; std::getline(stream, line); ) {
		process(line);
	}
}

namespace {
    std::vector<std::pair<std::string,double>> ctab {
        { "ONE HUNDRED", 100.00 },
        { "FIFTY", 50.00 },
        { "TWENTY", 20.00 },
        { "TEN", 10.00 },
        { "FIVE", 5.00 },
        { "TWO", 2.00 },
        { "ONE", 1.00 },
        { "HALF DOLLAR", .50 },
        { "QUARTER", .25 },
        { "DIME", .10 },
        { "NICKEL", .05 },
        { "PENNY", .01 },
    };
}

void process(std::string line) {
	size_t delim;
    auto PP = (int)(100.0 * std::stod(line, &delim));
    auto CH = (int)(100.0 * std::stod(line.substr(delim + 1)));
    if(PP == CH) {//TODO: double comparison!
        std::cout << "ZERO\n";
    } else if(PP > CH) {
        std::cout << "ERROR\n";
    } else {
        auto diff = CH - PP;
        bool mid = false;
        for(auto it = std::begin(ctab), end = std::end(ctab); end != it; ++it) {
            auto itv = (int)(100.0 * it->second);
            while(diff >= itv) {
                if(mid) {
                    std::cout << ',';
                } else {
                    mid = true;
                }
                std::cout << it->first;
                diff -= itv;
            }
        }
        std::cout << "\n";
    }
}
