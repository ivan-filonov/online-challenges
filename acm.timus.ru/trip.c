/*
 There is a travel agency in Adelton town on Zanzibar island. It has decided to
offer its clients, besides many other attractions, sightseeing the town. To earn
as much as possible from this attraction, the agency has accepted a shrewd
decision: it is necessary to find the shortest route which begins and ends at
the same place.
 Your task is to write a program which finds such a route. In the town there are
numCrosses crossing points numbered from 1 to numCrosses and numRoads two-way
roads numbered from 1 to numRoads.
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
integers: the number of crossing points numCrosses and the number of roads
numRoads (3 ≤ numCrosses ≤
100; 3 ≤ numRoads ≤ numCrosses · (numCrosses − 1)). Each of the next numRoads
lines describes one road. It
contains 3 integers: the number of its first crossing point a, the number of the
second one b, and the length of the road l (1 ≤ a, b ≤ numCrosses; a ≠ b; 1 ≤ l
≤ 300).
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

----------------------------
Test1 (from task description):

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

Answer is
2 1 3 5
No solution.

Well, it is not exactly the same, but seems to be correct

Test #2
5 6
1 5 1
3 5 100
3 4 2
2 4 1
4 5 10
1 2 1
-1

And the answer is
2 3 4

Test #3
6 7
1 2 1
2 3 1
3 2 1
2 1 1
4 5 2
5 6 2
6 4 2
-1

Answer is
4 6 5

Test #4
2 2
1 2 10
1 2 20
-1

Answer is
No solution.

Test #5
6 9
1 2 1
2 3 100
3 4 1
4 5 100
5 6 1
6 1 100
1 4 5
2 5 5
3 6 5
-1

Answer is
3 4 1 2 5 6

Test #6
5 0
6 1
1 2 3
-1

Answer is
No solution.
No solution.

Test #7
4 4
1 2 10
2 3 1
3 4 1
4 1 1
-1

Answer is
1 4 3 2

And test #8
5 6
1 5 1
3 5 100
3 4 2
2 4 1
4 5 10
1 2 1
-1

Answer is
1 5 4 2
*/
#define _GNU_SOURCE
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Input {
  FILE *source;
};

static int readInt(struct Input *from) {
  int r;
  scanf("%d", &r);
  return r;
}

struct Edge {
  int to;
  int len;
};

struct Vertex {
  int id;
  int numEdges;
  struct Edge edges[128];
  struct Edge *id2edge[128];
};

static int g_numVertices;
static struct Vertex g_vertices[128];
static struct Vertex *g_id2vertex[128];

static void createGraph() {
  g_numVertices = 0;

  memset(g_vertices, 0, sizeof(g_vertices));
  memset(g_id2vertex, 0, sizeof(g_id2vertex));

  for (int i = 0; i < 128; ++i) {
    struct Vertex *v = g_vertices + i;
    v->numEdges = 0;
  }
}

static void freeGraph() {}

static inline struct Vertex *findVertex(const int id) {
  return g_id2vertex[id];
}

static inline struct Edge *findEdge(const struct Vertex *v, const int id) {
  return v->id2edge[id];
}

static int getDist(const int u, const int v) {
  return findEdge(findVertex(u), v)->len;
}

static int lowerBound(int *a, int size, int val) {
  int hi = 0, lo = size - 1;
  while (hi < lo) {
    int mid = hi + (lo - hi) / 2;
    if (a[mid] < val)
      hi = mid + 1;
    else
      lo = mid;
  }
  return hi;
}

struct FrontierItem {
  int dist;
  int id;
};

struct Frontier {
  struct FrontierItem data[128];
  int size;

  char contains[128];
  int id2pos[128];
} f;

static void printFrontier() {
  printf("F{");
  for (int i = 0; i < f.size; ++i) {
    const struct FrontierItem *item = f.data + i;
    const int dist = item->dist;
    const int id = item->id;
    printf(" %d(%d=%d)", dist, id, f.data[f.id2pos[id]].id);
  }
  printf(" }\n");
}

static void createFrontier() { memset(&f, 0, sizeof(f)); }

