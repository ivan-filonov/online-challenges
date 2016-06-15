int main() {
  char buf[80];
#if ONLINE_JUDGE
  scanf("%79s", buf);
#else
  strcpy(buf, "123456123456");
#endif
  char *p = buf;
  int r = 0;
  while (*p) {
    r *= 10;
    r += *p - '0';
    r %= 7;
		++p;
  }
  printf("%d\n", r);
  return 0;
}
