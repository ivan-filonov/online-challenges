int main() {
  int n;
#if ONLINE_JUDGE
  scanf("%d", &n);
#else
  n = 2;
#endif
  char *r = "Glupenky Pierre";
  switch (n) {
  case 1:
    r = "16";
    break;
  case 2:
    r = "16 06";
    break;
  case 3:
    r = "16 06 68";
    break;
  case 4:
    r = "16 06 68 88";
    break;
  }
	printf("%s\n", r);
  return 0;
}
