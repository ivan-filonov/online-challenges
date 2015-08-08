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
