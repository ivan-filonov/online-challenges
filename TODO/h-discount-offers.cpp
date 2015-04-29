#include <algorithm>
#include <fstream>
#include <iostream>
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

using std::string;
template<typename V> using vector = std::vector<V>;

const string vowels { "AEIOUYaeiouy" };
float score(const string & prod, const string & cust) {
  if(prod.empty() || cust.empty()) {
    return 0;
  }
  int nlep = 0, nlec = 0, nvc = 0;
  float r = 0;
  for(auto c : prod) {
    if(std::isalpha(c)) {
      ++nlep;
    }
  }
  for(auto c : cust) {
    if(std::isalpha(c)) {
      ++nlec;
      if(vowels.find(c) != string::npos) {
        ++nvc;
      }
    }
  }
  if(0 == nlep%2) {
    r = 1.5 * nvc;
  } else {
    r = (nlec - nvc);
  }
  for(int i = 2, j = std::min(prod.length(), cust.length()); i < j; ++i) {
    if((0 == nlep % i) && ( 0 == nlec % i)) {
      r *= 1.5;
      break;
    }
  }
  return r;
}

void process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
//    std::istringstream ss { line };
  size_t pos = 0;
  bool goods = false;
  vector<string> vc, vp;
  while(line.length() != pos) {
    size_t d = pos + 1;
    while(line.length() != d && line[d] != ',' && line[d] != ';') {
      ++d;
    }
    (goods ? vp : vc).emplace_back(line.substr(pos, d - pos));
    if(!goods) {
      goods |= ';' == line[d];
    }
    if(d == line.length()) {
      pos = d;
    } else {
      pos = d + 1;
    }
  }
  const int mi = std::min(vc.size(), vp.size());
  if(vc.size() < vp.size()) {
    vc.resize(vp.size());
  } else {
    vp.resize(vc.size());
  }
  const int wid = vc.size();
  vector<float> m(wid * wid, 0.0f);
  int di = 0;
  for(auto & ic : vc) {
    for(auto & ip : vp) {
      m[di++] = score(ip, ic);
    }
  }
  
  vector<int> p(wid);
  for(int i = 0; i != wid; ++i) {
    p[i] = i;
  }
  float r = 0;
  for(;;) {
    float s = 0;
    for(int i = 0; i != wid; ++i) {
      s += m[i * wid + p[i]];
    }
    r = std::max(r, s);
    if(!std::next_permutation(p.begin(), p.end())) {
      break;
    }
  }
  std::cout.setf(std::cout.fixed);
  std::cout.precision(2);
  std::cout << r << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "Jack Abraham,John Evans,Ted Dziuba;iPad 2 - 4-pack,Girl Scouts Thin Mints,Nerf Crossbow",
    "Jeffery Lebowski,Walter Sobchak,Theodore Donald Kerabatsos,Peter Gibbons,Michael Bolton,Samir Nagheenanajar;Half & Half,Colt M1911A1,16lb bowling ball,Red Swingline Stapler,Printer paper,Vibe Magazine Subscriptions - 40 pack",
    "Jareau Wade,Rob Eroh,Mahmoud Abdelkader,Wenyi Cai,Justin Van Winkle,Gabriel Sinkin,Aaron Adelson;Batman No. 1,Football - Official Size,Bass Amplifying Headphones,Elephant food - 1024 lbs,Three Wolf One Moon T-shirt,Dom Perignon 2000 Vintage",
  };
  vector<string> v_expect {
    "21.00",
    "83.50",
    "71.25",
  };
  for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
    process(v_test[i]);
    std::cout << v_expect[i] << "\n";
  }
}
#endif //#ifdef TEST

void process_file(char* path) {
  std::ifstream stream(path);
  for(string line; std::getline(stream, line); ) {
    process(line);
  }
}
