#include <algorithm>
#include <climits>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

static const int INF = INT_MAX / 4;

// граф
static int g_nverts;
// список вертексов
static int g_vlist[128];
// есть ли вертекс
static bool g_have_vert[128];
// сколько у вертекса ребер
static int g_vertex_nedges[128];
// списки ребер по вертексам
static int g_elist[128][128];
// есть ли ребро у вертекса
static bool g_have_edge[128][128];
// длинны ребер
static int g_edges[128][128];

static void g_addv(int u) {
  if (g_have_vert[u]) {
    return;
  }

  g_vlist[g_nverts] = u;
  g_have_vert[u] = true;
  g_nverts += 1;
}

static void g_adde(int u, int v, int len) {
  g_addv(u);
  g_addv(v);

  if (g_have_edge[u][v]) {
    g_edges[u][v] = std::min(g_edges[u][v], len);
    return;
  }

  g_have_edge[u][v] = true;
  g_edges[u][v] = len;
  g_elist[u][g_vertex_nedges[u]] = v;
  g_vertex_nedges[u] += 1;
}

static bool g_read(std::istream &cin) {
  g_nverts = 0;
  std::memset(g_vlist, 0, sizeof(g_vlist));
  std::memset(g_have_vert, 0, sizeof(g_have_vert));
  std::memset(g_vertex_nedges, 0, sizeof(g_vertex_nedges));
  std::memset(g_elist, 0, sizeof(g_elist));
  std::memset(g_have_edge, 0, sizeof(g_have_edge));
  std::memset(g_edges, 0, sizeof(g_edges));

  int nverts;
  int nedges;

  cin >> nverts;
  if (-1 == nverts) {
    return false;
  }

  cin >> nedges;

  for (int i = 0; i < nedges; ++i) {
    int u, v, len;
    cin >> u >> v >> len;
    g_adde(u, v, len);
    g_adde(v, u, len);
  }
  return true;
}

// решение
// длинна пути по циклу
static int s_dist;
// сколько в цикле вершин
static int s_len;
// сам путь
static int s_points[128];

// вариант решения
static int s_move[128];

static int s_calc_dist(const int u, const int v) {
  int sum = g_edges[u][v];
  for (int cur = u; cur != v;) {
    const int next = s_move[cur];
    sum += g_edges[cur][next];
    cur = next;
  }
  return sum;
}

static int f_size;
static int f_list[128];
static int f_dist[128];
static bool f_checked[128];

static void update_edges(const int u, const int v, const int node,
                         const int ndist) {
  for (int n2 = 0, ne = g_vertex_nedges[node]; n2 < ne; ++n2) {
    const int node2 = g_elist[node][n2];
    if (node == u && node2 == v) {
      continue;
    }

    if (f_checked[node2]) {
      continue;
    }

    const int new_dist = ndist + g_edges[node][node2];

    bool add = true;
    for (int i = 0; i < f_size; ++i) {
      if (f_list[i] == node2) {
        add = false;
        if (new_dist < f_dist[i]) {
          f_dist[i] = new_dist;
          s_move[node2] = node;
        }
      }
    }
    if (add) {
      f_list[f_size] = node2;
      f_dist[f_size] = new_dist;
      f_size += 1;
      s_move[node2] = node;
    }
  }
}

static bool trace(const int u, const int v) {
  s_move[u] = v;
  s_move[v] = u;

  f_size = 1;
  f_list[0] = u;
  f_dist[0] = 0;

  std::memset(f_checked, 0, sizeof(f_checked));

  while (f_size > 0) {
    int min = 0;
    for (int i = 0; i < f_size; ++i) {
      if (f_dist[i] < f_dist[min]) {
        min = i;
      }
    }

    const int node = f_list[min];
    const int ndist = f_dist[min];

    f_size -= 1;
    std::swap(f_list[min], f_list[f_size]);
    std::swap(f_dist[min], f_dist[f_size]);

    f_checked[node] = true;

    if (node == v) {
      return true;
    }

    update_edges(u, v, node, ndist);
  }

  return false;
}

static bool solve() {
  s_dist = 0;
  for (int ui = 0; ui < g_nverts; ++ui) {
    const int u = g_vlist[ui];
    for (int vi = 0, ve = g_vertex_nedges[u]; vi < ve; ++vi) {
      const int v = g_elist[u][vi];
      if (u > v) {
        continue;
      }

      if (!trace(u, v)) {
        continue;
      }

      // s_points - решение
      if (s_dist && s_calc_dist(u, v) >= s_dist) {
        continue;
      }

      s_len = 0;
      s_dist = 0;
      for (int cur = v;; cur = s_move[cur]) {
        s_points[s_len] = cur;
        s_dist += g_edges[cur][s_move[cur]];
        s_len += 1;
        if (u == cur) {
          break;
        }
      }
    }
  }
  return s_dist != 0;
}

int main() {
#if ONLINE_JUDGE
  using std::cin;
  cin.sync_with_stdio(false);
#else
  std::fstream tf("test2");
  auto &cin = tf;
#endif
  for (;;) {
    if (!g_read(cin)) {
      break;
    }

    std::ostringstream os;
    if (!solve()) {
      os << "No solution.";
    } else {
      for (int i = 0; i < s_len; ++i) {
        os << (i ? " " : "") << s_points[i];
      }
    }
    std::cout << os.str() << "\n";
  }
}
