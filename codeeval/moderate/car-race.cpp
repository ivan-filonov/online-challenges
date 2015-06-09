/*
 * Common base C++ source for codeeval solutions.
 * */
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

void test();
void process_file(char*);
int main(int argc, char ** argv) {
    if( 0 ) {
        test();
    } else {
        process_file(argv[1]);
    }
	return 0;
}

void process(std::vector<std::string>& v);

void test() {
  std::vector<std::string> data {
    "1.029 115 1.122 125 1.185 100 0.53 110 0.751 95 1.242 85 0.533 85 1.003 120 0.465 110 0.546 125 0.446 90 0.582 70 0.878 45 0.49 30 1.016 130 1.047 140 1.146 75 0.496 85 0.857 125 0.971 0",
    "1 266 8.1 1.4",
    "2 178 8.7 4.8",
    "3 251 8.0 3.4",
    "4 215 6.8 3.8",
    "5 220 5.9 3.3",
    "6 262 4.5 1.5",
    "7 267 5.4 2.6",
    "8 268 7.8 3.8",
    "9 225 4.7 1.8",
    "10 266 4.0 1.9",
  };
  std::vector<std::string> expected {
    "10 241.05",
    "6 244.98",
    "7 247.32",
    "1 254.07",
    "8 258.67",
    "3 273.02",
    "9 283.52",
    "5 298.28",
    "4 309.22",
    "2 375.86",
  };
  process(data);
  for(auto&s:expected) {
    std::cout << s << "\n";
  }
}

void process_file(char* path) {
	std::ifstream stream(path);
  bool mid = false;
  std::vector<std::string> v;
	for(std::string line; std::getline(stream, line); ) {
    v.push_back(line);
	}
  process(v);
}

struct car_t {
  int id;
  double vmax;
  double acc;
  double dec;

  double rtime;

  bool operator < (const car_t& other) const {
    return rtime < other.rtime;
  }
  
  void race(const std::vector<double> &dist, const std::vector<double> &angle);
};

void get_track_info(std::string& s, std::vector<double> &dist, std::vector<double> &angle) {
  std::istringstream ss { s };
  double t_a, t_d;
  while(ss >> t_d >> t_a) {
    dist.push_back(t_d);
    t_a /= 180.0;
    angle.push_back(t_a);
  }
}

void process(std::vector<std::string>& v) {
  auto it = v.begin();

  std::vector<double> dist;
  std::vector<double> angle;
  get_track_info(*it++, dist, angle);
  
  std::vector<car_t> vc;
  for(car_t c; v.end() != it; ++it) {
    std::istringstream ss { *it };
    ss >> c.id >> c.vmax >> c.acc >> c.dec;
    c.vmax /= 3600.0;
    c.acc = c.vmax / c.acc;
    c.dec = c.vmax / c.dec;
    c.race(dist, angle);
    vc.push_back(c);
  }
  std::sort(std::begin(vc), std::end(vc));
  std::cout << std::fixed << std::setprecision(2);
  for(auto &c : vc) {
    std::cout << c.id << ' ' << std::round(c.rtime*100.0)*.01 << "\n";
  }
}

void car_t::race(const std::vector<double> &dist, const std::vector<double> &angle) {
  rtime = 0;
  double v = 0;
  auto d_it = dist.begin(), d_e = dist.end();
  auto a_it = angle.begin();
  for(; d_e != d_it; ++d_it, ++a_it) {
    // part 1 - accelerate to vmax
    double dt = (vmax - v) / acc;
    double dist1 = v * dt + acc * dt * dt * .5;
    rtime += dt;
    // part 3 - decelerate to vmax*(*a_it)
    double ev = vmax * (1.0 - (*a_it));
    dt = (vmax - ev) / dec;
    double dist3 = ev * dt + dec * dt * dt * .5;
    rtime += dt;
    // part 2 - pass at vmax
    rtime += ((*d_it) - dist1 - dist3) / vmax;
    v = ev;
  }
}

