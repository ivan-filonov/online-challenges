/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <set>
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
	process("hat");
	std::cout << "aht,ath,hat,hta,tah,tha" << std::endl;
	process("abc");
	std::cout << "abc,acb,bac,bca,cab,cba" << std::endl;
	process("Zu6");
	std::cout << "6Zu,6uZ,Z6u,Zu6,u6Z,uZ6" << std::endl;
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
	std::set<std::string> ms;
	int c = 1;
	for(int n = 1; n <= s.length(); ++n) {
		c *= n;
	}
	for(int i = 0; i < c; ++i) {
		std::string r, t(s);
		r.reserve(s.length());
		int j = i;
		while(!t.empty()) {
			auto k = j % t.length();
			r.insert(r.end(), t[k]);
			j = j / t.length();
			t.erase(k, 1);
		}
		ms.insert(r);
	}
	int mid = false;
	for(auto it = ms.begin(), end = ms.end(); it != end; ++it) {
		if(mid) {
			std::cout << ',';
		} else {
			mid = true;
		}
		std::cout << *it;
	}
	std::cout << std::endl;
}
