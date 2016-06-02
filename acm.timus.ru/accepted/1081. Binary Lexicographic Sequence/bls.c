#include <stdio.h>

int main() {
  int len;
  int num;
#ifdef ONLINE_JUDGE || 1
  scanf("%d%d", &len, &num);
#else
  len = 4;
  num = 8;
#endif

  int fib[45] = {
      1, 1,
  };
  for (int i = 2; i < 45; ++i) {
    fib[i] = fib[i - 1] + fib[i - 2];
  }
  if (num > (fib[len] + fib[len - 1])) {
    printf("-1\n");
  } else {

    for (int i = len, prev = 0; i > 0; --i) {
      const int c0 = fib[i];
      const int c1 = fib[i ? i : 0];
      if (num > c0 && !prev) {
        prev = 1;
        num -= c0;
      } else {
        prev = 0;
        if (num > c1)
          num -= c1;
      }
      printf("%d", prev);
    }
    printf("\n");
  }

  return 0;
}
