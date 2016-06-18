int get() {
#if ONLINE_JUDGE
  int i;
  scanf("%d", &i);
  return i;
#else
  static int ar[] =
      {
          2, 1054, 1492, 4, 1492, 65536, 1492, 100,
      },
             idx = 0;
  return ar[idx++];
#endif
}

int main() {
  int set_size = get();
  int set[set_size], i;
  for (i = 0; i < set_size; ++i) {
    set[i] = get();
  }
  int count = 0;
  for (i = get(); i > 0; --i) {
    int val = get();
    int l = 0, r = set_size - 1;
    if (set[l] == val || set[r] == val) {
      ++count;
    } else if (set[l] < val && set[r] > val) {
      while (l < r) {
        int mid = l + (r - l) / 2;
        int vmid = set[mid];
        if (vmid == val) {
          ++count;
          break;
        } else if (vmid > val) {
          r = mid;
        } else {
          l = mid + 1;
        }
      }
    }
  }
  printf("%d\n", count);
  return 0;
}
