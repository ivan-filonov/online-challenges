static inline int next() {
#if ONLINE_JUDGE
  int v;
  scanf("%d", &v);
  return v;
#else
  static int a[] = {0,    2600, 3800, 2600, 2500, 2600, 0,    5300, 3900,
                    4400, 3800, 5300, 0,    1900, 4500, 2600, 3900, 1900,
                    0,    3700, 2500, 4400, 4500, 3700, 0},
             i = 0;
  return a[i++];
#endif
}

int m[5][5];

static inline void load() {
  int row, col;
  for (row = 0; row < 5; ++row) {
    for (col = 0; col < 5; ++col) {
      m[row][col] = next();
    }
  }
}

static inline int dist(int a, int b) { return m[a - 1][b - 1]; }

static inline void pr(int *a, int l) {
  int i;
  for (i = 0; i < l; ++i) {
    printf("%s%d", i ? " " : "", a[i]);
  }
  printf("\n");
}

static inline int d(int *p) {
  int res = 0, i;
  for (i = 0; i < 4; ++i) {
    res += dist(p[i], p[i + 1]);
  }
  return res;
}

int main() {
  load();
  int p[4][5] =
      {
          {1, 2, 3, 4, 5}, {1, 3, 2, 4, 5}, {1, 4, 3, 2, 5}, {1, 3, 4, 2, 5},
      },
      i, j, l, k;
  j = d(p[0]);
  k = 0;
  for (i = 1; i < 4; ++i) {
    l = d(p[i]);
    if (l < j) {
      j = l;
      k = i;
    }
  }
  printf("%d\n%d %d %d %d %d\n", j, p[k][0], p[k][1], p[k][2], p[k][3],
         p[k][4]);

  return 0;
}
