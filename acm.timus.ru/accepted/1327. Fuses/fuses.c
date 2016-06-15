int main() {
  int A, B, i, j;
#if ONLINE_JUDGE
  scanf("%d%d\n", &A, &B);
#else
  A = 100, B = 200;
#endif
  for (i = A, j = 0; i <= B; ++i) {
    j += i % 2;
  }
  printf("%d\n", j);
  return 0;
}
