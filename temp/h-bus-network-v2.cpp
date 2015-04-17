#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;
template<typename K, typename V> using map = std::map<K,V>;
template<typename K> using set = std::set<K>;

const int MAX = 1073741823;

#ifdef TEST
void test();
#endif //#ifdef TEST
void process_file(char*);

int main(int argc, char ** argv) {
#ifdef TEST
  test();
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

string process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  std::istringstream ss { line };
  ss.ignore(MAX, '(');
  int A, B;
  ss >> A;
  ss.ignore(MAX, ',');
  ss >> B;
  
  if(A == B) {
    return "0";
  }

  while(ss.ignore(MAX, '[').tellg() > 0) {
    vector<int> v;
    for(char c = ','; ',' == c;) {
      v.push_back(0);
      ss >> v.back() >> c;
    }
  }
  return "None";
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "(2,4); R1=[1,2,3,11,12,4]; R2=[5,6,4]; R3=[1,6,7]; R4=[5,6,4]; R5=[8,6,3]",
    "(1,7); R1=[1,2,3,4]; R2=[5,6,4]; R3=[9,6,7]; R4=[12,1,2,3,11,16,15,14,10,13,7]",
    "(3,299); R1=[1,2,3,4]; R2=[6,7,19,12,4]; R3=[11,14,16,6]; R4=[24,299,42,6]",
    "(3,4); R1=[1,2,3]; R2=[6,7,19,12,4]; R3=[11,14,16,6]",
  };
  vector<string> v_expect {
    "28",
    "59",
    "73",
    "None",
  };
  for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
    std::cout << process(v_test[i]) << "\n";
    std::cout << v_expect[i] << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char* path) {
  std::ifstream stream(path);
  for(string line; std::getline(stream, line); ) {
    std::cout << process(line) << "\n";
  }
}
