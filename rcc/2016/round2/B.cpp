#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::cin.sync_with_stdio(false);

  int numTests;
  std::cin >> numTests;

  while (numTests-- > 0) {
    int numBuyers;
    int numSellers;
    std::cin >> numBuyers >> numSellers;

    std::vector<int> buyers(numBuyers);
    for (auto &a : buyers) {
      std::cin >> a;
    }
    std::sort(buyers.begin(), buyers.end());

    std::vector<int> sellers(numSellers);
    for (auto &a : sellers) {
      std::cin >> a;
    }
    std::sort(sellers.rbegin(), sellers.rend());

    std::string ops;
    std::cin >> ops;

    int balance = 0;
    int failed = 0;
    for (auto c : ops) {
      if (c == '+') {
        balance += buyers.back();
        buyers.pop_back();
      } else {
        if (sellers.back() > balance) {
          ++failed;
          continue;
        }
        balance -= sellers.back();
        sellers.pop_back();
      }
    }
    std::cout << failed << "\n";
  }

  return 0;
}
