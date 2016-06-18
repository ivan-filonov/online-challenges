#include <iostream>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

int main() {
#ifdef ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::istringstream cin{
      "7\nIsenbaev Oparin Toropov\nAyzenshteyn Oparin Samsonov\nAyzenshteyn "
      "Chevdar Samsonov\nFominykh Isenbaev Oparin\nDublennykh Fominykh "
      "Ivankov\nBurmistrov Dublennykh Kurpilyanskiy\nCormen Leiserson Rivest"};
#endif
  int numTeams;
  {
    std::string t;
    std::getline(cin, t);
    numTeams = std::stoi(t);
  }

  std::unordered_map<std::string, std::unordered_set<std::string>> g;
  std::set<std::string> names;
  {
    int team = 0;
    for (std::string tl; std::getline(cin, tl) && team < numTeams; ++team) {
      std::string tn = "#" + std::to_string(team);
      std::istringstream ts{tl};
      for (std::string n; ts >> n;) {
        g[n].insert(tn);
        g[tn].insert(n);
        names.insert(n);
      }
    }
  }

  const char *ise = "Isenbaev";
  std::set<std::pair<int, std::string>> frontier{{0, ise}};
  std::unordered_map<std::string, int> dist{{ise, 0}};
  std::unordered_set<std::string> visited{ise};
  while (!frontier.empty()) {
    auto p = *frontier.begin();
    frontier.erase(frontier.begin());
    for (auto &name : g[p.second]) {
      if (visited.count(name)) {
        continue;
      }
      int edge = '#' == name[0] ? 1 : 0;
      int newDist = dist[p.second] + edge;
      if (!dist.count(name) || dist[name] > newDist) {
        frontier.erase(std::make_pair(dist[name], name));
        dist[name] = newDist;
        frontier.emplace(dist[name], name);
      }
      visited.insert(name);
    }
  }

  for (auto &n : names) {
    std::cout << n << " ";
    if (dist.count(n)) {
      std::cout << dist[n];
    } else {
      std::cout << "undefined";
    }
    std::cout << "\n";
  }

  return 0;
}