static void frontierUpdate(const int id, const int newDist) {
  const int itemIdx = f.id2pos[id];

  if (itemIdx < f.size - 1) {
    int lo = itemIdx + 1;
    int hi = f.size;
    int mid = hi;
    while (lo < hi) {
      mid = lo + (hi - lo) / 2;
      const int d = f.data[mid].dist;
      if (d < newDist) {
        hi = mid;
      } else if (d > newDist) {
        lo = mid + 1;
      } else {
        break;
      }
    }

    // [ 0 ... itemIdx ... mid ... size )
    for (int idx = itemIdx; idx < mid; ++idx) {
      f.data[idx] = f.data[idx + 1];
      f.id2pos[f.data[idx].id] = idx;
    }

    f.data[mid].id = id;
    f.data[mid].dist = newDist;
    f.id2pos[id] = mid;
  } else {
    f.data[itemIdx].dist = newDist;
  }
}

static void prepareFrontier(int start) {
  for (int i = 0; i < g_numVertices; ++i) {
    int id = g_vertices[i].id;
    f.data[i].id = id;
    f.contains[id] = 1;
    f.id2pos[id] = i;

    f.data[i].dist = INT_MAX;
  }
  f.size = g_numVertices;

  frontierUpdate(start, 0);
}

static void frontierPop(int *id, int *dist) {
  f.size -= 1;
  struct FrontierItem *item = f.data + f.size;
  f.contains[item->id] = 0;

  *id = item->id;
  *dist = item->dist;
}

static void printGraph() {
  printf("Graph {\n");
  for (int i = 0; i < g_numVertices; ++i) {
    printf(" { id:%d", g_vertices[i].id);
    for (int j = 0; j < g_vertices[i].numEdges; ++j) {
      printf(" { to: %d, len: %d}", g_vertices[i].edges[j].to,
             g_vertices[i].edges[j].len);
    }
    printf(" }\n");
  }
  printf("}//Graph\n");
}

static int g_move[128];

static int buildSolution(const int start, const int goal, int **solution,
                         int *solutionSize) {
  *solution = malloc(sizeof(int) * g_numVertices);
  *solutionSize = 0;
  int result = 0;
  for (int j = goal;;) {
    (*solution)[*solutionSize] = j;
    *solutionSize += 1;
    result += getDist(j, g_move[j]);

    if (j == start)
      break;
    j = g_move[j];
  }
  if (*solutionSize < 3) {
    result = 0;
  }
  return result;
}

static int dijkstra_modified(const int start, const int goal, int **solution,
                             int *solutionSize) {
  *solution = NULL;
  *solutionSize = 0;

  createFrontier();

  int dist[128];
  for (int i = 0; i < g_numVertices; ++i) {
    const int id = g_vertices[i].id;
    dist[id] = i == start ? 0 : INT_MAX;
  }

  g_move[start] = goal;
  g_move[goal] = start;

  prepareFrontier(start);

  while (f.size > 0) {
    int node;
    int nodeDist;
    frontierPop(&node, &nodeDist);

    if (nodeDist == INT_MAX) {
      break;
    }

    if (node == goal) {
      return buildSolution(start, goal, solution, solutionSize);
    }

    struct Vertex *current = findVertex(node);
    for (int i = 0; i < current->numEdges; ++i) {
      struct Edge *e = current->edges + i;
      if (node == start && e->to == goal) {
        continue;
      }
      if (!f.contains[e->to]) {
        continue;
      }

      const int oldDist = dist[e->to];
      const int newDist = nodeDist + e->len;
      if (oldDist < nodeDist + e->len) {
        continue;
      }

      frontierUpdate(e->to, newDist);
      dist[e->to] = newDist;
      g_move[e->to] = node;
    }
  }

  return 0;
}

static int compareVertices(struct Vertex *v1, struct Vertex *v2) {
  return v1->id - v2->id;
}

static int compareEdges(struct Edge *e1, struct Edge *e2) {
  return e1->to - e2->to;
}

static void buildGraphIndices() {
  for (int i = 0; i < g_numVertices; ++i) {
    struct Vertex *v = g_vertices + i;
    g_id2vertex[v->id] = v;
    for (int e = 0; e < v->numEdges; ++e) {
      struct Edge *c = v->edges + e;
      v->id2edge[c->to] = c;
    }
  }
}

static void sortGraphItems() {
  qsort(g_vertices, g_numVertices, sizeof(struct Vertex), compareVertices);
  for (int i = 0; i < g_numVertices; ++i) {
    qsort(g_vertices[i].edges, g_vertices[i].numEdges, sizeof(struct Edge),
          compareEdges);
  }
  buildGraphIndices();
}

