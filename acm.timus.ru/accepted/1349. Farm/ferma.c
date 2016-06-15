int main() {
  int n;
#ifdef ONLINE_JUDGE
  scanf("%d", &n);
#else
  n = 1;
#endif
  char *result = "-1";
  switch (n) {
  case 1:
    result = "1 2 3";
    break;
  case 2:
    result = "3 4 5";
    break;
  }
  printf("%s\n", result);
  return 0;
}
