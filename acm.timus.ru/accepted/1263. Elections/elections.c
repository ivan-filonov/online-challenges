static int get() {
  int r;
#ifdef ONLINE_JUDGE
  scanf("%d", &r);
#else
  static int n__[] = {3, 6, 1, 2, 3, 2, 1, 1};
  static int n_ = 0;
  r = n__[n_++];
#endif
  return r;
}
int main() {
  int v[10001] =
      {
          0,
      },
      i;
  const int n = get();
  const int m = get();
  for (i = 0; i < m; ++i) {
    int j = get();
    v[j] += 1;
  }

  for (i = 1; i <= n; ++i) {
    v[i] *= 10000;
    int t = v[i];
    v[i] = t / m + (t % m > m / 2 ? 1 : 0);
    printf("%d.%2.2d%%\n", v[i] / 100, v[i] % 100);
  }
  return 0;
}
