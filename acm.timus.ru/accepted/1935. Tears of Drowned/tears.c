static inline int next() {
  int v;
#if ONLINE_JUDGE
  scanf("%d", &v);
#else
  static int a[] = {3, 5, 10, 3}, i = 0;
  // static int a[] = {6, 1, 3, 2, 5, 4, 6}, i = 0;
  v = a[i++];
#endif
  return v;
}

static int comp(const int *a, const int *b) { return *a - *b; }

int main() {
  int n = next(), i, r, m, j;
  for (r = m = i = 0; i < n; ++i) {
    j = next();
    r += j;
    if (j > m) {
      m = j;
    }
  }
  r += m;
  printf("%d\n", r);
  return 0;
}