static int reduceGraph() {
  int result = 0;
  int *deleted = calloc(128, sizeof(int));
  int num_deleted = 0;
  for (int i = 0; i < g_numVertices; ++i) {
    struct Vertex *del = g_vertices + i;
    if (del->numEdges > 1) {
      continue;
    }
    deleted[del->id] = 1;
    num_deleted += 1;

    g_numVertices -= 1;

    *del = g_vertices[g_numVertices];

    result = 1;
  }

  if (!num_deleted) {
    goto cleanup;
  }

  for (int i = 0; i < g_numVertices; ++i) {
    struct Vertex *v = g_vertices + i;
    for (int j = 0; j < v->numEdges; ++j) {
      if (!deleted[v->edges[j].to]) {
        continue;
      }
      v->numEdges -= 1;
      v->edges[j] = v->edges[v->numEdges];
    }
  }

  sortGraphItems();

cleanup:
  free(deleted);
  return result;
}

static void removeVertex(const int id) {
  struct Vertex *v = findVertex(id);
  const int vne = v->numEdges;
  const int alloc_size = vne * sizeof(int);
  int *edges = malloc(alloc_size);
  for (int i = 0; i < vne; ++i) {
    edges[i] = v->edges[i].to;
  }

  memmove(v, v + 1,
          (g_numVertices - (v - g_vertices) - 1) * sizeof(struct Vertex));
  g_numVertices -= 1;

  for (int i = 0; i < vne; ++i) {
    struct Vertex *cv = findVertex(edges[i]);
    struct Edge *edge = findEdge(cv, id);

    memmove(edge, edge + 1,
            (cv->numEdges - (edge - cv->edges) - 1) * sizeof(struct Edge));
  }

  free(edges);
}

static int solve(int **solution, int *solutionSize) {
  int slen = INT_MAX;
  *solution = NULL;
  *solutionSize = 0;

  for (; 0;) {
    if (!reduceGraph()) {
      break;
    }
  }
  if (g_numVertices == 0) {
    return -1;
  }

  for (int u = 0; u < g_numVertices; ++u) {
    struct Vertex *v1 = g_vertices + u;
    for (int v = 0; v < v1->numEdges; ++v) {
      struct Edge *e = v1->edges + v;
      if (v1->id > e->to) {
        continue;
      }

      int *dsol;
      int dss;
      int dl;
      dl = dijkstra_modified(v1->id, e->to, &dsol, &dss);
      if (dl) {
        if (dl < slen) {
          if (*solution) {
            free(*solution);
          }
          *solution = dsol;
          *solutionSize = dss;
          slen = dl;
        } else {
          free(dsol);
        }
      }
    }
  }
  return slen == INT_MAX ? -1 : slen;
}

static void addEdge(const int u, const int v, const int len) {
  struct Vertex *pv;
  if (g_id2vertex[u] == NULL) {
    pv = g_vertices + g_numVertices;
    g_numVertices += 1;
    pv->id = u;
    g_id2vertex[u] = pv;
  } else {
    pv = g_id2vertex[u];
  }

  if (pv->id2edge[v] == NULL) {
    struct Edge *edge = pv->edges + pv->numEdges;
    pv->numEdges += 1;
    edge->to = v;
    edge->len = len;
    pv->id2edge[v] = edge;
  } else {
    struct Edge *edge = pv->id2edge[v];
    if (edge->len > len) {
      edge->len = len;
    }
  }
}

static void readGraph(struct Input *input) {
  for (int i = 0; i < 128; ++i) {
    g_id2vertex[i] = NULL;
  }

  g_numVertices = readInt(input);
  if (g_numVertices == -1) {
    return;
  }

  const int numRoads = readInt(input);

  createGraph();

  for (int i = 0; i < numRoads; ++i) {
    int u = readInt(input) - 1;
    int v = readInt(input) - 1;
    int dist = readInt(input);
    if (u > v) {
      int t = u;
      u = v;
      v = t;
    }

    addEdge(u, v, dist);
    addEdge(v, u, dist);
  }
  sortGraphItems();
}

int main() {
  struct Input input = {
      .source = stdin,
  };

  for (;;) {
    readGraph(&input);
    if (g_numVertices == -1) {
      break;
    }

    int *solution;
    int solutionSize;
    int r = solve(&solution, &solutionSize);

    if (-1 == r) {
      printf("No solution.\n");
    } else {
      char buf[4096];
      char *ptr = buf;
      while (solutionSize-- > 0) {
        sprintf(ptr, "%d%s", solution[solutionSize] + 1,
                solutionSize ? " " : "");
        ptr += strlen(ptr);
      }
      printf("%s\n", buf);

      free(solution);
    }

    freeGraph();
  }
}
