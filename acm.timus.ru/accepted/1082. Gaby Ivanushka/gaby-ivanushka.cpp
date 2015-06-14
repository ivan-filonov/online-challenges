#include <algorithm>
#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

long c;
vector<long> A;

long P(long l, long r)//partition, pivot - [l], return value: items in prefix are less that pivot and items in suffix are greater than pivot
{
  long x=A[l],//pivot
        i=l-1,
        j=r+1,
        t;
  while(1)
  {
    do{--j; ++c;}
    while(A[j]>x);// find right item in A that is <= x, add suffix len to c

    do{++i; ++c;}
    while(A[i]<x);// find left item in A that is >= x, add prefix len to c

    if(i<j)//if left < right - swap them, now A[i] <= A[j] 
    {
      t=A[i];
      A[i]=A[j];
      A[j]=t;
    }
    else return j;
  }
}

void Q(long l, long r)//quick sort
{
  long n;
  if(l<r)
  {
    n=P(l,r);
    Q(l,n);
    Q(n+1,r);
  }
}

int main() {
  std::cin.sync_with_stdio(false);
  int n;
  std::cin >> n;
  vector<int> v;
  v.resize(n);
  for(int i = 0; i !=n; ++i) {
    v[i] = i + 1;
  }
  A.resize(n);
  for(;;) {
    c = 0;
    std::copy(v.begin(), v.end(), A.begin());
    Q(0, n-1);
    if(c == (n*n+3*n-4)/2) {
      break;
    }
    std::next_permutation(v.begin(), v.end());
  }
  bool mid = false;
  for(auto i : v) {
    std::cout << (mid ? " " : "") << i;
    mid = true;
  }
  std::cout << "\n";
  return 0;
}

