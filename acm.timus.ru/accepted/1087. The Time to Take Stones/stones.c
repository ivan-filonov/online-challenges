int N, M;
char res[10001];
int k[50];
static int cmp(int *a, int *b) { return *a - *b; }
int main() {
  int i;
#ifdef ONLINE_JUDGE
  scanf("%d%d", &N, &M);
  for (i = 0; i < M; ++i) {
    scanf("%d", k + i);
  }
#else
  N = 3;
  M = 3;
  k[0] = 2;
  k[1] = 3;
  k[2] = 1;
#endif
  qsort(k, M, sizeof(int), cmp);
  for (i = 0; i <= N; ++i) {
    res[i] = 2;
  }
  int current;
  for (current = k[0]; current <= N; ++current) {
    int canInv = 0;
    int j;
    for (j = 0; j < M; ++j) {
      int move = k[j];
      if (current > move) {
        canInv = res[current - move] == 2;
      }
      if (canInv) {
        break;
      }
    }
    if (canInv) {
      res[current] = 1;
    }
  }
  printf("%d\n", res[N]);
  return 0;
}
