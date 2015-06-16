/*
  http://acm.timus.ru/problem.aspx?space=1&num=1012

  1012. K-based Numbers. Version 2
  Time limit: 0.5 second
  Memory limit: 16 MB
  Let’s consider K-based numbers, containing exactly N digits. We define a number to be valid if its K-based notation doesn’t contain two successive zeros. For example:

      1010230 is a valid 7-digit number;
      1000198 is not a valid number;
      0001235 is not a 7-digit number, it is a 4-digit number. 

  Given two numbers N and K, you are to calculate an amount of valid K based numbers, containing N digits.
  You may assume that 2 ≤ K ≤ 10; N ≥ 2; N + K ≤ 1800.
  Input
  The numbers N and K in decimal notation separated by the line break.
  Output
  The result in decimal notation.

  Sample
  input 
  2
  10
  output
  90
*/
/*
 0         1        10         0
 1        10         9         0
 2        90         9        81
 3       891         9       810
 4      8829         9      8019
 5     87480         9     79461
 6    866781         9    787320
 7   8588349         9   7801029
*/
#include <iostream>
#include <vector>

//TODO: read ../snippets/bignum.hpp
#include "../snippets/bignum.hpp"

using std::string;
template<typename V> using vector = std::vector<V>;

int main() {
  std::cin.sync_with_stdio(false);

  num_t length, base;
  std::cin >> length >> base;


  return 0;
}
