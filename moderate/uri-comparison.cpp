/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
//#include <iomanip>
#include <iostream>
//#include <iterator>
#include <sstream>
//#include <string>
//#include <vector>

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
	process("http://abc.com:80/~smith/home.html;http://ABC.com/%7Esmith/home.html");
	std::cout << "True"    "\n";
    // std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	for(std::string line; std::getline(stream, line); ) {
		process(line);
	}
}

struct uri {
  std::string schema;
  std::string host;
  std::string port;
  std::string path;

  void decode(std::string& s) {
    if(std::string::npos == s.find('%')) {
      return;
    }
    std::string s1;
    for(auto i = s.begin(), e = s.end(); e != i; ++i) {
      if('%' != *i) {
        s1.push_back(*i);
      } else {
        char c1 = *(++i);
        if(!std::isxdigit(c1)) {
          s1.push_back('%');
          i--;
          continue;
        }
        char c2 = *(++i);
        if(!std::isxdigit(c2)) {
          s1.push_back('%');
          i--;
          i--;
          continue;
        }
        std::string s2 { c1, c2 };
        s1.push_back(std::stoi(s2, nullptr, 16));
      }
    }
    s = s1;
  }

  uri(const std::string& s) {
    size_t ofs = 0;
    if(std::string::npos != s.find("://")) {
      schema = s.substr(0, ofs = s.find("://"));
      ofs += 3;
    }
    size_t he = s.find('/', ofs);
    std::string hp;
    if(std::string::npos != he) {
      hp = s.substr(ofs, he - ofs);
      path = s.substr(he + 1);
    } else {
      hp = s.substr(ofs);
    }
    size_t pp = hp.find(':');
    if(std::string::npos != pp) {
      host = hp.substr(0, pp);
      port = hp.substr(pp+1);
    } else {
      host = hp;
      port = "80";
    }
    decode(schema);
    decode(host);
    decode(path);
    std::transform(schema.begin(), schema.end(), schema.begin(), [](char c){return std::toupper(c);});
    std::transform(host.begin(), host.end(), host.begin(), [](char c){return std::toupper(c);});
/*    std::cout << "schema = " << schema << "\n";
    std::cout << "host = " << host << "\nport = " << port << "\n";
    std::cout << "path = " << path << "\n";
    */
  }

  bool operator == (const uri& other) const {
    return (schema == other.schema) && (port == other.port) && (host == other.host) && (path == other.path);
  }
};

void process(std::string line) {
  auto delim = line.find(';');
  uri u1 { line.substr(0, delim) };
  uri u2 { line.substr(delim+1) };
  std::cout << (u1==u2? "True\n" : "False\n");
}
