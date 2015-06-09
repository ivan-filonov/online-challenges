#include <cmath>
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

/*
    Pi        Pi number
    e         Euler's number
    sqrt()    Square root
    cos()     Cosine (using radians as an argument)
    sin()     Sine (using radians as an argument)
    tan()     Tangent (using radians as an argument)
    lg()      Decimal logarithm
    ln()      Natural logarithm
1   ()        Brackets
2   ||        Absolute value, e.g. |-5 - 10|
3   !         Factorial
4   -         Unary minus
5   ^         Exponent
6   mod       Modulus divide, e.g. 5 mod 2 = 1 (only integers will be supplied here)
7   *, /      Multiply, Divide (left-to-right precedence)
8   +, -      Add, Subtract (left-to-right precedence)

Constraints:
Each number in input expression is greater than -20,000 and less than 20,000.
Each output number is greater than -20,000 and less than 20,000.
If output number is a float number it should be rounded to the 5th digit after the dot.
E.g 14.132646 gets 14.13265, 14.132644 gets 14.13264, 14.132645 gets 14.13265.

If output number has less than 5 digits after the dot you don't need to add zeros.
E.g. you need to print 16.34 (and not 16.34000) in case the answer is 16.34.
And you need to print 16 (and not 16.00000) in case the answer is 16.
 * */
using num_t = long double;

using std::string;
template<typename V> using vector = std::vector<V>;

struct tok {
  string s;
  num_t v;
  bool num;
};

const string ops { "()|!-^*/+" };

num_t expr (vector<tok> & v, size_t * i);
num_t expr1 (vector<tok> & v, size_t * i);
num_t expr2 (vector<tok> & v, size_t * i);
num_t expr3 (vector<tok> & v, size_t * i);
num_t expr4 (vector<tok> & v, size_t * i);
num_t expr5 (vector<tok> & v, size_t * i);
num_t expr6 (vector<tok> & v, size_t * i);
num_t expr7 (vector<tok> & v, size_t * i);

void expect(vector<tok> & v, size_t * i, const string &t);
bool check(vector<tok> & v, size_t * i, const string &t);

// ... '+'|'-' ...
num_t expr (vector<tok> & v, size_t * i) {
  auto l = expr1(v,i);
  while(v.size() != *i) {
    if(check(v,i,"-")) {
      auto r = expr1(v,i);
//      std::cout << "expr(i=" << *i <<"): " << l << " - " << r << " -> " << (l-r) << "\n";
      l -= r;
    } else if(check(v,i,"+")) {
      auto r = expr1(v,i);
//      std::cout << "expr(i=" << *i <<"): " << l << " + " << r << " -> " << (l+r) << "\n";
      l += r;
    } else break;
  }
  return l;
}

// ... '*'|'/' ...
num_t expr1 (vector<tok> & v, size_t * i) {
  auto l = expr2(v,i);
  while(v.size() != *i) {
    if(check(v,i,"*")) {
      auto r = expr2(v,i);
//      std::cout << "expr1(i=" << *i <<"): " << l << " * " << r << " -> " << (l*r) << "\n";
      l *= r;
    } else if(check(v,i,"/")) {
      auto r = expr2(v,i);
//      std::cout << "expr1(i=" << *i <<"): " << l << " / " << r << " -> " << (l/r) << "\n";
      l /= r;
    } else break;
  }
  return l;
}

// ... 'mod' ...
num_t expr2 (vector<tok> & v, size_t * i) {
  auto l = expr3(v,i);
  if(check(v,i,"mod")) {
    auto r = expr3(v,i);
//    std::cout << "expr2(i=" << *i <<"): " << l << " mod " << r << " -> " << ((int) l % (int) r) << "\n";
    l = (int) l % (int) r;
  }
  return l;
}

// ... '^' ...
num_t expr3 (vector<tok> & v, size_t * i) {
  auto l = expr4(v,i);
  if(check(v,i,"^")) {
    auto e = expr4(v,i);
//    std::cout << "expr3(i=" << *i <<"): " << l << " ^ " << e << " -> " << std::pow(l, e) << "\n";
    l = std::pow(l, e);
  }
  return l;
}

