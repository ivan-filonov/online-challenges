int main() {
  int a, b;
#if ONLINE_JUDGE
  scanf("%d%d", &a, &b);
#else
  a = 100;
  b = 40;
#endif
  int r = b * 2 + 40;
  int l = a * 2 + 39;
  printf("%d\n", r > l ? r : l);
  return 0;
}
