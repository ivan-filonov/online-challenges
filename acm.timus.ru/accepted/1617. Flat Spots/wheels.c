static int get() {
#if ONLINE_JUDGE
  int r;
  scanf("%d", &r);
  return r;
#else
  static int a[] = {9, 689, 690, 690, 689, 689, 690, 689, 688, 688};
  static int i = 0;
  return a[i++];
#endif
}
int main() {
  int cnt[102] =
      {
          0,
      },
      i, j;
  i = get();
  while (i-- > 0) {
    cnt[get() - 600] += 1;
  }
  for (j = i = 0; i < 102; ++i) {
    j += cnt[i] / 4;
  }
  printf("%d\n", j);
  return 0;
}
