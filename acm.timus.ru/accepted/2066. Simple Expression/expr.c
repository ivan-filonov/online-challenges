int main() {
  int a, b, c /* 0 <= a <= b <= c <= 100 */;
#ifdef ONLINE_JUDGE
  scanf("%d%d%d", &a, &b, &c);
#else
  a = 1;
  b = 2;
  c = 3;
#endif
  printf("%d\n", a - ((b * c > b + c) ? b * c : b + c));
  return 0;
}
