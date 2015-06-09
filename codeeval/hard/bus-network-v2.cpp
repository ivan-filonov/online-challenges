#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

const int MAX = 1000000000;

#ifdef TEST
void test();
#endif //#ifdef TEST
void process_file(char*);

int main(int argc, char ** argv) {
#ifdef TEST
  test();
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

struct point {
  point * prev;
  point * next;
  point * sprev;
  point * snext;
  int route;
  int id;
  int dist;

  point(int _route, int _id) : route(_route), id(_id), dist(MAX),
    prev(nullptr), next(nullptr), sprev(nullptr), snext(nullptr) {}

  bool operator < (const point &other) const {
    return dist < other.dist;
  }
};

string process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  std::istringstream ss { line };
  ss.ignore(MAX, '(');
  int A, B;
  ss >> A;
  ss.ignore(MAX, ',');
  ss >> B;
 
  if(A == B) {
    return "0";
  }

  vector<point> points;

  int cur_route = 0;
  while(ss.ignore(MAX, '[').tellg() > 0) {
    vector<int> v;
    for(char c = ','; ',' == c;) {
      int id;
      ss >> id >> c;
      v.push_back(id);
      // cur_route, id - текущая точка
      points.emplace_back(cur_route, id);
      if(A == id) {
        points.back().dist = 0;
      }
    }
    ++cur_route;
  }

  // now init graph links
  for(size_t idx = 1; idx != points.size(); ++idx) {
    auto & pt = points[idx];
    auto & prev = points[idx - 1];
    // 1. link points in one route
    if(pt.route == prev.route) {
      prev.next = &points[idx];
      pt.prev = &points[idx-1];
    }
    // 2. link points by id
    for(auto jdx = 0; jdx != idx; ++jdx) {
      auto & rpt = points[jdx];
      if(rpt.route != pt.route && rpt.id == pt.id) {
        pt.snext = rpt.snext;
        pt.sprev = &points[jdx];
        rpt.snext = &points[idx];
        if(nullptr != pt.snext) {
          pt.snext->sprev = &points[idx];
        }
        break;
      }
    }
  }

  vector<point*> border;
  for(auto & pt : points) {
    if(!pt.dist) {
      border.push_back(&pt);
    }
  }

  while(!border.empty()) {
    std::sort(border.begin(), border.end(), [](const point * a, const point * b){ return a->dist > b->dist;});
    auto & pt = *border.back();
    border.pop_back();
    
    const auto d7 = pt.dist + 7;
    const auto d12 = pt.dist + 12;

    auto p = pt.prev;
    if(nullptr != p && p->dist > d7) {
      p->dist = d7;
      border.push_back(p);
    }

    p = pt.next;
    if(nullptr != p && p->dist > d7) {
      p->dist = d7;
      border.push_back(p);
    }

    for(auto p = pt.sprev; nullptr != p; p = p->sprev) {
      if(p->dist > d12) {
        p->dist = d12;
        border.push_back(p);
      }
    }

    for(auto p = pt.snext; nullptr != p; p = p->snext) {
      if(p->dist > d12) {
        p->dist = d12;
        border.push_back(p);
      }
    }
  }

  int res = MAX;
  for(auto &p : points) {
    if(p.id == B) {
      res = std::min(res, p.dist);
    }
  }

  return res < MAX ? std::to_string(res) : "None";
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "(2,4); R1=[1,2,3,11,12,4]; R2=[5,6,4]; R3=[1,6,7]; R4=[5,6,4]; R5=[8,6,3]",
    "(1,7); R1=[1,2,3,4]; R2=[5,6,4]; R3=[9,6,7]; R4=[12,1,2,3,11,16,15,14,10,13,7]",
    "(3,299); R1=[1,2,3,4]; R2=[6,7,19,12,4]; R3=[11,14,16,6]; R4=[24,299,42,6]",
    "(3,4); R1=[1,2,3]; R2=[6,7,19,12,4]; R3=[11,14,16,6]",
  };
  vector<string> v_expect {
    "28",
    "59",
    "73",
    "None",
  };
  for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
    std::cout << process(v_test[i]) << "\n";
    std::cout << v_expect[i] << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char* path) {
  std::ifstream stream(path);
  for(string line; std::getline(stream, line); ) {
    std::cout << process(line) << "\n";
  }
}
