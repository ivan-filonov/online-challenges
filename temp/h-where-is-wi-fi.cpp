#include <fstream>
#include <iostream>
#include <cmath>
#include <set>
#include <sstream>
#include <vector>

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

using namespace std;

bool buildings = true;

struct building {
  string name;
  vector<double> x, y;

  bool hit_test(double hx, double hy) {
    auto minx = x[0];
    auto maxx = x[0];
    auto miny = y[0];
    auto maxy = y[0];
    for(auto &_x : x) {
      minx = min(minx, _x);
      maxx = max(maxx, _x);
    }
    for(auto &_y : y) {
      miny = min(miny, _y);
      maxy = max(maxy, _y);
    }
    if(hx < minx || hx > maxx || hy < miny || hy > maxy) {
      return false;
    }
    int num_right = 0;
    for(size_t ei = 0; ei != x.size(); ++ei) {
      double ex[2] = { x[ei], x[(ei + 1) % x.size()] };
      double ey[2] = { y[ei], y[(ei + 1) % y.size()] };
      if(hy < min(ey[0], ey[1]) || hy > max(ey[0], ey[1])) {
        continue;
      }
      ex[1] -= ex[0];
      ey[1] -= ey[0];
      auto vx = hx - ex[0];
      auto vy = hy - ey[0];

      {
        // 1. hit test line
        auto lhex = ex[1];
        auto lhey = ey[1];
        auto lhvx = vx;
        auto lhvy = vy;
        if(fabs(lhex) < fabs(lhey)) {
          swap(lhex, lhey);
          swap(lhvx, lhvy);
        }
        auto lhk = lhvx / lhex;
        if(fabs(lhvy - lhk * lhey) < 1e-6) {
          // point is exaclty on building border
          return true;
        }
      }
      // 2. is point left or right from the line
      if(fabs(ey[1]) < 1e-6) {
        // horizontal edge
      } else {
        // non-horizontal edge
        auto tx = vy / ey[1] * ex[1];
        if(tx > vx) {
          ++num_right;
        }
      }
    }
    return num_right % 2 > 0;
  }
};

vector<building> vb;

// NAME x;y ...
void process_building(string line) {
  std::istringstream ss { line };
  vb.emplace_back();
  std::getline(ss, vb.back().name, ' ');
  for(string tx, ty; std::getline(ss, tx, ';') && std::getline(ss, ty, ' ');) {
    vb.back().x.push_back(std::stod(tx));
    vb.back().y.push_back(std::stod(ty));
  }
}

struct spot {
  string MAC;
  vector<double> x;
  vector<double> y;
  vector<double> dir;
};

vector<spot> vs;

// coords MAX;asimut
void process_point(string line) {
  std::istringstream ss { line };
  string t;
  std::getline(ss, t, ';');
  auto x = std::stod(t);
  std::getline(ss, t, ' ');
  auto y = std::stod(t);
  for(string mac, dir; std::getline(ss, mac, ';') && std::getline(ss, dir, ' ');) {
    auto d = std::stod(dir);
    spot * s = nullptr;
    for(auto & sp : vs) {
      if(sp.MAC == mac) {
        s = &sp;
        break;
      }
    }
    if(nullptr == s) {
      vs.emplace_back();
      s = &vs.back();
      s->MAC = mac;
    }
    s->x.push_back(x);
    s->y.push_back(y);
    s->dir.push_back(90 - d);
  }
}

void process(string line) {
  if(line.empty()) {
    buildings = false;
  } else {
    if(buildings) {
      process_building(line);
    } else {
      process_point(line);
    }
  }
}

