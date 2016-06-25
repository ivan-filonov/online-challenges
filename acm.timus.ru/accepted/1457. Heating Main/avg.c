static inline int next() {
#if ONLINE_JUDGE
  int res;
  scanf("%d", &res);
  return res;
#else
  static int a[] = {3, 7, 5, 4}, b = 0;
  return a[b++];
#endif
}

int main() {
  int count = next();
  double sum = 0.0, div = 0.0;
  while (count-- > 0) {
    sum += (double)next();
    div += 1.0;
  }
  printf("%.6f\n", sum / div);
  return 0;
}
