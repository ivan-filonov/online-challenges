int main() {
  int vol_thickness, cov_thickness, start, stop;
#if ONLINE_JUDGE
  scanf("%d%d%d%d", &vol_thickness, &cov_thickness, &start, &stop);
#else
  vol_thickness = 10, cov_thickness = 1, start = 1, stop = 2;
#endif
  int res = vol_thickness; // start==stop
  const int full_thickness = (vol_thickness + 2 * cov_thickness);
  const int d = abs(stop - start) - 1;
  if (start < stop) {
    res = 2 * cov_thickness + d * full_thickness;
  } else {
    res = 2 * vol_thickness + 2 * cov_thickness + d * full_thickness;
  }
  printf("%d\n", res);
  return 0;
}
