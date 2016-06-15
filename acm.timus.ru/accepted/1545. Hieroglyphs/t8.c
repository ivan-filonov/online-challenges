char buf[4];
#ifdef ONLINE_JUDGE
static int getn() {
  int n;
  scanf("%d", &n);
  return n;
}
static void getb() { scanf("%s", buf); }
#else
static char *sa_[] = {"na", "no", "ni", "ki", "ka", "ku", "k"};
static int si_ = 0;
static void getb() { strcpy(buf, sa_[si_++]); }
static int getn() { return 6; }
#endif
int main() {
  int n = getn();
  int m[32][32];
	memset(m, 0, sizeof(m));
  while (n-- > 0) {
    getb();
    m[buf[0] - 'a'][buf[1] - 'a'] = 1;
  }
  getb();
  for (n = 0; n < 26; ++n) {
    if (m[buf[0] - 'a'][n]) {
      printf("%c%c\n", buf[0], n + 'a');
    }
  }
  return 0;
}