// '-' ...
num_t expr4 (vector<tok> & v, size_t * i) {
  num_t sign = 1;
  if(check(v,i,"-")) {
    sign = -1;
  }
  return expr5(v,i) * sign;
}

// ... '!'
num_t expr5 (vector<tok> & v, size_t * i) {
  auto r = expr6(v,i);
  if(check(v,i,"!")) {
//    std::cout << "expr5(i=" << *i <<"): factorial " << r;
    for(int t = r - 1; t; --t) {
      r *= t;
    }
//    std::cout << " -> " << r << "\n";
  }
  return r;
}

// '|' ... '|'
num_t expr6 (vector<tok> & v, size_t * i) {
  num_t r;
  if(check(v,i,"|")) {
    r = std::fabs(expr(v,i));
    expect(v,i,"|");
  } else {
    r = expr7(v,i);
  }
  return r;
}

// ident || num || function '(' ... ')' || '(' ... ')'
num_t expr7 (vector<tok> & v, size_t * i) {
  num_t r;
  if(v[*i].num) {
    r = v[*i].v;
    ++*i;
  } else if(check(v,i,"(")) {
    r = expr(v,i);
    expect(v,i,")");
//    std::cout << "expr7(i=" << *i <<") : ( " << r << " )\n";
  } else if(std::isalpha(v[*i].s.front())) {
    auto & f = v[*i].s;
    ++*i;
    expect(v,i,"(");
    auto e = expr(v,i);
    expect(v,i,")");
    if(f == "sqrt") {
      r = std::sqrt(e);
    } else if(f == "cos") {
      r = std::cos(e);
    } else if(f == "sin") {
      r = std::sin(e);
    } else if(f == "tan") {
      r = std::tan(e);
    } else if(f == "lg") {
      r = std::log10(e);
    } else if(f == "ln") {
      r = std::log(e);
    } else r = e; 
//    std::cout << "expr7(i=" << *i <<") : " << f << " ( " << e << " ) -> " << r << "\n";
  } else {
    std::cout << "DIE: '" << v[*i].s << "'\n";
    throw v[*i];
  }
  return r;
}

void process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  vector<tok> vt;
  size_t pos = 0;
  while(line.length() != pos) {
    while(line.length() != pos && std::isspace(line[pos])) {
      ++pos;
    }
    if(line.length() == pos) {
      break;
    }
    vt.emplace_back();
    auto & b = vt.back();
    b.num = false;
    const auto c = line[pos];
    if('.' == c || std::isdigit(c)) {
      // numeric literal
      size_t o;
      b.num = true;
      b.v = std::stold(line.substr(pos), &o);
      b.s = line.substr(pos, o);
      pos += o;
    } else if(std::isalpha(c)) {
      // identifier
      size_t o = pos;
      while(line.length() != o && std::isalnum(line[o])) {
        ++o;
      }
      b.s = line.substr(pos, o - pos);
      pos = o;
      if(b.s == "e") {
        b.num = true;
        b.v = std::exp(1.0l);
      } else if(b.s == "Pi") {
        b.num = true;
        b.v = M_PI;
      }
    } else if(string::npos != ops.find(c)) {
      // single-character operator
      b.s = c;
      ++pos;
    } else throw pos;
  }
  pos = 0;
  auto t = expr(vt, &pos);
  vector<char> buf(512);
  snprintf(buf.data(), buf.size(), "%.5Lf", t);
  string ts { buf.data() };
  if(string::npos != ts.find('.')) {
    while(ts.back() == '0') {
      ts.pop_back();
    }
    if('.' == ts.back()) {
      ts.pop_back();
    }
  }
  std::cout << ts << "\n";
}

//#define VERBOSE

void expect(vector<tok> & v, size_t * i, const string &t) {
  if(v.size() == *i) {
#ifdef VERBOSE
    std::cout << "failed expect(t='" << t << "') - end of line\n";
#endif//#ifdef VERBOSE
    throw t;
  }
  if(v[*i].s != t) {
#ifdef VERBOSE
    std::cout << "failed expect(i=" << *i << ", t='" << t << "'), instead got '" << v[*i].s << "'\n";
#endif//#ifdef VERBOSE
    throw t;
  }
#ifdef VERBOSE
  std::cout << "OK expect for '" << t << "' at " << *i << "\n";
#endif//#ifdef VERBOSE
  ++*i;
}

