int main() {
  int n, k, r;
  scanf("%d%d", &n, &k);
  n *= 2;
  r = n / k;
  if (r < 2) {
    r = 2;
  } else {
    if (n % k) {
      r += 1;
    }
  }
  printf("%d\n", r);
  return 0;
}
