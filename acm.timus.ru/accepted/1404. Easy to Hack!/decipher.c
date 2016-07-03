static inline int dec(char c, int val) { return (c + 26 - val) % 26; }
int main() {
  char buf[101];
#if ONLINE_JUDGE
  scanf("%100s", buf);
#else
  strcpy(buf, "xbduyr");
#endif
  const int len = strlen(buf);
  for (int i = 0; i < len; ++i) {
    buf[i] -= 'a';
  }
  for (int pos = len - 1; pos > 0; --pos) {
    buf[pos] = dec(buf[pos], buf[pos - 1]);
  }
  buf[0] = dec(buf[0], 5);
  for (int i = 0; i < len; ++i) {
    buf[i] += 'a';
  }
  printf("%s\n", buf);
  return 0;
}
