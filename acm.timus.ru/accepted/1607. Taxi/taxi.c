int main() {
  int op = 150, ip = 50, od = 1000, dd = 100, r;
#ifdef ONLINE_JUDGE
  scanf("%d%d%d%d", &op, &ip, &od, &dd);
#endif
  if (op >= od) {
    r = op;
  } else {
    for (;;) {
      op += ip;
      if (op >= od) {
        r = od;
        break;
      }
      od -= dd;
      if (od <= op) {
        r = op;
        break;
      }
    }
  }
  printf("%d\n", r);
  return 0;
}
