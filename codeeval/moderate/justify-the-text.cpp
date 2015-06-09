/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("Hello, World!");
	std::cout << "Hello, World!" << std::endl;

	process("The precise 50-digits value of Pi is 3.14159265358979323846264338327950288419716939937510.");
	std::cout << "The         precise         50-digits        value        of        Pi        is" << std::endl;
	std::cout << "3.14159265358979323846264338327950288419716939937510." << std::endl;

	process("But he who would be a great man ought to regard, not himself or his interests, but what is just, whether the just act be his own or that of another. Next as to habitations. Such is the tradition.");
	std::cout << "But  he  who would be a great man ought to regard, not himself or his interests," << std::endl;
	std::cout << "but what is just, whether the just act be his own or that of another. Next as to" << std::endl;
	std::cout << "habitations. Such is the tradition." << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void print_vec(std::vector<std::string> &vec, int spaces = 1, int add = 0) {
	for(auto b = std::begin(vec), it = b, e = std::end(vec); e != it; ++it) {
		if(it != b) {
			for(int i = 0; i < spaces + (add?1:0); ++i) {
				std::cout << ' ';
			}
			if(add) {
				--add;
			}
		}
		std::cout << *it;
	}
	std::cout << std::endl;
}

void process(std::string s) {
	std::istringstream ss(s);
	std::vector<std::string> words;
	std::string w;
	size_t wl = 0;
	while(std::getline(ss, w, ' ')) {
		if( wl + 1 + w.length() > 81 ) {
			auto needed = (81 - wl);
			auto spc = needed / (words.size() - 1) + 1;
			auto add = needed % (words.size() - 1);
			print_vec(words, spc, add);

			words.clear();
			wl = 0;
		}
		words.push_back(w);
		wl += w.length() + 1;
	}
	print_vec(words);
}
