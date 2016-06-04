#include <stdio.h>
#include <stdlib.h>
int main() {
#ifdef ONLINE_JUDGE
#else
  "4 0 4 4 2 6 8 3 7";
#endif
  const char *d = "0123456789QWERTY";
  unsigned int N, i, a, b, c;
  unsigned char *v;
  char buf[4096 * 4];
  unsigned int bp, bs;

  scanf("%d", &N);
  v = calloc(1, N + 1);

  bp = bs = 0;
  for (i = 1; i <= N; ++i) {
    for (;;) {
      if (bs == bp)
        bs = fread(buf, 1, sizeof(buf), stdin), bp = 0;
      if ((a = buf[bp++]) >= '0')
        break;
    }
    for (;;) {
      if (bs == bp)
        bs = fread(buf, 1, sizeof(buf), stdin), bp = 0;
      if ((b = buf[bp++]) >= '0')
        break;
    }
    v[i] = (a - '0') + (b - '0');
  }

  c = 0;
  for (i = 0; i <= N; ++i) {
    b = v[N - i] + c;
    c = b / 10;
    v[N - i] = b % 10;
  }

  // a = 0;
  for (i = *v ? 0 : 1; i <= N; ++i) {
    //  a |= v[i];
    //  if (!a) {
    //    continue;
    //  }
    fwrite(d + v[i], 1, 1, stdout);
  }
  free(v);

  printf("\n");

  return 0;
}
