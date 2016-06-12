static int cmp(int *a, int *b) { return *a - *b; }
static inline int get(char *a, int n) { return (a[n / 8] >> (n % 8)) & 1; }
static inline void set(char *a, int n) { a[n / 8] |= 1 << (n % 8); }
int main() {
  int N, M, i, *k;
  char *res;
#ifdef ONLINE_JUDGE
  scanf("%d%d", &N, &M);
  k = malloc(M * sizeof(int));
  for (i = 0; i < M; ++i) {
    scanf("%d", k + i);
  }
#else
  N = 17;
  M = 3;
  k = malloc(M * sizeof(int));
  k[0] = 4;
  k[1] = 3;
  k[2] = 1;
#endif
  qsort(k, M, sizeof(int), cmp);
  res = malloc(N / 8 + 1);
  memset(res, 0, N / 8 + 1);
  int current;
  for (current = k[0]; current <= N; ++current) {
    int canInv = 0;
    int j;
    for (j = 0; j < M; ++j) {
      int move = k[j];
      if (current > move) {
        canInv = get(res, current - move) == 0;
      }
      if (canInv) {
        break;
      }
    }
    if (canInv) {
      set(res, current);
    }
  }
  free(k);
  printf("%d\n", 2 - get(res, N));
  free(res);
  return 0;
}
