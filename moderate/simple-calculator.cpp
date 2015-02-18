/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <stdio.h>

void test();
void process_file(char*);
int main(int _a, char ** _v) {
//	test();
	process_file(_v[1]);
	return 0;
}

void process(std::string s);

void test() {
	process("250*14.3");
	std::cout << "3575"  "\n";// std::endl for flush?
	process("3^6 / 117");
	std::cout << "6.23077"  "\n";// std::endl for flush?
	process("(2.16 - 48.34)^-1");
	std::cout << "−0.02165"  "\n";// std::endl for flush?
	process("(59 - 15 + 3*6)/21");
	std::cout << "2.95238"  "\n";// std::endl for flush?
	process("(59 - 15 + 3*6)/2.1");
	process("600^500/600^501");
}

void process_file(char* path) {
	std::ifstream stream(path);
	std::string line;
	while(std::getline(stream, line)) {
		process(line);
	}
}

typedef std::vector<std::string>::iterator _iter_t;
typedef long double num_t;

num_t expr(_iter_t& it, const _iter_t end);
num_t expr1(_iter_t& it, const _iter_t end);
num_t expr2(_iter_t& it, const _iter_t end);
num_t expr3(_iter_t& it, const _iter_t end);
num_t expr4(_iter_t& it, const _iter_t end);

num_t expr(_iter_t& it, const _iter_t end) {
  num_t l = expr1(it, end);
  while(end != it) {
    const auto add = *it == "+";
    const auto sub = *it == "-";
    if(add || sub) {
      ++it;
      auto r = expr1(it, end);
      l = add ? (l + r) : (l - r);
    } else {
      break;
    }
  }
  return l;
}

num_t expr1(_iter_t& it, const _iter_t end) {
  num_t l = expr2(it, end);
  while(end != it) {
    const auto mul = *it == "*";
    const auto div = *it == "/";
    if(mul || div) {
      ++it;
      auto r = expr2(it, end);
      l = mul ? (l * r) : (l / r);
    } else {
      break;
    }
  }
  return l;
}

num_t expr2(_iter_t& it, const _iter_t end) {
  num_t l = expr3(it, end);
  while(end != it) {
    if(*it == "^") {
      ++it;
      l = std::pow(l, expr3(it, end));
    } else {
      break;
    }
  }
  return l;
}

num_t expr3(_iter_t& it, const _iter_t end) {
    if(*it == "-") {
      ++it;
      return -expr4(it, end);
    }
  return expr4(it, end);
}

num_t expr4(_iter_t& it, const _iter_t end) {
  auto c = (*it)[0];
  if(std::isdigit(c) || ('.' == c)) {
    auto s = *it++;
    return std::stod(s);
  } else if(*it == "(") {
    ++it;
    auto r = expr(it, end);
    if(*it == ")") {
      ++it;
    } else {
      throw *it;
    }
    return r;
  }
}

void process(std::string s) {
	// 1. tokenize string
	std::vector<std::string> tokens;
	size_t pos = 0;
  std::vector<char> ops {'(',')','+','-','*','/','^'};
	while(pos < s.length()) {
		if(std::isspace(s[pos])) {
      ++pos;
		} else if(std::count(std::begin(ops), std::end(ops), s[pos]) > 0) {
      tokens.push_back(std::string(std::begin(s) + pos, std::begin(s) + pos + 1));
      ++pos;
    } else if(std::isdigit(s[pos]) || ('.' == s[pos])) {
      size_t incr;
      tokens.push_back(std::to_string(std::stold(s.substr(pos), &incr)));
      pos += incr;
    } else {
      throw 1;
    }
	}
  // 2. evaluate
  auto it = std::begin(tokens);
  num_t val = expr(it, std::end(tokens));
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(5) << val;
   auto sval =  oss.str();
   if(std::string::npos != sval.find('.')) {
     while('0' == sval.back()) {
       sval.pop_back();
     }
   }
   if('.' == sval.back()) {
     sval.pop_back();
  }
  std::cout << sval << "\n";
}
