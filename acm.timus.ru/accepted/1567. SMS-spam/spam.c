#include <stdio.h>
int main() {
  char buf[1001];
#if ONLINE_JUDGE
  fgets(buf, sizeof(buf), stdin);
#else
  const char *test =
      "pokupaite gvozdi tolko v kompanii gvozdederov i tovarischi!\n" /* 114 */;
  char *d = buf;
  while (*d++ = *test++) {
  }
#endif
  const char *c = buf;
  int r = 0;
  for (; *c; ++c) {
    if (*c >= 'a' && *c <= 'z') {
      r += 1 + (*c - 'a') % 3;
    } else
      switch (*c) {
      case '.':
        r += 1;
        break;
      case ',':
        r += 2;
        break;
      case '!':
        r += 3;
        break;
      case ' ':
        r += 1;
        break;
      }
  }
  printf("%d\n", r);
  return 0;
}
