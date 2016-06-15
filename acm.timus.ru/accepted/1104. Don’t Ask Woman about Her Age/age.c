static const char *s = "A1A";
static int geti() {
  int c;
#if ONLINE_JUDGE
  c = getchar();
#else
  c = *s++;
#endif
  if (c < '0') {
    return -1;
  }
  if (c <= '9') {
    return c - '0';
  }
  return c - 'A' + 10;
}
int main() {
  int i, j, s = 0, m = 2;
  for (;;) {
    i = geti();
    if (-1 == i) {
      break;
    }
		if(i >= m) {
			m = i + 1;
		}
		s += i;
  }
	
	for(j = -1, i = m - 1; i < 37; ++i) {
		if(s % i == 0) {
			j = i + 1;
			break;
		}
	}

  if (j > 0) {
    printf("%d\n", j);
  } else {
    printf("No solution.\n");
  }
  return 0;
}
