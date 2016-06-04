/*
One curious child has a set of N little bricks (5 ≤ N ≤ 500). From these bricks
he builds different staircases. Staircase consists of steps of different sizes
in a strictly descending order. It is not allowed for staircase to have steps
equal sizes. Every staircase consists of at least two steps and each step
contains at least one brick. Picture gives examples of staircase for N=11 and
N=5:
Your task is to write a program that reads the number N and writes the only
number Q — amount of different staircases that can be built from exactly N
bricks.
Input
Number N
Output
Number Q

Sample
input
212
output
995645335
*/
#include <iostream>
#include <sstream>
#include <vector>

template <typename V> using vector = std::vector<V>;

int main() {
  int N;
#ifdef ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
  cin >> N;
#else
  N = 212;
#endif

  using num_t = long long int;
  vector<num_t> ans(N + 1, 0);
  ans[0] = 1;

  for (int dist = 1; dist <= N; ++dist) {
    for (int pos = N; pos >= dist; --pos) {
      ans[pos] += ans[pos - dist];
    }
  }
  std::cout << (ans[N] - 1);

  return 0;
}
