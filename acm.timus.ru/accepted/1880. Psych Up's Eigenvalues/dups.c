int main() {
#ifdef ONLINE_JUDGE
  int val[3][4000];
  int cnt[3];
  int i, j;
  for (i = 0; i < 3; ++i) {
    scanf("%d", cnt + i);
    for (j = 0; j < cnt[i]; ++j) {
      scanf("%d", &(val[i][j]));
    }
  }
#else
  int val[3][4000] = {
      {13, 20, 22, 43, 146}, {13, 22, 43, 146}, {13, 43, 67, 89, 146},
  };
  int cnt[3] = {5, 4, 5};
#endif
  int *a = val[0], *ea = a + cnt[0];
  int *b = val[1], *eb = b + cnt[1];
  int *c = val[2], *ec = c + cnt[2];
  int res = 0;
  while (a != ea && b != eb && c != ec) {
    if (*a == *b && *a == *c) {
      ++res;
    }

    int **p = &a;
    if (**p > *b)
      p = &b;
    if (**p > *c)
      p = &c;
    ++*p;
  }
  printf("%d\n", res);
  return 0;
}
