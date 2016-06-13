#include <stdio.h>
#if ONLINE_JUDGE
static int get_int() {
  int r;
  scanf("%d\n", &r);
  return r;
}
static char get_peng() {
  char buf[32];
  fgets(buf, sizeof(buf), stdin);
  return buf[0];
}
#else
static int get_int() { return 7; }
static char _c[] = {"EMLEMML"};
static int _ci = 0;
static char get_peng() { return _c[_ci++]; }
#endif
int main() {
  int n = get_int(), i, p[3] = {
                            0,
                        };
  for (i = 0; i < n; ++i) {
    switch (get_peng()) {
    case 'E':
      p[0] += 1;
      break;
    case 'L':
      p[1] += 1;
      break;
    case 'M':
      p[2] += 1;
      break;
    }
  }
  const char *q[3] = {"Emperor", "Little", "Macaroni"};
  i = p[0] > p[1] ? 0 : 1;
  if (p[2] > p[i]) {
    i = 2;
  }
  printf("%s Penguin\n", q[i]);
  return 0;
}
