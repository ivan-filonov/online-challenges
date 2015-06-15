/* 
  http://acm.timus.ru/problem.aspx?space=1&num=1009

  Let’s consider K-based numbers, containing exactly N digits. We define a number to be valid if its K-based notation doesn’t contain two successive zeros. For example:

      1010230 is a valid 7-digit number;
      1000198 is not a valid number;
      0001235 is not a 7-digit number, it is a 4-digit number. 

  Given two numbers N and K, you are to calculate an amount of valid K based numbers, containing N digits.
  You may assume that 2 ≤ K ≤ 10; N ≥ 2; N + K ≤ 18.

  Input
  The numbers N and K in decimal notation separated by the line break.

  Output
  The result in decimal notation.

  Sample:
  input
  2
  10
  output
  90
 */
#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;
using num_t = long long;

num_t ipow(num_t val, int pow) {
  num_t result = 1;
  while(pow-->0) {
    result *= val;
  }
  return result;
}

int main() {
  std::cin.sync_with_stdio(false);

  num_t base, length;
  std::cin >> length >> base;

  num_t result = 0;//first digit != 0

  for(num_t i = 0, max_i = 1 << (length - 1); i < max_i; ++i) {
    num_t num_set = 0, prev = 1, fail = 0;
    for(num_t bit = 1; bit < max_i && !fail; bit <<= 1) {
      num_t cur = (bit & i);
      if(!cur && !prev) {
        fail = 1;
      }
      if(cur) {
        ++num_set;
      }
      prev = cur;
    }
    if(!fail) {
      result += ipow(base - 1, num_set);
    }
  }

  result *= base - 1;

  std::cout << result;

  return 0;
}
