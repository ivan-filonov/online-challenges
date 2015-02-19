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
int main(int argc, char ** argv) {
//	test();
	process_file(argv[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("3;3;1 2 3 4 5 6 7 8 9");
	std::cout << "1 2 3 6 9 8 7 4 5"  "\n";// std::endl for flush?
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

void process(std::string s) {
//	std::cout << "s = '" << s << "'\n";
  std::istringstream ss(s);
  std::string t;
  std::getline(ss, t, ';');
  auto hei = std::stoi(t);
  std::getline(ss, t, ';');
  auto wid = std::stoi(t);
  std::vector<std::string> m;
  m.reserve(wid * hei);
  while(std::getline(ss, t, ' ')) {
    m.push_back(t);
  }
  int x = 0, y = 0;//initial point
  int dx = 1, dy = 0;//initial vector
  const int k11 = 0, k12 = -1,//x1 = k11*x+k12*y
            k21 = 1, k22 = 0;//y1 = k21*x + k22*y
  std::vector<int> lim { -1, wid, 0, hei};//borders, x bw [0]..[1], y bw [2]..[3]
  for(int i = 0 ; i < m.size(); ++i) {
      if( x || y ) {
          std::cout << ' ';
      }
//    std::cout << '"' << std::flush;
    std::cout << m[x + y * wid];
//    std::cout << "\" "  << std::flush;
    bool rotate = false;
    int x1 = x + dx;
    int y1 = y + dy;
    if(dx) {
//      std::cout << " x" << dx << " " << std::flush;
      if(x1 == lim[0]) {
        rotate = true;
//        std::cout << "x = " << x << ", lim[0] = " << lim[0] << "\n";
        ++lim[0];
      } else if(x1 == lim[1]) {
        rotate = true;
//        std::cout << "x = " << x << ", lim[1] = " << lim[1] << "\n";
        --lim[1];
      }
    }
    if(dy) {
//      std::cout << " y" << dy << " " << std::flush;
      if(y1 == lim[2]) {
        rotate = true;
//        std::cout << "y = " << x << ", lim[2] = " << lim[2] << "\n";
        ++lim[2];
      } else if(y1 == lim[3]) {
        rotate = true;
//        std::cout << "y = " << x << ", lim[3] = " << lim[3] << "\n";
        --lim[3];
      }
    }
    if(rotate) {
//      std::cout << "before rotate: dx = " << dx << ", dy = " << dy << "\n" << std::flush;
      int dx1 = k11 * dx + k12 * dy;
      int dy1 = k21 * dx + k22 * dy;
      dx = dx1;
      dy = dy1;
//      std::cout << "after rotate: dx = " << dx << ", dy = " << dy << "\n" << std::flush;
    }
//    std::cout << "[x = " << x << ", y = " << y << ", dx = " << dx << ", dy = " << dy << "]" << std::flush;
    x += dx;
    y += dy;
  }
  std::cout << "\n";
}
