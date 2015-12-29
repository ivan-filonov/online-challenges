/*
 There is a travel agency in Adelton town on Zanzibar island. It has decided to
offer its clients, besides many other attractions, sightseeing the town. To earn
as much as possible from this attraction, the agency has accepted a shrewd
decision: it is necessary to find the shortest route which begins and ends at
the same place.
 Your task is to write a program which finds such a route. In the town there are
N crossing points numbered from 1 to N and M two-way roads numbered from 1 to M.
Two crossing points can be connected by multiple roads, but no road connects a
crossing point with itself. Each sightseeing route is a sequence of road numbers
y1, …, yk, k > 2. The road yi (1 ≤ i ≤ k − 1) connects crossing points xi and
xi+1, the road yk connects crossing points xk and x1. All the numbers x1, …, xk
should be different. The length of the sightseeing route is the sum of the
lengths of all roads on the sightseeing route, i.e. L(y1) + L(y2) + … + L(yk)
where L(yi) is the length of the road yi (1 ≤ i ≤ k). Your program has to find
such a sightseeing route, the length of which is minimal, or to specify that it
is not possible, because there is no sightseeing route in the town.

 Input
 Input contains T tests (1 ≤ T ≤ 5). The first line of each test contains two
integers: the number of crossing points N and the number of roads M (3 ≤ N ≤
100; 3 ≤ M ≤ N · (N − 1)). Each of the next M lines describes one road. It
contains 3 integers: the number of its first crossing point a, the number of the
second one b, and the length of the road l (1 ≤ a, b ≤ N; a ≠ b; 1 ≤ l ≤ 300).
Input is ended with a “−1” line.

 Output
 Each line of output is an answer. It contains either a string “No solution.” in
case there isn't any sightseeing route, or it contains the numbers of all
crossing points on the shortest sightseeing route in the order how to pass them
(i.e. the numbers x1 to xk from our definition of a sightseeing route),
separated by single spaces. If there are multiple sightseeing routes of the
minimal length, you can output any one of them.

 test:
 input
 5 7
 1 4 1
 1 3 300
 3 1 10
 1 2 16
 2 3 100
 2 5 15
 5 3 20
 4 3
 1 2 10
 1 3 20
 1 4 30
 -1

expected:
1 3 5 2
No solution.
 */
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

using std::string;
template <typename V> using vector = std::vector<V>;
template <typename K, typename V> using map = std::unordered_map<K, V>;

int main() {
  std::cin.sync_with_stdio(false);
  for (int N, M; std::cin >> N && N > -1;) {
    std::cin >> M;
    for (int i = 0; i < M; ++i) {
      int u, v, dist;
      std::cin >> u >> v >> dist;
    }
    const char *NO = "No solution.\n";
    if (2 >= N) {
      std::cout << NO;
      continue;
    }
  }
}
