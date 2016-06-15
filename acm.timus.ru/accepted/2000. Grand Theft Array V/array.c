#if ONLINE_JUDGE
static void geti(int *i) { scanf("%d", i); }
#else
static int a_[] = {10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 4, 8};
static int i_ = 0;
static void geti(int *i) { *i = a_[i_++]; }
#endif
int main() {
  int n, *a, *sb, pa, pb, i, t;
  geti(&n);
  a = malloc(sizeof(int) * n);
  sb = malloc(sizeof(int) * n);
  sb[0] = 0;
  t = 0;
  for (i = 0; i < n; ++i) {
    geti(a + i);
    t += a[i];
    if (i) {
      sb[i] = a[i - 1] + sb[i - 1];
    }
  }
  geti(&pa);
  geti(&pb);

  --pa;
  --pb;

  int mid = pa + (pb - pa) / 2;
  int ls = sb[mid];
  int mv = a[mid];
  int rs = t - ls - mv;

  free(a);
  free(sb);

	int ra;
	if(pa == pb) {
		ra = ls > rs ? ls : rs;
		ra += mv;
	} else if(pa + 1 == pb) {
		ra = ls;
		if(pa%2) {
			ra += mv;
		}
	} else if(pa - 1 == pb) {
		ra = rs;
		if(pa%2) {
			ra += mv;
		}
	} else if(pa < pb) {
		ra = ls + mv;
	} else {
		ra = rs + mv;
	}

  printf("%d %d\n", ra, t - ra);

  return 0;
}
