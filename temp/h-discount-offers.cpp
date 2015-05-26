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

struct cust_t {
  size_t sid;
  int vowels;
  int consonants;
  
  void set(const string & s) {
    vowels = 0;
    consonants = 0;
    for(auto c : s) {
      if(std::isalpha(c)) {
        if(::vowels.find(c) != string::npos) {
          ++ vowels;
        } else {
          ++ consonants;
        }
      }
    }
  }
};

struct prod_t {
  size_t sid;
  int letters;

  void set(const string & s) {
    letters = 0;
    for(auto c : s) {
      if(std::isalpha(c)) {
        ++letters;
      }
    }
  }
};

vector<int> primes { 2, 3, 5 };

void grow_primes(int new_max) {
  auto i = primes.back() + 1;
  while(i <= new_max) {
    bool flag = false;
    for(int p : primes) {
      if(0 == i % p) {
        flag = true;
        break;
      }
    }
    if(!flag) {
      primes.push_back(i);
    }
    ++i;
  }
}

double score(const cust_t & cust, const prod_t & prod) {
  double s = 0;
  if(prod.letters % 2 == 0) {
    s = 1.5 * cust.vowels;
  } else {
    s = cust.consonants;
  }
  auto cust_letters = cust.consonants + cust.vowels;
  grow_primes(cust_letters);
  grow_primes(prod.letters);
  for(int p : primes) {
    if(0 == cust_letters % p && 0 == prod.letters %p) {
      s *= 1.5;
      break;
    }
  }
  return s;
}

void process(string line) {
  grow_primes(100);
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST

  vector<string> vs;
  vector<cust_t> vc;//customers
  vector<prod_t> vp;//products

  // prepare lists of customers and products
  bool part1 = true;
  for(size_t pos = 0;;) {
    auto delim = line.find_first_of(",;", pos);

    auto sid = vs.size();
    vs.emplace_back(line.substr(pos, delim - pos));

    if(part1) {
      vc.emplace_back();
      vc.back().sid = sid;
      vc.back().set(vs.back());
    } else {
      vp.emplace_back();
      vp.back().sid = sid;
      vp.back().set(vs.back());
    }
    
    if(delim == string::npos) {
      break;
    }
    if(part1 && ';' == line[delim]) {
      part1 = false;
    } 
    pos = delim + 1;
  }

  // precalculate pairs
  vector<double> prec (vc.size() * vp.size());
  auto pwid = vp.size();
  auto phei = vc.size();
  for(int ic = 0; ic != phei; ++ic) {
    for(int ip = 0; ip != pwid; ++ip) {
      prec[ic * pwid + ip] = score(vc[ic], vp[ip]);
    }
  }
  // idx = col + line * pwid

  if(phei > pwid) {//transpose it
    vector<double> temp (prec.size());
    for(int i = 0; i != phei; ++i) {
      for(int j = 0; j != pwid; ++j) {
        temp[i + j * phei] =  prec[i * pwid + j];
      }
    }
    std::swap(phei, pwid);
    temp.swap(prec);
  }

  vector<double> vsum(pwid, 0.0);

  for(int line = 0; line < phei; ++line) {
    vector<double> nvsum(pwid, 0.0);
    ;
    nvsum.swap(vsum);
  }

  // print result
  double res = 0.25;
  std::cout.precision(2);
  std::cout.setf(std::cout.fixed);
  std::cout << res << "\n";
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
