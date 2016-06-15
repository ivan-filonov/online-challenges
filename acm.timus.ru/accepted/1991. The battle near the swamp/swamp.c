static int get() {
  int r;
#if ONLINE_JUDGE
  scanf("%d", &r);
#else
  static int _v[] = {4, 5, 2, 7, 5, 0};
  static int _i = 0;
  r = _v[_i++];
#endif
  return r;
}
int main() {
  int s = 0, u = 0, i, k, j;
  i = get();
  k = get();
  while (i-- > 0) {
    j = get();
    if (j > k) {
      u += j - k;
    }
    if (j < k) {
      s += k - j;
    }
  }
  printf("%d %d", u, s);
  return 0;
}
