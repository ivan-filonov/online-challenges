static inline int next() {
  int v;
#if ONLINE_JUDGE
  scanf("%d", &v);
#else
  static int a[] = {3, 0, 1, 2, 2, 2, 3, 1, 2, 2, 4, 1, 5, 1}, i = 0;
  v = a[i++];
#endif
  return v;
}
int main() {
  int t = next(), p = next();
  printf("%d\n", t * (t - 1) / 2 - p);
  return 0;
}