bool check(vector<tok> & v, size_t * i, const string &t) {
  if(v.size() != *i && v[*i].s == t) {
#ifdef VERBOSE
    std::cout << "OK check for '" << t << "' at " << *i << "\n";
#endif//#ifdef VERBOSE
    ++*i;
    return true;
  }
  return false;
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "e / Pi",
    "908.39 - e",
    "190 + e",
    "684.71 - 770",
    "811 - e / 970",
    "e - e",
    "|e / (-981)|",
    "959 * 882.83 / e / (-204)",
    "Pi + (-185.91)",
    "(((12-43.2) + 823.592)*(23.34/2))-2^1",
    "(-486) / |(-(103 / (-108))) * 55 * (-162) - (-(733.64 / (-311.14) + Pi))|",
    "396 + (-449.17) / |140.13 - (-545)| / 30.23",
    "(-732) / 411.53 + 88.27",
    "e * (-69) / (-879.18)",
    "Pi + 696",
    "620 + 803 / (-509) + 664 - 419",
    "998 / (-(971.74 - Pi))",
    "sin(sqrt(5!))*lg(21)*Pi/e",
    "||2.24 / (-99)| + 29|",
    "(-658.74) * (-21) / 300.66 + (-52) / Pi + 96.21",
    "e * 874 - e + (-166.30) / |(-754.84) - 148|",
    "(-242) + (-306.22) / 70.90",
    "566 - 534.12",
    "(-913.31) + 93 - 387 + |(-951) - Pi|",
    "|986 mod (-1) * 164 mod 476 + 224.54|",
    "(-(538 * e))",
    "-(0.048-0.047)^-1",
    "(-695.65) - Pi * 46.00",
    "(-50) / (-540) / Pi + (-356.91)",
    "(cos((228.87 / 456.08)) + cos(858.76 - e / (-272))) + (984 / (-813) / 659.05 / (-367.60) / 588.88)",
    "719 * (-570.61) / 754.82 / 645 - e * 659",
    "(-(Pi + (-589.36) + 613))",
    "(-409) - |(-543.25) + 327.57|",
    "(-735.13) / (-102) / Pi",
    "709 - 486.19 - Pi",
    "(-(Pi * Pi)) / 603 + 982",
    "(((12-43.2) + 823.592)*(23.34/2))-2^-2",
    "|(-97.48) / Pi + 638.19|",
    "e + (-26.24) * e",
    "(-(Pi * e))",
    "e - Pi",
    "(-(687 - (-182))) + (-442.35) - (-731.77)",
    "(-377.54) + |742.19 - 895.75| / 717 / 997",
    "sin(cos(tan(lg(ln(13)))))",
    "|(-126.52) - e / Pi + (-294) + Pi|",
    "(-835.47) / 612.72 - (-621) + 944.48 + Pi",
    "|Pi - 240.02 + (-319) * e / (-697.04)|",
    "357.69 / Pi",
    "(-((-50) * |Pi - |(-243) + 407||))",
    "249 + e - (-790) - |(-((-764) + 274)) / 292.51 * 523.56|",
  };
  vector<string> v_expect {
    "0.86525",
    "905.67171",
    "192.71828",
    "-85.29000",
    "810.99719",
    "0",
    "0.00277",
    "-1526.76094",
    "-182.7684",
    "9245.21464",
    "-0.05719",
    "395.97831",
    "86.49127",
    "0.21333",
    "699.14159",
    "863.42239",
    "-1.03035",
    "-1.52683",
    "29.02262",
    "125.66846",
    "2372.87584",
    "-246.31904",
    "31.88",
    "-253.1684",
    "224.54",
    "-1462.43562",
    "-1000",
    "-840.16326",
    "-356.88052",
    "0.43671",
    "-1792.19041",
    "-26.78159",
    "-624.68",
    "2.2941",
    "219.6684",
    "981.98363",
    "9246.96464",
    "607.16115",
    "-68.60943",
    "-8.53973",
    "-0.42331",
    "-579.58",
    "-377.53978",
    "0.78795",
    "418.24366",
    "1567.25805",
    "235.63438",
    "113.85626",
    "8042.92036",
    "164.67339",
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
