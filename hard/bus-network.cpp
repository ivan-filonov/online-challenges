#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

namespace {
#ifdef TEST
  void test();
#endif //#ifdef TEST
  void process_file(char*);
}

int main(int argc, char ** argv) {
#ifdef TEST
  test();
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

namespace {
  using std::string;
  template<typename V> using vector = std::vector<V>;
  template<typename T> using shared_ptr = std::shared_ptr<T>;
  using std::make_shared;

  const int MAX = 1073741823;

  struct pt {
    int route;
    int id;
    int dist;

    pt(int _route, int _id) : dist(MAX), route(_route), id(_id) {}

    bool operator < (const pt & other) const {
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

    vector<vector<int>> routes;
    vector<pt> points;
    while(ss.ignore(MAX, '[').tellg() > 0) {
      const int r_id = routes.size();
      routes.emplace_back();
      auto & v = routes.back();

      for(char c = ','; ',' == c;) {
        v.push_back(0);
        ss >> v.back() >> c;

        points.emplace_back(r_id, v.back());
      }
    }

    for(auto & p : points) {
      if(p.id == A) {
        p.dist = 0;
      }
    }

    for(auto it = points.begin(); it != points.end(); ++it) {
      std::sort(it, points.end());

      auto & r = routes[it->route];
      size_t it_idx;
      for(it_idx = 0; it_idx < r.size(); ++it_idx) {
        if(r[it_idx] == it->id) {
          break;
        }
      }
      const int i1 = it_idx ? r[it_idx - 1] : -1;
      const int i2 = (r.size() - it_idx > 1) ? r[it_idx + 1] : -1;

      for(auto it2 = it + 1; it2 != points.end(); ++it2) {
        // 12-minute edges:
        if(it->id == it2->id) {
          it2->dist = std::min(it2->dist, it->dist + 12);
        }
        // 7-minute edges:
        if(it->route == it2->route) {
          if(i1 == it2->id || i2 == it2->id) {
            it2->dist = std::min(it2->dist, it->dist + 7);
          }
        }
      }
    }

    int d = MAX;
    for(auto & p : points) {
      if(p.id == B) {
        d = std::min(d, p.dist);
      }
    }

    return d < MAX ? std::to_string(d): "None";
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
}
