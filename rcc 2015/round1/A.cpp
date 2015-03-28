#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
  std::string t;

  std::getline(std::cin, t);
  int num_tests = stoi(t);

  for(int i = 0; i < num_tests; ++i) {
    std::vector<int> gri, dim;

    std::getline(std::cin, t);
    std::istringstream ss1 { t };
    int n, l;
    ss1 >> n >> l;

    std::getline(std::cin, t);
    std::istringstream ss2 { t };
    for(int j = 0; j < n; ++j) {
      int k;
      ss2 >> k;
      gri.push_back(k);
    }

    std::getline(std::cin, t);
    std::istringstream ss3 { t };
    for(int j = 0; j < n; ++j) {
      int k;
      ss3 >> k;
      dim.push_back(k);
    }

    std::sort(gri.begin(), gri.end());
    std::sort(dim.begin(), dim.end());
    auto sg = std::accumulate(gri.begin(), gri.end() - (n-l), 0);
    auto sd = std::accumulate(dim.begin() + (n-l), dim.end(), 0);
    std::cout << ((sg > sd) ? "YES" : "NO") << "\n";
  }

  return 0;
}
