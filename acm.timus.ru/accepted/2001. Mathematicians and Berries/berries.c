int main() {
#ifdef ONLINE_JUDGE
  int a[3], b[3];
  int i;
  for (i = 0; i < 3; ++i) {
    scanf("%d%d", a + i, b + i);
  }
#else
  int a[3] = {1, 2, 0}, b[3] = {2, 1, 3};
#endif
  printf("%d %d\n", a[0] - a[2], b[0] - b[1]);
  return 0;
}
