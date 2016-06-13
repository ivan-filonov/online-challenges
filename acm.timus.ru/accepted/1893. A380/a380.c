int main() {
  char buf[32];
#if ONLINE_JUDGE
  scanf("%30s", buf);
#else
  strcpy(buf, "21F");
#endif
  char *l;
  int row = strtol(buf, &l, 10);
  const char *w = "window", *a = "aisle", *n = "neither";
  if (row < 3) {
    switch (*l) {
    case 'A':
    case 'D':
      l = w;
      break;
    default:
      l = a;
    }
  } else if (row < 21) {
    switch (*l) {
    case 'A':
    case 'F':
      l = w;
      break;
    default:
      l = a;
    }
  } else {
    switch (*l) {
    case 'A':
    case 'K':
      l = w;
      break;
    case 'C':
    case 'D':
    case 'G':
    case 'H':
      l = a;
      break;
    default:
      l = n;
    }
  }
  printf("%s\n", l);
  return 0;
}
