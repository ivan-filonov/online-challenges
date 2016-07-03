int main() {
  int N;
#if ONLINE_JUDGE
  scanf("%d", &N);
#else
  N = 7;
#endif
  const unsigned int n2 = 2u * N;
  unsigned int p = sqrt(n2) + 1;
  while (p * p + p > n2) {
    --p;
  }
  unsigned int a = 0;
  for (; p >= 1; --p) {
    const unsigned int pp = p * p + p;
    a = (N - pp / 2) / p;
    if (pp + 2 * a * p == n2) {
      break;
    }
  }
  ++a;
  printf("%d %d\n", a, p);
  return 0;
}
