int main() {
  char buf[51];
  int cnt[26] =
      {
          0,
      },
      i, j;
#if ONLINE_JUDGE
  scanf("%s", buf);
#else
  strcpy(buf, "tebidohtebidoh");
#endif

  for (i = 0; buf[i]; ++i) {
    cnt[buf[i] - 'a']++;
  }

  for (i = 0, j = 0; i < 26; ++i) {
    if (cnt[i] > cnt[j]) {
      j = i;
    }
  }

  printf("%c\n", 'a' + j);

  return 0;
}
