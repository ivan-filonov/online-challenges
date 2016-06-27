static inline int next() {
  int v;
#if ONLINE_JUDGE
  scanf("%d", &v);
#else
  static int a[] = {3, 4, 1, 6}, i = 0;
  v = a[i++];
#endif
  return v;
}
static int comp(int *a, int *b) { return *b - *a; }
int main() {
  int N = next(), v[25001], i;
  if (!N) {
    goto exit;
  }

  for (i = 0; i < N; ++i) {
    v[i] = next();
  }

  qsort(v, N, sizeof(int), comp);

  for (i = 0; i < N; ++i) {
    printf("%d\n", v[i]);
  }

exit:
  return 0;
}
