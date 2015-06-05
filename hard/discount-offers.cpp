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
  vector<string> vs;
  vector<cust_t> v_customers;//customers
  vector<prod_t> v_products;//products

  // prepare lists of customers and products
  bool part1 = true;
  for(size_t pos = 0;;) {
    auto delim = line.find_first_of(",;", pos);

    auto sid = vs.size();
    vs.emplace_back(line.substr(pos, delim - pos));

    if(part1) {
      v_customers.emplace_back();
      v_customers.back().sid = sid;
      v_customers.back().set(vs.back());
    } else {
      v_products.emplace_back();
      v_products.back().sid = sid;
      v_products.back().set(vs.back());
    }
    
    if(delim == string::npos) {
      break;
    }
    if(part1 && ';' == line[delim]) {
      part1 = false;
    } 
    pos = delim + 1;
  }

  // weight matrix with fake line and column at index 0
  vector<double> prec ((1 + v_customers.size()) * (1 + v_products.size()));
  // prec[customer * pwid + product] - customer and product are now 1-based
  auto pwid = v_products.size() + 1 ;
  auto phei = v_customers.size() + 1;
  for(int customer = 0; customer < v_customers.size(); ++customer) {
    for(int product = 0; product < v_products.size(); ++product) {
      prec[customer * pwid + pwid + product + 1] = -score(v_customers[customer], v_products[product]);
    }
  }
  
  if(v_customers.size() > v_products.size()) {
    vector<double> temp(prec.size());
    for(int line = 0; line < phei; ++line) {
      for(int col = 0; col < pwid; ++col) {
        temp[line + phei * col] = prec[line * pwid + col];
      }
    }
    std::swap(pwid, phei);
    temp.swap(prec);
  }

  // idx = col + line * pwid

  // hungarian alg now:
  const auto num_lines = phei - 1;
  const auto num_columns = pwid - 1;

  vector<double> line_level(phei, 0.0);
  vector<double> column_level(pwid, 0.0);
  vector<int> assigned_line_for_column(pwid, 0);
  vector<int> backtrack(pwid, 0);
  
  vector<double> minv(pwid);
  vector<bool> used(pwid);

  for(auto line_to_add = 1; line_to_add <= num_lines; ++line_to_add) {
    assigned_line_for_column[0] = line_to_add;
    int cur_column = 0;

    minv.assign(pwid, 1e9);
    used.assign(pwid, false);

    do {
      used[cur_column] = true;
      const int cur_line = assigned_line_for_column[cur_column];
      double delta = 1e9;
      int next_column = 0;

      for(auto column = 1; column <= num_columns; ++column) {
        if(used[column]) {
          continue;
        }

        const auto cur = prec[cur_line * pwid + column] - line_level[cur_line] - column_level[column];
        if(cur < minv[column]) {
          minv[column] = cur;
          backtrack[column] = cur_column;
        }

        if(minv[column] < delta) {
          delta = minv[column];
          next_column = column;
        }
      }

      for(auto column = 0; column <= num_columns; ++column) {
        if(used[column]) {
          line_level[assigned_line_for_column[column]] += delta;
          column_level[column] -= delta;
        } else {
          minv[column] -= delta;
        }
      }

      cur_column = next_column;
    } while(0 != assigned_line_for_column[cur_column]);
    do {
      const int prev_column = backtrack[cur_column];
      assigned_line_for_column[cur_column] = assigned_line_for_column[prev_column];
      cur_column = prev_column;
    } while(0 != cur_column);
  }

  // print result
  double res = 0.0;
  for(int column = 1; column <= num_columns; ++column) {
    auto line = assigned_line_for_column[column];
    res += prec[line * pwid + column];
  }
//  std::cout.precision(2);
//  std::cout.setf(std::cout.fixed);
  std::cout << std::abs(res) << "\n";
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "Jack Abraham;iPad 2 - 4-pack,Girl Scouts Thin Mints,Nerf Crossbow",
    "Jack Abraham,John Evans,Ted Dziuba;iPad 2 - 4-pack",
    "Jack Abraham,John Evans,Ted Dziuba;iPad 2 - 4-pack,Girl Scouts Thin Mints,Nerf Crossbow",
    "Jeffery Lebowski,Walter Sobchak,Theodore Donald Kerabatsos,Peter Gibbons,Michael Bolton,Samir Nagheenanajar;Half & Half,Colt M1911A1,16lb bowling ball,Red Swingline Stapler,Printer paper,Vibe Magazine Subscriptions - 40 pack",
    "Jareau Wade,Rob Eroh,Mahmoud Abdelkader,Wenyi Cai,Justin Van Winkle,Gabriel Sinkin,Aaron Adelson;Batman No. 1,Football - Official Size,Bass Amplifying Headphones,Elephant food - 1024 lbs,Three Wolf One Moon T-shirt,Dom Perignon 2000 Vintage",
  };
  vector<string> v_expect {
    "7.00",
    "6.00",
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