void print_results() {
  std::set<string> res;
  for(auto & s : vs) {
    if(s.x.size() < 2) {
      continue;
    }
    double c = 0;
    double ax = 0;
    double ay = 0;
    for(size_t i = 1; i != s.x.size(); ++i) {
      auto x1 = s.x[i-1];
      auto y1 = s.y[i-1];
      auto d1 = s.dir[i-1] * M_PI / 180.0;
      auto x2 = s.x[i];
      auto y2 = s.y[i];
      auto d2 = s.dir[i] * M_PI / 180.0;

      // x1 + t1 * cos(d1) = x2 + t2 * cos(d2)
      // y1 + t1 * sin(d1) = y2 + t2 * sin(d2)
      //
      // x1 * sin(d1) + t1 * sin(d1) * cos(d1) = x2 * sin(d1) + t2 * sin(d1) * cos(d2)
      // y1 * cos(d1) + t1 * sin(d1) * cos(d1) = y2 * cos(d1) + t2 * sin(d2) * cos(d1)
      //
      // x1 * sin(d1) - y1 * cos(d1)  = x2 * sin(d1) + t2 * sin(d1) * cos(d2) - y2 * cos(d1) - t2 * sin(d2) * cos(d1)
      //
      // t2 * sin(d1) * cos(d2) - t2 * sin(d2) * cos(d1) = x1 * sin(d1) - y1 * cos(d1) - x2 * sin(d1) + y2 * cos(d1)
      //
      auto k2 = (sin(d1) * cos(d2) - sin(d2) * cos(d1));
      if(fabs(k2) < 1e-6) {
        continue;
      }
      auto t2 = (x1 * sin(d1) - y1 * cos(d1) - x2 * sin(d1) + y2 * cos(d1)) / k2;
      x2 += t2 * cos(d2);
      y2 += t2 * sin(d2);

      c += 1.0;
      ax += x2;
      ay += y2;
    }
    ax /= c;
    ay /= c;
    // now find building at (ax,ay)
    for(auto & b : vb) {
      if(b.hit_test(ax, ay)) {
        res.insert(b.name);
      }
    }
  }
  for(auto & s : res) {
    std::cout << s << "\n";
  }
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "B001 25.19;16.10 25.19;40.93 35.83;40.93 35.83;23.19 42.93;23.19 42.93;16.10 25.19;16.10",
    "B002 25.19;40.93 25.19;51.57 60.67;51.57 60.67;33.83 50.02;33.83 50.02;40.93 25.19;40.93",
    "B003 50.02;33.83 60.67;33.83 60.67;16.10 42.93;16.10 42.93;23.19 50.02;23.19 50.02;33.83",
    "B004 25.19;58.67 25.19;79.95 35.83;79.95 35.83;58.67 25.19;58.67",
    "B005 42.93;79.95 60.67;79.95 60.67;72.86 50.02;72.86 50.02;65.76 60.67;65.76 60.67;58.67 42.93;58.67 42.93;79.95",
    "B006 71.31;69.31 81.95;79.95 103.24;58.67 92.60;48.02 85.50;55.12 92.60;62.21 85.50;69.31 78.41;62.21 71.31;69.31",
    "B007 74.86;44.48 81.95;44.48 89.05;37.38 89.05;33.83 81.95;33.83 81.95;26.74 89.05;26.74 89.05;23.19 81.95;16.10 74.86;16.10 74.86;44.48",
    "B008 99.69;40.93 110.33;51.57 117.43;44.48 106.79;33.83 99.69;40.93",
    "B009 96.14;23.19 110.33;23.19 110.33;16.10 96.14;16.10 96.14;23.19",
    "B010 74.86;58.67 81.95;58.67 81.95;51.57 74.86;51.57 74.86;58.67",
    "",
    "67.76;12.55 7a-a1-1d-86-68-65;306.8699 78-d0-54-69-14-98;303.0239 ef-4a-98-9c-7e-8e;330.2551 d7-0e-15-4e-23-9c;41.4965 e1-89-0b-c8-75-53;80.9097",
    "53.57;12.55 7a-a1-1d-86-68-65;41.6335 78-d0-54-69-14-98;0.0000 ef-4a-98-9c-7e-8e;0.0000 d7-0e-15-4e-23-9c;58.8407",
    "39.38;12.55 7a-a1-1d-86-68-65;72.1811 78-d0-54-69-14-98;56.9761 ef-4a-98-9c-7e-8e;29.7449",
    "25.19;12.55 7a-a1-1d-86-68-65;79.3803 78-d0-54-69-14-98;71.9958",
    "21.64;23.19 78-d0-54-69-14-98;92.5448 ef-4a-98-9c-7e-8e;66.0375",
    "21.64;37.38 78-d0-54-69-14-98;116.0535 ef-4a-98-9c-7e-8e;90.0000",
    "21.64;51.57 7c-bc-27-bc-af-08;14.4208 ef-4a-98-9c-7e-8e;113.9625",
    "21.64;65.76 7c-bc-27-bc-af-08;30.9638",
    "21.64;79.95 7c-bc-27-bc-af-08;119.0546",
    "35.83;83.50 7c-bc-27-bc-af-08;227.7263",
    "50.02;83.50 ad-18-c5-28-4d-3c;108.4349 7c-bc-27-bc-af-08;252.1213",
    "64.21;83.50 ad-18-c5-28-4d-3c;120.9638",
    "78.41;83.50 ad-18-c5-28-4d-3c;161.5651",
    "92.60;83.50 ad-18-c5-28-4d-3c;225.0000",
    "103.24;72.86 ad-18-c5-28-4d-3c;270.0000",
    "110.33;65.76 ad-18-c5-28-4d-3c;284.0362 13-31-a3-a5-da-af;206.5651",
    "120.98;55.12 13-31-a3-a5-da-af;234.4623",
    "120.98;40.93 13-31-a3-a5-da-af;261.8699 e1-89-0b-c8-75-53;217.9987",
    "120.98;26.74 13-31-a3-a5-da-af;293.1986 e1-89-0b-c8-75-53;244.3590",
    "120.98;12.55 13-31-a3-a5-da-af;315.0000 e1-89-0b-c8-75-53;287.7447",
    "106.79;12.55 13-31-a3-a5-da-af;336.8014 d7-0e-15-4e-23-9c;309.0939 e1-89-0b-c8-75-53;327.9946",
    "92.60;12.55 13-31-a3-a5-da-af;8.1301 7a-a1-1d-86-68-65;280.8403 d7-0e-15-4e-23-9c;335.2249 e1-89-0b-c8-75-53;61.9275",
    "78.41;12.55 13-31-a3-a5-da-af;35.5377 7a-a1-1d-86-68-65;288.4349 78-d0-54-69-14-98;290.3764 ef-4a-98-9c-7e-8e;315.0000 d7-0e-15-4e-23-9c;17.1027 e1-89-0b-c8-75-53;77.1250",
    "67.76;16.10 13-31-a3-a5-da-af;53.1301 7a-a1-1d-86-68-65;288.4349 78-d0-54-69-14-98;291.8014 ef-4a-98-9c-7e-8e;326.3099 d7-0e-15-4e-23-9c;47.6026 e1-89-0b-c8-75-53;86.5664",
    "67.76;30.29 13-31-a3-a5-da-af;75.9638 7a-a1-1d-86-68-65;216.8699 78-d0-54-69-14-98;239.0362 ef-4a-98-9c-7e-8e;296.5651 d7-0e-15-4e-23-9c;87.5104",
    "67.76;44.48 ad-18-c5-28-4d-3c;26.5651 13-31-a3-a5-da-af;104.0362 7a-a1-1d-86-68-65;198.4349 78-d0-54-69-14-98;212.0054 ef-4a-98-9c-7e-8e;243.4349 d7-0e-15-4e-23-9c;129.5597",
    "67.76;58.67 ad-18-c5-28-4d-3c;45.0000 13-31-a3-a5-da-af;126.8699 ef-4a-98-9c-7e-8e;213.6901 d7-0e-15-4e-23-9c;149.4703",
    "67.76;72.86 ad-18-c5-28-4d-3c;90.0000",
  };
  for(auto & s : v_test) {
    process(s);
  }
  print_results();
  vector<string> v_expect {
    "B003",
    "B005",
    "B007",
  };
  std::cout << "--------------------------------------------------\n";
  for(auto & s : v_expect) {
    std::cout << s << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char* path) {
  std::ifstream stream(path);
  for(string line; std::getline(stream, line); ) {
    process(line);
  }
  print_results();
}
