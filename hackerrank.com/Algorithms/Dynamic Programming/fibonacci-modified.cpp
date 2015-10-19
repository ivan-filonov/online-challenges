#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

void sum(vector<int> &v, const vector<int> & a, const vector<int> & b) {
  v.reserve(max(a.size(), b.size()));
  v.clear();
  int carry = 0, i, j;
  for(i = 0, j = min(a.size(), b.size()); i < j; ++i) {
    int t = carry + a[i] + b[i];
    v.push_back(t % 10000);
    carry = t / 10000;
  }
  if(a.size() > b.size()) {
    for(j = a.size(); i < j; ++i) {
      int t = carry + a[i];
      v.push_back(t % 10000);
      carry = t / 10000;
    }
  } else if( b.size() > a.size()) {
    for(j = b.size(); i < j; ++i) {
      int t = carry + b[i];
      v.push_back(t % 10000);
      carry = t / 10000;
    }
  }
  if(carry) {
    v.push_back(carry);
  }
}

void print(const vector<int> &v) {
  bool mid = false;
  for(auto it = v.crbegin(), e = v.crend(); it != e; ++it) {
    if(mid) {
      if(*it < 1000) cout << '0';
      if(*it < 100) cout << '0';
      if(*it < 10) cout << '0';
      cout << *it;
    } else {
      if(!*it) {
        continue;
      }
      cout << *it;
      mid = true;
    }
  }
  if(!mid) {
    cout << '0';
  }
}

void imul(vector<int> &r, const vector<int> & v, int i) {
  r.reserve(v.size() + 1);
  r.clear();
  int carry = 0;
  for(int vv : v) {
    int t = carry + vv * i;
    r.push_back(t % 10000);
    carry = t / 10000;
  }
  if(carry) {
    r.push_back(carry);
  }
}

void sqr(vector<int> &r, const vector<int> &v) {
  vector<int> vs(v), t, t1;
  for(int i : v) {
    imul(t, vs, i);
    sum(t1, r, t);
    t1.swap(r);
    vs.insert(vs.begin(), 0);
  }
}

int main() {
  int a, b, N;
  cin >> a >> b >> N;
  vector<int> A(1), B(1), t, t1;
  A[0] = a;
  B[0] = b;
  while(N-->2) {
    sqr(t, B);
    sum(t1, t, A);
    A.swap(B);
    B.swap(t1);
  }
  print(B);

  return 0;
}

