int main() {
  int q, z, r[10], i;
#if ONLINE_JUDGE
  scanf("%d%d", &q, &z);
  for (i = 0; i < 10; ++i) {
    scanf("%d", r + i);
  }
#else
  q = 300;
  z = 719;
  for (i = 0; i < 10; ++i) {
    r[i] = 0;
  }
  r[6] = 21;
#endif
  z -= q;
  for (i = 0; i < 10; ++i) {
    z -= r[i] * 20;
  }
  printf("%s\n", z < 0 ? "Dirty debug :(" : "No chance.");
  return 0;
}
