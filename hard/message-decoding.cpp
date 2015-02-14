/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
}

void process(std::string s);

void test() {
	process("$#**\\0100000101101100011100101000");
	std::cout << "##*\\$" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	std::vector<int> cbase(8);
	cbase[0] = 0;
	for(int i = 1; i < 8; ++i) {
		cbase[i] = cbase[i-1] + (1<<i) - 1;
	}

	auto delim = s.find_last_not_of("01") + 1;
	auto msg = s.substr(delim);
	s.erase(delim);
	int ofs = 0;
	while(ofs < msg.length()) {
		auto code_len = std::stoi(msg.substr(ofs,3), nullptr, 2);
		ofs += 3;
		if(0 == code_len) {
			break;
		}

		while(ofs < msg.length()) {
			auto code = std::stoi(msg.substr(ofs, code_len), nullptr, 2);
			ofs += code_len;
			if((1 << code_len) - 1 == code) {
				break;
			}
			std::cout << s[code + cbase[code_len-1]];
		}
	}
	std::cout << std::endl;
}
