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

const int MAX = 1000000000;

int N = 0;

// coords -> offset
int c(int x, int y, int floor) {
  return (floor * N + y) * N + x;
}

// offset -> coords
void c(int o, int * x, int * y, int * z) {
  *x = o % N;
  o /= N;
  *y = o % N;
  *z = o / N;
}

int process(string line) {
#ifdef TEST
  std::cout << "s = '" << line << "'\n";
#endif //#ifdef TEST
  N = std::stoi(line);
  vector<char> v(N*N*N);
  vector<int> d(v.size(), MAX);
  for(size_t si = line.find(';') + 1, di = 0; si != line.length(); ++si, ++di) {
    v[di] = line[si];
  }

  vector<int> b;
  for(int i = 0, o; i < N; ++i) {
    o = c(i, 0, 0);
    if(' ' == v[o]) {
      b.push_back(o);
      break;
    }
    o = c(0, i, 0);
    if(' ' == v[o]) {
      b.push_back(o);
      break;
    }
    o = c(i, N-1, 0);
    if(' ' == v[o]) {
      b.push_back(o);
      break;
    }
    o = c(N-1, i, 0);
    if(' ' == v[o]) {
      b.push_back(o);
      break;
    }
  }
  d[b.back()] = 1;

  int f = 0;
  for(int i = 0, o; i < N; ++i) {
    o = c(i, 0, N-1);
    if(' ' == v[o]) {
      f = o;
      break;
    }
    o = c(0, i, N-1);
    if(' ' == v[o]) {
      f = o;
      break;
    }
    o = c(i, N-1, N-1);
    if(' ' == v[o]) {
      f = o;
      break;
    }
    o = c(N-1, i, N-1);
    if(' ' == v[o]) {
      f = o;
      break;
    }
  }

  vector<int> t(6);
  while(!b.empty()) {
    std::sort(b.begin(), b.end(), [&d](int a,int b){return d[a]>d[b];});
    int cx, cy, cz, cur { b.back() };
    b.pop_back();
    c(cur, &cx, &cy, &cz);
    t.clear();
    
    // cur - offset for point to process
    if(cx > 0) {//cx-1,cy,cz
      auto to = c(cx-1, cy, cz);
      if('*' != v[to]) {
        t.push_back(to);
      }
    }
    if(cx < N - 1) {//cx+1,cy,cz
      auto to = c(cx+1, cy, cz);
      if('*' != v[to]) {
        t.push_back(to);
      }
    }
    if(cy > 0) {//cx,cy-1,cz
      auto to = c(cx, cy-1, cz);
      if('*' != v[to]) {
        t.push_back(to);
      }
    }
    if(cy < N - 1) {//cx,cy+1,cz
      auto to = c(cx, cy+1, cz);
      if('*' != v[to]) {
        t.push_back(to);
      }
    }
    if(cz > 0) {//cx,cy,cz-1
      auto to = c(cx, cy, cz-1);
      if('*' != v[to] && 'o' == v[cur]) {
        t.push_back(to);
      }
    }
    if(cz < N - 1) {//cx,cy,cz+1
      auto to = c(cx, cy, cz+1);
      if('*' != v[to] && 'o' == v[to]) {
        t.push_back(to);
      }
    }
    // t - all neighbours we can move to
    const auto dd = d[cur] + 1;
    for(auto i : t) {
      if(d[i] > dd) {
        d[i] = dd;
        b.push_back(i);
      }
    }
  }

  auto df = d[f];
  return df < MAX ? df : 0;
}

#ifdef TEST
void test() {
  vector<string> v_test {
    "5;******   ** ****   *** ********  o**o* ** * ************   ** * **o* ************ o **** **   ******** ***o  ****o**o  ******",
    "5;****** * ** * **   *** ********o*o** *o**   ************   **o* ** *o************   **** **   ******** *** o ****o**  o******",
    "7;********     ** *** ** * * ** * * **     **** *********** *   ** *** **   *o**o***o**oo   ****************  oo ** * * ** * * ** * * ** *o o****************     ** * ****o* * ** *** **o    ****************     ** *** **     ****** ** o  o**************** *oo ** *** **o  * ** *** **o    *********** ****   * ** * * ** * *o** ***o**     ********",
  };
  vector<string> v_expect {
    "15",
    "0",
    "39",
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
