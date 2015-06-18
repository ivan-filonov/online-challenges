/*
  Palindromic Ranges

  Challenge Description:

  A positive integer is a palindrome if its decimal representation (without leading zeros) is a palindromic string (a string that reads the same forwards and backwards). For example, the numbers 5, 77, 363, 4884, 11111, 12121 and 349943 are palindromes.

  A range of integers is interesting if it contains an even number of palindromes. The range [L, R], with L <= R, is defined as the sequence of integers from L to R (inclusive): (L, L+1, L+2, ..., R-1, R). L and R are the range's first and last numbers.

  The range [L1,R1] is a subrange of [L,R] if L <= L1 <= R1 <= R. Your job is to determine how many interesting subranges of [L,R] there are.
  Input sample:

  Your program should accept as its first argument a path to a filename. Each line in this file is one test case. Each test case will contain two positive integers, L and R (in that order), separated by a space. eg.

  1 2
  1 7
  87 88

  Output sample:

  For each line of input, print out the number of interesting subranges of [L,R] eg.

  1
  12
  1

  For the curious: In the third example, the subranges are: [87](0 palindromes), [87,88](1 palindrome),[88](1 palindrome). Hence the number of interesting palindromic ranges is 1 
*/
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
  int L, R;
  {
    std::istringstream ss { line };
    ss >> L >> R;
  }

  vector<int> p;
  {
    int ndigits = 0;
    for(int i = L; i; i /= 10) {
      ++ndigits;
    }

    // palindrome generator state:
    int pal_num;//left part
    int pal_mid;//mid digit
    int pal_max;//upper limit of left part (not inclusive)
    
    pal_num = L;
    pal_max = 1;
    for(int i = 0; i < ndigits/2; ++i) {
      pal_num /= 10;
      pal_max *= 10;
    }
    if(ndigits % 2) {
      pal_mid = pal_num % 10;
      pal_num /= 10;
    }

    int cur_pal;
    do {
      // generate new palindrome value
      cur_pal = pal_num;
      if(ndigits % 2) {
        cur_pal = cur_pal * 10 + pal_mid;
      }
      for(int i = 0, t = pal_num; i < ndigits/2; ++i, t /= 10) {
        cur_pal = cur_pal * 10 + t % 10;
      }

      // increment generator state
      if(ndigits % 2 && pal_mid < 9) {
        ++pal_mid;
      } else {
        ++pal_num;
        pal_mid = 0;
        if(pal_num == pal_max) {
          if(ndigits % 2) {
            pal_max *= 10;
          } else {
            pal_num = pal_max / 10;
          }
          ndigits++;
        }
      }

      if(cur_pal >= L && cur_pal <= R) {
        p.push_back(cur_pal);
      }
    } while(cur_pal <= R);
  }

  if(L==R) {
    std::cout << (1 - p.size()) << "\n";
    return;
  }

  auto get_left = [&L, &p](int i) { return i ? p[i-1] : (L - 1); };
  auto get_right = [&R, &p](int i) { return p.size() == i ? R + 1 : p[i]; };

  int result = 0;

  // посчитать все последовательности с 0 палиндромами внутре
  for(int r = 0; r <= p.size(); ++r) {
    int k = get_right(r) - get_left(r) - 1;
    result += k * (k + 1) / 2;
  }

  // последовательности с 2*k палиндромами внутре
  for(int skip = 2; skip <= p.size(); skip += 2) {
    for(int r0 = 0, r1 = skip; r1 <= p.size(); ++r0, ++r1) {
      int c0 = get_right(r0) - get_left(r0);
      int c1 = get_right(r1) - get_left(r1);
      result += c0 * c1;
    }
  }

  std::cout << result << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "1 2",
    "1 7",
    "87 88",
    "102 2234",
  };
  vector<string> v_expect {
    "1",
    "12",
    "1",
    "хер знает",
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
