#include <stdio.h>

int scount(const char * buf, const char * ss) {
  int res = 0;
  for(const char * start = buf, *ptr = buf, *ptr2; *ptr; ++start) {
    ptr = start;
    ptr2 = ss; 
    while( *ptr && *ptr2 && *ptr == *ptr2) {
      ++ptr;
      ++ptr2;
    }
    if(!*ptr2) {
      ++res;
    }
  }
  return res;
}

int main() {
  char substring_to_count[26];
  int N = 0;
  scanf("%d\n%s", &N, substring_to_count);

  char buf[90] = {0, };
  int l1 = 1;
  int l2 = 1;

  long long res = 0;
  if(1 == N) {
    res = scount("A", substring_to_count);
  } else if(2 == N) {
    res = scount("B", substring_to_count);
  } else {
    long long count[11] = {
      0,//step = 0
      scount("A", substring_to_count),
      scount("B", substring_to_count),
      };
    buf[0] = 'B';
    buf[1] = 'A';
    l2 = 2;
    for(int step = 3; step < 11 && step <= N; ++step) {
      count[step] = scount(buf, substring_to_count);
      for(int ofs = 0; ofs != l1; ++ofs) {
        buf[l2 + ofs] = buf[ofs];
      }
      int new_len = l1 + l2;
      l1 = l2;
      l2 = new_len;
    }
    buf[l2] = 0;
    if(N < 11) {
      res = count[N];
    } else {
      long long d[2] = {
        count[9] - count[8] - count[7],
        count[10] - count[9] - count[8],
      };
      int c1 = count[9];
      int c2 = count[10];
      for(int step = 10; step < N; ++step) {
        int t = c1 + c2 + d[step % 2];
        c1 = c2;
        c2 = t;
      }
      res = c2;
    }
  }
  printf("%lld\n", res);
}
