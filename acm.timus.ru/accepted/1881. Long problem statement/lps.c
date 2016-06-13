int main() {
  int p_h, p_w, nw, l = 0, ll = 0, i, j;
  scanf("%d%d%d\n", &p_h, &p_w, &nw);
  char p[101];
  while (nw-- > 0) {
    scanf("%s", p);
    i = strlen(p);

    if (ll && ll + 1 + i <= p_w) {
      ll += 1 + i;
    } else {
      ll = i;
      l += 1;
    }
  }
  i = l % p_h;
  printf("%d\n", l / p_h + (i ? 1 : 0));
  return 0;
}
