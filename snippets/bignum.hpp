#pragma once

#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;
using num_t = long long;
using bignum = vector<int>;

const num_t BASE = 1000000000ll;

bignum operator + (const bignum & a, const bignum & b);
std::ostream & operator << (std::ostream & stream, const bignum & num);
bignum operator * (const bignum & n, const num_t c);
bignum operator * (const bignum & a, const bignum & b);

bignum operator + (const bignum & a, const bignum & b) {
  bignum res;
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

std::ostream & operator << (std::ostream & stream, const bignum & num) {
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
    }
    mid = true;
    std::cout << *it;
  }
  return stream;
#endif//else @ #if 0
}

bignum operator * (const bignum & n, const num_t c) {
  throw 1;
  return bignum();
}

bignum operator * (const bignum & a, const bignum & b) {
  throw 1;
  return bignum();
}

