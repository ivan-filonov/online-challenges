/*
 * We can receive a lucky ticket in a public transport. How to reveal whether 
 * the ticket is lucky or not? We call a ticket lucky if the sum of its digits 
 * in the first half equals to the sum of digits in the second half. For 
 * example, ticket 328940 is a lucky one because 3+2+8=9+4+0.
 * Write a program that will count the maximum number of lucky tickets 
 * depending on the length of the ticket number. In other words, how many 
 * lucky combinations can be if a ticket number comprises 4, 6, 8, … digits ?
 *
 *
 * 1. The length of a ticket number can be from 2 to 100 digits.
 * 2. Tickets 000000 and 999999 should be also counted.
 * 3. All the input numbers are even.
 * 4. The number of test cases is 40.
 *
 */
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
// required when TEST is defined
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
	bigint res;
	if(a.size() < b.size()) {
		return b * a;
	}
	bigint cur = a;
	for(int i = 0; i != b.size(); ++i) {
		if(i) {
			cur.insert(cur.begin(), 0);
		}
		if(b[i]) {
			res = res + cur * b[i];
		}
	}
	return res;
}
//-----------------------------------------------bignum.hpp-------------------------------

void process(string line) {
#ifdef TEST
	std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
	const int total_digits = std::stoi(line);
	const int cnt_to_chk = total_digits/2;// *[3 - all input numbers are even]*

	using res_type = bigint;
	vector<res_type> current;
	vector<res_type> next;
	current.assign(10, res_type({1}));
	for(int cnt = 1; cnt < cnt_to_chk; ++cnt) {
		next.resize(cnt * 9 + 10);
		for(int i = 0; i < next.size(); ++i) {
			res_type sum({0});
			for(int l = 0; l != 10 && l != i + 1; ++l) {
				if(i - l < current.size()) {
					sum = sum + current[i - l];
				}
			}
			next[i] = sum;
		}
		current.swap(next);
	}
	res_type sum (0);
	for(auto val : current) {
		sum = sum + val * val;
	}
	std::cout << sum << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
	"2",
	"4",
	"6",
	"8",
	"40",
  };
  vector<string> v_expect {
	"10",
	"670",
	"55252",
	"4816030",
	"?",
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
