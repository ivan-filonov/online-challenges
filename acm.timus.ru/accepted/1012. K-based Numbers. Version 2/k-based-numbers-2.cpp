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

using std::string;
template<typename V> using vector = std::vector<V>;

//-----------------------------------------------bignum.hpp-------------------------------
using num_t = long long;
using bigint = vector<int>;

const num_t BASE = 1000000000ll;

bigint operator + (const bigint & a, const bigint & b);
std::ostream & operator << (std::ostream & stream, const bigint & num);
bigint operator * (const bigint & n, const num_t c);
bigint operator * (const bigint & a, const bigint & b);

bigint operator + (const bigint & a, const bigint & b) {
  bigint res;
  num_t carry = 0;

  auto it_a = a.begin();
  auto e_a = a.end();
  auto it_b = b.begin();
  auto e_b = b.end();

  for(;it_a != e_a && it_b != e_b; ++it_a, ++it_b) {
    num_t _a = *it_a;
    num_t _b = *it_b;
    num_t sum = _a + _b + carry;

    carry = sum / BASE;
    sum = sum % BASE;

    res.push_back((int)sum);
  }

  decltype(it_a) it, e;
  if(it_a == e_a) {
    it = it_b;
    e = e_b;
  } else {
    it = it_a;
    e = e_a;
  }

  for(; it != e; ++it) {
    num_t _a = *it;
    num_t sum = _a + carry;

    carry = sum / BASE;
    sum = sum % BASE;

    res.push_back((int)sum);
  }

  if(carry) {
    res.push_back((int)carry);
  }

  return res;
}

std::ostream & operator << (std::ostream & stream, const bigint & num) {
  bool mid = false;
#if 0
  for(auto i : num) {
    std::cout << (mid?",":"") << i;
    mid = true;
  }
  return stream;
#else//#if 0
  for(auto it = num.rbegin(), e = num.rend(); it != e; ++it) {
    if(mid) {
      for(num_t p = BASE / 10; p > *it && p > 1; p /= 10) {
        std::cout << '0';
      }
    } else {
      if(!*it) {
        continue;
      }
    }
    mid = true;
    std::cout << *it;
  }
  if(!mid) {
    std::cout << '0';
  }
  return stream;
#endif//else @ #if 0
}

bigint operator * (const bigint & n, const num_t c) {
  if(c >= BASE || c <= 0) {
    throw c;
  }
  bigint res;
  num_t carry = 0;

  for(int i : n) {
    num_t _i = i;
    num_t r = _i * c + carry;
    carry = r / BASE;
    res.push_back((int)(r % BASE));
  }

  if(carry) {
    res.push_back((int)carry);
  }

  return res;
}

bigint operator * (const bigint & a, const bigint & b) {
  throw 1;
  return bigint();
}
//-----------------------------------------------bignum.hpp-------------------------------

int main() {
  std::cin.sync_with_stdio(false);

  num_t length, base;
  std::cin >> length >> base;

  bigint prev2 { (int)(base - 1) };
  bigint prev { (int)((base - 1) * base % BASE), (int)((base - 1) * base / BASE) };

  while(length-->2) {
    bigint s = prev + prev2;
    prev2 = prev;
    prev = s * (base - 1);
  }

  std::cout << prev;

  return 0;
}
