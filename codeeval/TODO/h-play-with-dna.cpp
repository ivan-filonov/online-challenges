/*
 * Play with DNA
 *
 * Challenge Description:
 *
 * This challenge is related to bioinformatics. To help in a DNA research, you have to write an algorithm that finds all the occurrences of a DNA segment in a given DNA string. But, it would be too easy for you. So, write an algorithm with the maximum N number of allowed mismatches. By mismatch we mean the minimum of the total number of substitutions, deletions, and insertions that must be performed on a DNA slice to completely match a given segment. You need to compare the DNA slices with the same length as a given pattern (for example, the segments 'AGTTATC' -> 'AGTATGC' have only 2 mismatches).
 *
 * For the DNA string 'CGCCCGAATCCAG' and the segment 'CCC', the first match with the maximum 1 allowed mismatch is 'CGC', the second one is 'GCC', the third one is 'CCC', and so on.
 *
 * CCC -> CGC # One mismatch
 * CCC -> GCC # One mismatch
 * CCC -> CCC # 0 mismatch
 *
 * For the given segment 'CCC', the DNA string 'CGCCCGAATCCAG', and the maximum allowed mismatch '1', the list of the matches is 'CGC GCC CCC CCG TCC CCA'.
 * 
 * Input sample:
 * CCC 1 CGCCCGAATCCAG
 * GCGAG 2 CCACGGCCTATGTATTTGCAAGGATCTGGGCCAGCTAAATCAGCACCCCTGGAACGGCAAGGTTCATTTTGTTGCGCGCATAG
 * CGGCGCC 1 ACCCCCGCAGCCATATGTCCCCAGCTATTTAATGAGGGCCCCGAACACGGGGAGTCTTACACGATCTGCCCTGGAATCGC
 *
 * Your program should accept a path to a filename as its first argument. Each line in the file contains a segment of DNA, the maximum number of allowed mismatches N, and a DNA string separated by a single space.
 * 
 * Output sample:
 * CCC CCA CCG CGC GCC TCC
 * GCAAG GCAAG GCCAG GCGCG GCGCA GCTAA
 * No match
 *
 * Print out all the occurrences of a segment S in a DNA string in the order from the best match (separated by a single space) taking into account the number of allowed mismatches. In case of several segments with the equal number of matches, print them in alphabetical order. If there is no such a case, print out 'No match'.
 *
 * Constraints:
 *
 *   The length of a DNA string is in a range from 100 to 300.
 *   N is in a range from 0 to 40.
 *   The length of a segment S is in a range from 3 to 50.
 *
 * */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
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
// required when TEST is defined
template<typename V> using vector = std::vector<V>;

void process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  string pattern;
  string data;
  int max_errors;
  {
    std::istringstream ss { std::move(line) };
    ss >> pattern >> max_errors >> data;
    ++max_errors;
  }

  vector<vector<string>> rv (max_errors + 1);
  for(int ofs = 0; ofs + pattern.length() != data.length(); ++ofs) {
    int ec = 0;
    for(int i = 0; ec != max_errors && i != pattern.length(); ++i) {
      if(pattern[i] != data[i + ofs]) {
        ++ec;
      }
    }
    if(ec != max_errors) {
      rv[ec].emplace_back(data.substr(ofs, pattern.length()));
    }
  }

  bool mid = false;
  for(auto & v : rv) {
    std::sort(v.begin(), v.end());
    for(auto & s : v) {
      std::cout << (mid ? " " : "") << s;
      mid = true;
    }
  }
  std::cout << (mid ? "\n" : "No match.\n");
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "CCC 1 CGCCCGAATCCAG",
    "GCGAG 2 CCACGGCCTATGTATTTGCAAGGATCTGGGCCAGCTAAATCAGCACCCCTGGAACGGCAAGGTTCATTTTGTTGCGCGCATAG",
    "CGGCGCC 1 ACCCCCGCAGCCATATGTCCCCAGCTATTTAATGAGGGCCCCGAACACGGGGAGTCTTACACGATCTGCCCTGGAATCGC",
  };
  vector<string> v_expect {
    "CCC CCA CCG CGC GCC TCC",
    "GCAAG GCAAG GCCAG GCGCG GCGCA GCTAA",
    "No match",
  };
  for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
    process(v_test[i]);
    std::cout << v_expect[i] << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char* path) {
  std::ifstream stream(path);
  for(string line; std::getline(stream, line); ) {
    process(line);
  }
}
