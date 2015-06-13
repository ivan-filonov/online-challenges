#include <algorithm>
#include <stdio.h>
#include <vector>

template<typename V> using vector = std::vector<V>;

struct ev {
  int start;
  int finish;

  ev(int _start, int _finish) : start(_start), finish(_finish) {}
  bool operator < (const ev & o) const {
    if(finish == o.finish) {
      return start < o.start;
    }
    return finish < o.finish;
  }
};

int main() {
  int n;
  scanf("%d", &n);
  vector<ev> v;
  v.reserve(100000);
  for(int i = 0, j, k; i != n; ++i) {
    scanf("%d %d", &j, &k);
    v.emplace_back(j, k);
  }

  std::sort(v.begin(), v.end());

  int a = 0, count = 1;
  for(int i = 1; i != v.size(); ++i) {
    if(v[i].start > v[a].finish) {
      a = i;
      ++count;
    }
  }

  printf("%d\n", count);

  return 0;
}
