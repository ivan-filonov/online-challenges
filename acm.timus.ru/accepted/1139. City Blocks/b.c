static inline int gcd(int a, int b) {
  int c;
  while (a) {
    c = a;
    a = b % a;
    b = c;
  }
  return b;
}
int main() {
  int n, m;
#if ONLINE_JUDGE
  scanf("%d%d", &n, &m);
#else
  n = 4;
  m = 3;
#endif
  --n, --m;
  printf("%d\n", m + n - gcd(n, m));
  return 0;
}
