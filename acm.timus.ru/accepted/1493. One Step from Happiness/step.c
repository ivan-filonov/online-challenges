static inline int sum(int v) {
  int r = 0;
  while (v) {
    r += v % 10;
    v /= 10;
  }
	return r;
}
int main() {
  char buf[16];
#ifdef ONLINE_JUDGE
  scanf("%s", buf);
#else
  strcpy(buf, "445219");
  strcpy(buf, "012200");
  strcpy(buf, "715068");
#endif
  int n = strtol(buf, 0, 10);
  int left = sum(n / 1000), right = n % 1000;
  if (left == sum((right + 1) % 1000) || left == sum((right + 999) % 1000)) {
    printf("Yes\n");
  } else {
    printf("No\n");
  }
  return 0;
}
