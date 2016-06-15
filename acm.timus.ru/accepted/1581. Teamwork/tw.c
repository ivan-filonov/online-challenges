static int z() {
#ifdef ONLINE_JUDGE
  int r;
  scanf("%d", &r);
  return r;
#else
  static int v[] = {8, 1, 1, 2, 3, 3, 3, 10, 10};
  static int j = 0;
  return v[j++];
#endif
}

int main() {
  int N = z();
  int cur = -1;
  int cnt;
  while (N-- > 0) {
    int v = z();
    if (v == cur) {
      ++cnt;
    } else {
      if (cur != -1) {
        printf("%d %d ", cnt, cur);
      }
      cur = v;
      cnt = 1;
    }
  }
  printf("%d %d\n", cnt, cur);
  return 0;
}
