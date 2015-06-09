/*
 * Common base C++ source for codeeval solutions.
 * */
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void process_file(char*);
int main(int argc, char ** argv) {
	process_file(argv[1]);
	return 0;
}

void process(std::vector<std::string>);

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
  std::vector<std::string> m;
	while(std::getline(stream, line)) {
    m.push_back(line);
	}
  process(m);
}

/*
def places(s):
	idx = [i for i in range(len(s)) if 'C' == s[i]] + [i for i in range(len(s)) if '_' == s[i]]
	return idx
def process(tr):
	p = places(tr[0])[0]
	for si in range(len(tr)):
		s = tr[si]
		#print(s)
		for i in places(s):
			if abs(p-i)<2:
				j = i
				break
		s=s[:j]+'/|\\'[j-p+1]+s[j+1:]
		p = j
		print(s)
*/
std::vector<int> places(std::string s) {
  std::vector<int> v;
  for(int i = 0; i < s.length(); ++i) {
    if('C' == s[i]) {
      v.push_back(i);
    }
  }
  for(int i = 0; i < s.length(); ++i) {
    if('_' == s[i]) {
      v.push_back(i);
    }
  }
  return v;
}

void process(std::vector<std::string> m) {
  auto p = places(m[0])[0];
  for(int si = 0, j; si < m.size(); ++ si) {
    auto s = m[si];
    for(int i : places(s)) {
      if(std::abs(p-i) < 2) {
        j = i;
        break;
      }
    }
    s[j] = "/|\\"[j-p+1];
    std::cout << s << "\n";
    p = j;
  }
}
