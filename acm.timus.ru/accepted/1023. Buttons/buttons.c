int main() {
  int K, K2;
#if ONLINE_JUDGE
  scanf("%d", &K);
#else
  K = 97;
#endif

  int i, L;

  L = K - 1;
  K2 = K / 2 + 1;

  for (i = 3; i < K2; ++i) {
    if (K % i == 0) {
      L = i - 1;
      break;
    }
  }

  printf("%d\n", L);
  return 0;
}
