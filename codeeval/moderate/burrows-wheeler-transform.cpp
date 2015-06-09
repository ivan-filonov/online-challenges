/*
 * Common base C++ source for codeeval solutions.
 * TODO: learm more about burrows-wheeler transform!!!
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
	process("oooooooo$  ffffffff     ffffffffuuuuuuuuaaaaaaaallllllllbbBbbBBb|");
	std::cout << "Buffalo buffalo Buffalo buffalo buffalo buffalo Buffalo buffalo$"    "\n";

	process("edarddddddddddntensr$  ehhhhhhhhhhhJ aeaaaaaaaaaaalhtf thmbfe           tcwohiahoJ eeec t e |");
	std::cout << "James while John had had had had had had had had had had had a better effect on the teacher$"    "\n";
	
    process("ooooio,io$Nnssshhhjo  ee  o  nnkkkkkkii |");
	std::cout << "Neko no ko koneko, shishi no ko kojishi$"    "\n";
}

void process_file(char* path) {
	std::ifstream stream(path);
	for(std::string line; std::getline(stream, line); ) {
		process(line);
	}
}

void process(std::string line) {
    if(line.back() != '|') {
        throw std::exception();
    } else {
        line.pop_back();
    }

    auto col1 = line;
    std::sort(col1.begin(), col1.end());

    // generate shortcut lists bstart and sk
    std::vector<int> count (256, 0);
    std::vector<int> bstart (256, -1);
    std::vector<int> sk (line.length(),-1);
    std::string res(line.length(), ' ');
    for(int i  = 0; i < line.length(); ++i) {
        auto si = line[i];
        sk[i] = count[si]++;
        si = col1[i];
        if( -1 == bstart[si] ) {
            bstart[si] = i;
        }
    }

    int li = line.find('$');
    for(int i = 0; i < line.length(); ++i) {
        if(li < 0 || li >= line.length()) {
            throw std::exception();
        }
        auto nb = line[li];
        res[line.length() - i - 1] = nb;
        // update index for next output byte
        li = bstart[nb] + sk[li];
    }

    std::cout << res << "\n";
}
