int main() {
#if ONLINE_JUDGE
  int a[7];
  {
    int i;
    for (i = 0; i < 7; ++i) {
      scanf("%d", a + i);
    }
  }
#else
  int a[7] = {1, 1, 1, 1, 1, 1, 1};
#endif
  int id = (a[1] ^ a[2] ^ a[3] ^ a[4]) * 4 + (a[0] ^ a[2] ^ a[3] ^ a[5]) * 2 +
           (a[0] ^ a[1] ^ a[3] ^ a[6]) * 1;
  int t[] = {7, 6, 1, 5, 2, 3, 4};
  int i;
  if (id) {
    int j = t[id - 1] - 1;
    a[j] = 1 - a[j];
  }
  for (i = 0; i < 7; ++i) {
    printf("%s%d", i ? " " : "", a[i]);
  }
  printf("\n");
  return 0;
}
