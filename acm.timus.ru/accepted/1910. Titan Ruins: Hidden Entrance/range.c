#ifdef ONLINE_JUDGE
static inline int nextInt() {
  int val;
  scanf("%d", &val);
  return val;
}
#else
static int data__[] = {6, 1, 4, 4, 4, 1, 1};
static int data__ofs = 0;
static int data__end = 7;
static inline int nextInt() {
  if (data__ofs == data__end) {
    raise(11);
  }
  return data__[data__ofs++];
}
#endif
int main() {
  int n = nextInt();
  int rsum, rpos = 2;
  int w[3], i, sum;
  for (i = 0, sum = 0; i < 3; ++i) {
    w[i] = nextInt();
    sum += w[i];
  }
  rsum = sum;

  // printf("window: %d %d %d\n", w[0], w[1], w[2]);
  for (i = 3; i < n; ++i) {
    int v = nextInt();
    // printf("drop %d, add %d, ", w[i % 3], v);
    sum -= w[i % 3];
    w[i % 3] = v;
    sum += v;
    // printf("sum -> %d (rsum: %d)\n", sum, rsum);
    // printf("window: %d %d %d\n", w[(i - 2) % 3], w[(i - 1) % 3], w[(i) % 3]);
    if (sum > rsum) {
      rsum = sum;
      rpos = i;
    }
  }

  printf("%d %d\n", rsum, rpos);

  return 0;
}
