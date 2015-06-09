/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

void test();
void process_file(char*);
int main(int argc, char ** argv) {
    if( 0 ) {
        test();
    } else {
        process_file(argv[1]);
    }
	return 0;
}

void process(std::string s);

void test() {
  process("Higher meaning;Hi mean");
  std::cout << "Hi____ mean___"    "\n";
  
  process("this is impossible;im possible");
  std::cout << "I cannot fix history"    "\n";
  
  process("twenty   two minutes;two minutes");
  std::cout << "______ two minutes"    "\n";
  
  process("Higher meaning;e");
  std::cout << "____e_ _______"    "\n";
}

void process_file(char* path) {
	std::ifstream stream(path);
	for(std::string line; std::getline(stream, line); ) {
		process(line);
	}
}

std::vector<std::string> get_words(std::string s) {
  std::vector<std::string> v;
  std::istringstream ss { s };
  for(std::string t; ss >> t;) {
    v.push_back(t);
  }
  return v;
}

template<typename _1>
void nop(_1, _1) {}

template<typename iter_t, typename op_t, typename f_t=void(iter_t,iter_t)>
bool test_subsequence (const iter_t &seq1_begin, const iter_t &seq1_end, const iter_t &seq2_begin, const iter_t &seq2_end, const op_t& op, const f_t& onyes = nop<iter_t>, const f_t& onno=nop<iter_t>) {
  auto it1 = seq1_begin;
  auto it2 = seq2_begin;
  while(seq1_end != it1 && seq2_end != it2) {
    if(op(*it1, *it2)) {
      onyes(it1,it2);
      ++it1;
      ++it2;
    } else {
      onno(it1,it1);
      ++it1;
    }
  }
  return seq2_end == it2;
}

void process(std::string line) {
  auto delim = line.find(';');
  auto words1 = get_words(line.substr(0, delim));
  auto words2 = get_words(line.substr(delim + 1));
  if(test_subsequence(words1.begin(), words1.end(), words2.begin(), words2.end(), [](const std::string& s1, const std::string& s2) {
    return test_subsequence(s1.begin(), s1.end(), s2.begin(), s2.end(), std::equal_to<char>());
  })) {
    bool mid = false;
    auto it1 = words1.begin();
    auto it2 = words2.begin();
    while(words1.end() != it1) {
      if(mid) {
        std::cout << ' ';
      } else {
        mid = true;
      }
      if(words2.end() != it2 && test_subsequence(it1->begin(), it1->end(), it2->begin(), it2->end(), std::equal_to<char>())) {
        const auto &w1 = *it1;
        const auto &w2 = *it2;
        for(auto ic1 = w1.begin(), ic2 = w2.begin(), e1 = w1.end(), e2 = w2.end(); e1 != ic1; ++ic1) {
          if(e2 != ic2 && *ic1 == *ic2) {
            std::cout << *ic1;
            ++ic2;
          } else {
            std::cout << '_';
          }
        }
        //std::cout << w1;
        ++it2;
      } else {
        for(auto c : *it1) {
          std::cout << '_';
        }
      }
      ++it1;
    }
    std::cout << "\n";
  } else {
    std::cout << "I cannot fix history\n";
  }
}

