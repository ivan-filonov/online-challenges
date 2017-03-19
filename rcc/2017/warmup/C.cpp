#include <array>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main() {
  std::cin.sync_with_stdio(false);
  using N = int64_t;

  N g, r, b;
  std::cin >> g >> r >> b;

  using partset = std::unordered_map<N, N>;
  std::array<partset, 10> green;
  for (N i = 0; i < g; ++i) {
    N n;
    std::cin >> n;
    green[n % 10][n] += 1;
  }

  std::array<std::array<partset, 10>, 10> red;
  for (N i = 0; i < r; ++i) {
    std::string n;
    std::cin >> n;
    red[n.front() - '0'][n.back() - '0'][std::stoll(n)] += 1;
  }

  std::array<partset, 10> blue;
  for (N i = 0; i < b; ++i) {
    std::string n;
    std::cin >> n;
    blue[n.front() - '0'][std::stoll(n)] += 1;
  }

  N res = 0;
  for (int left = 0; left < 10; ++left) {
    for (int right = 0; right < 10; ++right) {
      auto &G = green[left];
      auto &R = red[left][right];
      auto &B = blue[right];
      if (G.empty() || R.empty() || B.empty()) {
        continue;
      }
      N ng = 0, nr = 0, nb = 0;
      for (auto gg : G) {
        ng += gg.second;
      }
      for (auto rr : R) {
        nr += rr.second;
      }
      for (auto bb : B) {
        nb += bb.second;
      }

      N nn = ng * nr * nb;
			for(auto gg : G) {
				nn -= R[gg.first] * G[gg.first] * nb;
				nn -= B[gg.first] * G[gg.first] * nr;
			}
			for(auto rr : R) {
				nn -= R[rr.first] * B[rr.first] * ng;
			}
			res += nn;
    }
  }

  std::cout << res << "\n";
}
