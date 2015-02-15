/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("Amy Lawrence was proud and glad, and she tried to make Tom see it in her face - but he wouldn't look.");
	std::cout << "Amy Lawrence was proud and glad, and... <Read More>" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	const std::string more = "... <Read More>";
	if(s.length() > 55) {
		s.erase(40);
		auto sp = s.rfind(' ');
		if(std::string::npos != sp) {
			s.erase(sp);
		}
		s.append(more);
	}
	std::cout << s << std::endl;
}
