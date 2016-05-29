#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::cin.sync_with_stdio(false);

  int numTests;
  std::cin >> numTests;

  while (numTests-- > 0) {
    int pages;
    int queries;
    std::cin >> pages >> queries;

    std::vector<int> book;
    book.reserve(pages);
    for (int i = 0; i < pages; ++i) {
      book.push_back(i + 1);
    }

    while (queries-- > 0) {
      std::string cmd;
      int page;
      std::cin >> cmd >> page;
      if (cmd == "-") {
        auto it = std::lower_bound(book.begin(), book.end(), page);
        auto dist = std::distance(book.begin(), it) + 1;
        auto it2 = book.end() - dist;
        if (it > it2) {
          std::swap(it, it2);
        }
        book.erase(it2);
        book.erase(it);
      } else {
        std::cout << book[page - 1] << "\n";
      }
    }
  }

  return 0;
}
