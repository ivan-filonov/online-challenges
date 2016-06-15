static int g() {
#if ONLINE_JUDGE
  int i;
  scanf("%d", &i);
  return i;
#else
  static int a[] = {6, 5, 3, 1, 3, 1, 5, 3, 6, 5, 5, 5, 7, 2};
  static int i = 0;
  return a[i++];
#endif
}
int main() {
  int n = g();
  int s = g() - 2;
  while (n-- > 0) {
    s += g();
    s -= g();
    s -= 2;
  }
  if (s >= 0) {
    printf("%d\n", s);
  } else {
    printf("Big Bang!\n");
  }
  return 0;
}
