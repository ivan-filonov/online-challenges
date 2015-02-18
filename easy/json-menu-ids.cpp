/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>

void test();
void process_file(char*);
int main(int argc, char ** argv) {
//	test();
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("{\"menu\": {\"header\": \"menu\", \"items\": [{\"id\": 27}, {\"id\": 0, \"label\": \"Label 0\"}, null, {\"id\": 93}, {\"id\": 85}, {\"id\": 54}, null, {\"id\": 46, \"label\": \"Label 46\"}]}}");
	std::cout << "46"  "\n";// std::endl for flush?

	process("{\"menu\": {\"header\": \"menu\", \"items\": [{\"id\": 81}]}}");
	std::cout << "0"  "\n";// std::endl for flush?

	process("{\"menu\": {\"header\": \"menu\", \"items\": [{\"id\": 70, \"label\": \"Label 70\"}, {\"id\": 85, \"label\": \"Label 85\"}, {\"id\": 93, \"label\": \"Label 93\"}, {\"id\": 2}]}}");
	std::cout << "248"  "\n";// std::endl for flush?

}

void process_file(char* path) {
  std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

namespace {
  const std::string cs_id { "\"id\": " };
  const std::string cs_l { ", \"label\": " };
}

void process(std::string s) {
//	std::cout << "s = '" << s << "'\n";
  int sum = 0;
  size_t ofs = 0;
  while(std::string::npos != (ofs = s.find(cs_id, ofs))) {
//    std::cout << "A#" << s.substr(ofs) << "\n";
    ofs += cs_id.length();
    int i = 0;
//    std::cout << "B#" << s.substr(ofs) << "\n";
    while(std::isdigit(s[ofs])) {
      i = i * 10 + (s[ofs++] - '0');
    }
//    std::cout << "C#" << s.substr(ofs) << "\n";
    if(s.find(cs_l, ofs) == ofs) {
      sum += i;
    }
  }
  std::cout << sum << "\n";
}
