/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>

void test();
void process_file(char*);
int main(int argc, char ** argv) {
	test();
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
  process(":((");
  std::cout << "NO"  "\n";
  process("I am sick today (:()");
  std::cout << "YES"  "\n";
  process("(:)");
  std::cout << "YES"  "\n";
  process("Hacker cup: started :):)");
  std::cout << "YES"  "\n";
  process(")(");
  std::cout << "NO"  "\n";
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
  int prev = -1, d = 0, ml = 0, mr = 0;
  for(auto c : s) {
    if('(' == c) {
      if(':' == prev) {
        ++ml;
      } else {
        ++d;
      }
    } else if(')' == c) {
      if(':' == prev) {
        ++mr;
      } else {
        if( d > 0 ) {
          --d;
        } else {
          if(ml > 0) {
            --ml;
          } else {
            d = mr + 1;
            break;
          }
        }
      }
    }
    prev = c;
  }
  std::cout << ((d<=mr) ? "YES\n" : "NO\n");
}
