#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

using namespace std;

int chk (const string& s)
{
  int sum = 0;
  for (int i = 0; i < (int)s.size (); ++i) {
    sum += (i + 1) * (s[i] - '0');
  }
  return sum;
}

int main (int argc, char** argv)
{
  int N = 4;
  if (argc > 1) {
    N = stoi (argv[1]);
  }
  int count = 6;
  if (argc > 2) {
    count = stoi (argv[2]);
  }
  default_random_engine gen{random_device{}()};

  vector<string>        in, out;
  unordered_set<string> known;

  while (count > (int)in.size ()) {
    string s, d;

    uniform_int_distribution u{0, N - 1};
    bernoulli_distribution   bd{0.4};
    for (int j = 0; j < N; ++j) {
      s.push_back (bd (gen) ? '1' : '0');
    }
    while (chk (s) % (N + 1) != 0) {
      int o = u (gen);
      s[o] = '1' - (s[o] - '0');
    }

    d = s;

    const int noise = uniform_int_distribution{0, 310}(gen);
    if (noise < 10) { // no noise
    } else if (noise < 110) {
      if (std::count (s.begin (), s.end (), '0') > 0) {
        int ofs;
        for (ofs = u (gen); s[ofs] == '1';) {
          ofs = u (gen);
        }
        s[ofs] = '1';
      }
    } else if (noise < 210) {
      // remove
      s.erase (u (gen), 1);
    } else {
      // insert or append
      s.insert (s.begin () + uniform_int_distribution{0, N}(gen), bd (gen) ? '1' : '0');
    }

    auto [iter, inserted] = known.insert (s);
    if (inserted) {
      in.push_back (d);
      out.push_back (s);
    }
  }

  ofstream si{"test.in"};
  ofstream so{"test.out"};

  si << N << endl;
  for (auto& s : out) {
    si << s << endl;
  }
  for (auto& s : in) {
    so << s << endl;
  }


  return 0;
}
