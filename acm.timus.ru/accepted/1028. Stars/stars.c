#include <stdint.h>
#include <stdio.h>
static uint16_t L0[32] = {
    0,
};
static uint16_t L1[32][32] = {
    {
        0,
    },
};
static uint16_t L2[32][32][32] = {
    {
        {
            0,
        },
    },
};
static uint16_t ans[16384] = {
    0,
};
static void put_star(const int x) {
  const int i2 = x % 32;
  const int i1 = (x / 32) % 32;
  const int i0 = x / 1024;
  int i, rank = 0;
  for (i = 0; i < i0; ++i) {
    rank += L0[i];
  }
  for (i = 0; i < i1; ++i) {
    rank += L1[i0][i];
  }
  for (i = 0; i <= i2; ++i) {
    rank += L2[i0][i1][i];
  }

  ans[rank] += 1;

  L2[i0][i1][i2] += 1;
  L1[i0][i1] += 1;
  L0[i0] += 1;
}

int main() {
  int N, i;
#ifdef ONLINE_JUDGE
  scanf("%d", &N);
  for (i = 0; i < N; ++i) {
    int x, y;
    scanf("%d%d", &x, &y);
    put_star(x);
  }
#else
  int coords[5] = {1, 5, 7, 3, 5};
  N = sizeof(coords) / sizeof(coords[0]);
  for (i = 0; i < N; ++i) {
    put_star(coords[i]);
  }
#endif
  for (i = 0; i < N; ++i) {
    printf("%d\n", ans[i]);
  }
  return 0;
}
