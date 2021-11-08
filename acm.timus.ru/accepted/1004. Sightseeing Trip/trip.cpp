#include <algorithm>
#include <climits>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

static const int INF = INT_MAX / 4;

static std::vector<std::vector<int>> graph;

static bool readGraph (std::istream &stream)
{
  int points;
  stream >> points;
  if (-1 == points) {
    return false;
  }

  int edges;
  stream >> edges;

  graph.assign (points, std::vector<int> (points, INF));

  for (int i = 0; i < edges; ++i) {
    int u, v, d;
    stream >> u >> v >> d;
    u -= 1;
    v -= 1;

    if (d < graph[u][v]) {
      graph[u][v] = graph[v][u] = d;
    }
  }

  return true;
}

static std::vector<std::vector<int>> routes;

static bool find_best_edge (int &best_u, int &best_v)
{
  const int N = graph.size ();
  int       best_dist = INF;
  best_u = best_v = 0;

  for (int i = 0; i < N; ++i) {
    routes[i][i] = 0;

    // попытаться обновить лучший цикл
    for (int j = 0; j < i; ++j) {
      for (int k = 0; k < i; ++k) {
        if (j == k) {
          continue;
        }
        const int new_dist = graph[i][j] + graph[i][k] + routes[j][k];
        if (new_dist < best_dist) {
          best_dist = new_dist;
          best_u = i;
          best_v = j;
        }
      }
    }

    for (int j = 0; j < i; ++j) {
      // попытаться добавить ребро к пути
      routes[i][j] = INF;
      for (int k = 0; k < i; ++k) {
        if (routes[i][j] <= routes[j][k] + graph[k][i]) {
          continue;
        }
        routes[i][j] = routes[j][k] + graph[k][i];
      }
      routes[j][i] = routes[i][j];
    }

    for (int j = 0; j < i; ++j) {
      // попытаться склеить 2 пути
      for (int k = 0; k < i; ++k) {
        if (routes[j][k] <= routes[j][i] + routes[i][k]) {
          continue;
        }
        routes[j][k] = routes[j][i] + routes[i][k];
      }
    }
  }
  return INF != best_dist;
}

static void floyd_warshall_on_routes ()
{
  const int N = graph.size ();
  for (int i = 0; i < N; ++i) {
    routes[i][i] = 0;
  }

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      for (int k = 0; k < N; ++k) {
        if (routes[j][k] <= routes[j][i] + routes[i][k]) {
          continue;
        }
        routes[j][k] = routes[j][i] + routes[i][k];
      }
    }
  }
}

static bool solve (std::vector<int> &solution)
{
  const int N = graph.size ();
  routes.assign (N, std::vector<int> (N, INF));

  int u, v;
  if (!find_best_edge (u, v)) {
    return false;
  }

  graph[u][v] = graph[v][u] = INF;
  routes = graph;

  floyd_warshall_on_routes ();

  solution.clear ();
  while (u != v) {
    solution.push_back (v + 1);
    for (int i = 0; i < N; ++i) {
      if (routes[v][u] == routes[i][u] + graph[v][i]) {
        v = i;
        break;
      }
    }
  }
  solution.push_back (u + 1);

  return true;
}

int main ()
{
#if ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio (false);
#else
  std::fstream tf ("test");
  auto &       cin = tf;
#endif
  for (;;) {
    if (!readGraph (cin)) {
      break;
    }

    std::ostringstream oss;
    std::vector<int>   solution;
    if (!solve (solution)) {
      oss << "No solution.";
    } else {
      bool flag = false;
      for (int i : solution) {
        oss << (flag ? " " : "") << i;
        flag = true;
      }
    }
    std::cout << oss.str () << "\n";
  }
}
