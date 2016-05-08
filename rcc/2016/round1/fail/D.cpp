#include <algorithm>
#include <cstdio>
#include <vector>

int main() {
  int ntasks, nqueries;

	::scanf("%d %d", &ntasks, &nqueries);

	using N = long long int;
  std::vector<int> t(ntasks);
  for (auto &i : t) {
		::scanf("%d", &i);
  }

  while (nqueries-- > 0) {
    int cmd, val, z = 0;
		::scanf("%d %d", &cmd, &val);
    switch (cmd) {
    case 1:
      ++t[val - 1];
      break;

    case 2:
      --t[val - 1];
      break;

    case 3:
      for (auto tt : t) {
				z += tt / val;
        if (tt % val) {
          ++z;
        }
      }
			printf("%d\n", z);
      break;
    }
  }
}
