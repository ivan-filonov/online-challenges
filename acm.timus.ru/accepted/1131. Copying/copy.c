int main() {
  int N, K;
#if ONLINE_JUDGE
  scanf("%d%d", &N, &K);
#else
  N = 8;
  K = 3;
#endif
  int t = 0, incr = 1;

  N -= 1;

  for (;;) {
    if (!N) {
      break;
    }

    if (N > incr) {
      N -= incr;
    } else {
      N = 0;
    }

    if (incr * 2 < K) {
      incr *= 2;
    } else {
      incr = K;
    }

    ++t;

    if (incr == K) {
      break;
    }
  }
  t += N / K;
  if (N % K) {
    ++t;
  }
  printf("%d\n", t);

  return 0;
}
