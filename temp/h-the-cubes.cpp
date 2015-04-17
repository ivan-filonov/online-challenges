#include <fstream>
#include <iostream>
#include <vector>

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

using std::string;
template<typename V> using vector = std::vector<V>;

int process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  const auto N = std::stoi(line);
  vector<char> v(N*N*N);
  for(size_t si = line.find(';') + 1, di = 0; si != line.length(); ++si, ++di) {
    v[di] = line[si];
  }

  return 0;
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "5;******   ** ****   *** ********  o**o* ** * ************   ** * **o* ************ o **** **   ******** ***o  ****o**o  ******",
    "5;****** * ** * **   *** ********o*o** *o**   ************   **o* ** *o************   **** **   ******** *** o ****o**  o******",
    "7;********     ** *** ** * * ** * * **     **** *********** *   ** *** **   *o**o***o**oo   ****************  oo ** * * ** * * ** * * ** *o o****************     ** * ****o* * ** *** **o    ****************     ** *** **     ****** ** o  o**************** *oo ** *** **o  * ** *** **o    *********** ****   * ** * * ** * *o** ***o**     ********",
  };
  vector<string> v_expect {
    "15",
    "0",
    "39",
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
