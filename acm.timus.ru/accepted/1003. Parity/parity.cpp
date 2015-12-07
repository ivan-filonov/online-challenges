#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using std::string;
template <typename K, typename V> using map = std::map<K, V>;
template <typename V> using set = std::set<V>;
template <typename V> using vector = std::vector<V>;

class State {
public:
  map<int, int> f2l, l2f;
  map<int, bool> f2odd;

  bool add(int first, int last, bool odd) {
    const int haveLeft = f2l.count(first);
    // exact match
    if (haveLeft && f2l[first] == last) {
      return f2odd.at(first) == odd;
    }

    // no match
    if (!haveLeft && !l2f.count(last)) {
      f2l[first] = last;
      l2f[last] = first;
      f2odd[first] = odd;
      return true;
    }

    // left match
    if (haveLeft) {
      const int oldLast = f2l[first];
      const bool oldOdd = f2odd[first] ^ odd;
      if (oldLast > last) {
        // split
        f2l.erase(first);
        f2odd.erase(first);
        l2f.erase(oldLast);
        return add(first, last, odd) && add(last + 1, oldLast, oldOdd);
      } else {
        // append
        return add(oldLast + 1, last, oldOdd);
      }
    }

    // right match
    const int oldFirst = l2f[last];
    const bool oldOdd = f2odd[oldFirst] ^ odd;

    if (oldFirst < first) {
      // split
      f2l.erase(oldFirst);
      f2odd.erase(oldFirst);
      l2f.erase(last);
      return add(oldFirst, first - 1, oldOdd) && add(first, last, odd);
    }

    // prepend
    return add(first, oldFirst - 1, oldOdd);
  }
};

int main() {
  std::cin.sync_with_stdio(false);
  std::istringstream ss_{"10\n"
                         "5\n"
                         "1 2 even\n"
                         "3 4 odd\n"
                         "5 6 even\n"
                         "1 6 even\n"
                         "7 10 odd\n"
                         "-1\n"}; // 3
  std::istringstream ss_2{"100000000\n"
                          "10\n"
                          "1 5 even\n"
                          "6 10 even\n"
                          "11 18 even\n"
                          "19 25 even\n"
                          "1 8 even\n"
                          "16 25 even\n"
                          "16 40 even\n"
                          "9 40 odd\n"
                          "1000 5000 odd\n"
                          "1000 6000 odd\n"
                          "-1\n"}; // 7
  auto &ss = std::cin;

  for (int seqLength = 0; ss >> seqLength && seqLength > -1;) {
    int numQuestions = 0;
    ss >> numQuestions;

    State state;
    bool done = false;
    int result = 0;
    while (numQuestions-- > 0) {
      int first;
      int last;
      std::string parity;
      ss >> first >> last >> parity;

      if (!done) {
        if (first < 0 || last > seqLength) {
          done = true;
          continue;
        }
        if (state.add(first, last, parity == "odd")) {
          ++result;
        } else {
          done = true;
        }
      }
    }
    std::cout << result << "\n";
  }

  return 0;
}
