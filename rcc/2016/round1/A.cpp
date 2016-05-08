#include <algorithm>
#include <cstdio>
#include <iostream>

bool check(int zz, int zo, int oz, int oo) {
  if (std::abs(oz - zo) > 1) {
    return false;
  } else if (zz && oo && !(oz || zo)) {
    return false;
  }

  return true;
}

std::string FAIL = "impossible";
std::string build(int zz, int zo, int oz, int oo) {
  if (!check(zz, zo, oz, oo)) {
    return "impossible";
  }
  std::string res;
  if (!zo && !oz) {
    if (zz && oo) {
      return FAIL;
    } else if (zz) {
      res = "0";
      while (zz-- > 0) {
        res += '0';
      }
    } else if (oo) {
      res = "1";
      while (oo-- > 0) {
        res += '1';
      }
    }
  } else if (zo >= oz) {
    res = "0";
    while (zz-- > 0) {
      res += '0';
    }
    --zo;
    res += '1';
    while (oo-- > 0) {
      res += '1';
    }
    for (;;) {
      if (!oz) {
        break;
      }
      --oz;
      res += '0';
      if (!zo) {
        break;
      }
      --zo;
      res += '1';
    }
  } else if (zo < oz) {
    res = "1";
    while (oo-- > 0) {
      res += '1';
    }
    --oz;
    res += '0';
    while (zz-- > 0) {
      res += '0';
    }
    for (;;) {
      if (!zo) {
        break;
      }
      --zo;
      res += '1';
      if (!oz) {
        break;
      }
      --oz;
      res += '0';
    }
  }

  return res;
}

int main() {
  std::cin.sync_with_stdio(false);

  int t;
  std::cin >> t;

  for (; t; --t) {
    int zz, zo, oz, oo;
    std::cin >> zz >> zo >> oz >> oo;
    std::cout << build(zz, zo, oz, oo) << "\n";
  }
}
