#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace std;

static constexpr unsigned char H = 196;
static constexpr unsigned char V = 179;
static constexpr unsigned char LT = 218;
static constexpr unsigned char LB = 192;
static constexpr unsigned char RT = 191;
static constexpr unsigned char RB = 217;
static constexpr unsigned char ANY = 254;
static constexpr unsigned char NONE = 46;

class Square
{
public:
  int                                    top;
  int                                    left;
  int                                    side;
  vector<tuple<int, int, unsigned char>> chars;

  Square (const int top_, const int left_, const int side_)
  : top (top_)
  , left (left_)
  , side (side_)
  {
    const int right = left + side - 1;
    const int bottom = top + side - 1;
    chars.emplace_back (top, left, LT);
    for (int x = left + 1; x < right; ++x) {
      chars.emplace_back (top, x, H);
    }
    chars.emplace_back (top, right, RT);
    for (int y = top + 1; y < bottom; ++y) {
      for (int x : {left, right}) {
        chars.emplace_back (y, x, V);
      }
    }
    chars.emplace_back (bottom, left, LB);
    for (int x = left + 1; x < right; ++x) {
      chars.emplace_back (bottom, x, H);
    }
    chars.emplace_back (bottom, right, RB);
  }
};

class Main
{
public:
  int                           num_fragmets = 0;
  array<unsigned char, 20 * 50> data;

  Main ()
  {
    data.fill (NONE);
    cc.clear ();
    cc[NONE] = data.size ();
  }

  unordered_map<unsigned char, int> cc;
  void                              set (int y, int x, unsigned char c)
  {
    const auto oldc = data[y * 50 + x];
    if (oldc == c) {
      return;
    }
    cc[oldc] -= 1;
    cc[c] += 1;
    const auto old_none = oldc == NONE;
    const auto old_any = oldc == ANY;
    const auto old_fragment = !old_none && !old_any;
    const auto new_none = c == NONE;
    const auto new_any = c == ANY;
    const auto new_fragment = !new_none && !new_any;
    if (!old_fragment && new_fragment) {
      num_fragmets++;
    } else if (old_fragment && !new_fragment) {
      num_fragmets--;
    }
    data[y * 50 + x] = c;
  }

  unsigned char get (int y, int x)
  {
    return data[y * 50 + x];
  }

  void print ()
  {
    wcout << L"num fragments: " << num_fragmets << endl;
    for (auto [c, n] : cc) {
      wcout << L"'\\" << (c) << L"': " << n << endl;
    }
    for (int y = 0, ofs = 0; y < 20; ++y) {
      for (int x = 0; x < 50; ++x, ++ofs) {
        wcout.put (get (y, x));
      }
      wcout << endl;
    }
  }

  bool check (const Square& square, int& misses)
  {
    int num_any = 0;
    int num_misses = 0;
    for (auto [y, x, expected_c] : square.chars) {
      const auto c = get (y, x);
      if (c == NONE) {
        return false;
      } else if (c == ANY) {
        num_any += 1;
      } else if (c != expected_c) {
        num_misses += 1;
      }
    }
    if (num_any == square.side * 4 - 4) {
      return false;
    }
    misses = num_misses;
    return true;
  }

  void render (const Square& square)
  {
    for (auto [y, x, c] : square.chars) {
      set (y, x, c);
    }
  }

  void render_any (const Square& square)
  {
    for (auto [y, x, c] : square.chars) {
      set (y, x, ANY);
    }
  }

  void solve ()
  {
    print ();

    vector<Square> squares;
    for (int y = 0; y < 19; ++y) {
      for (int x = 0; x < 49; ++x) {
        if (get (y, x) == NONE) {
          continue;
        }
        for (int s = 20; s >= 2; --s) {
          if (x + s - 1 > 49 || y + s - 1 > 19) {
            continue;
          }
          Square sq{y, x, s};
          int    misses;
          if (check (sq, misses)) {
            squares.emplace_back (move (sq));
          }
        }
      }
    }

    vector<Square> answer;
    vector<Square> temp;
    for (int i = 0; i < 10 && num_fragmets > 0 && squares.size () > 0; ++i) {
      sort (begin (squares), end (squares),
        [](const Square& a, const Square& b) { return a.side > b.side; });
      bool copied = false;
      for (auto& s : squares) {
        int m = -1;
        if (!check (s, m)) {
          continue;
        }
        if (!copied && m == 0) {
          answer.emplace_back (move (s));
          copied = true;
        } else {
          temp.emplace_back (move (s));
        }
      }
      if (!copied) {
        break;
      }
      render_any (answer.back ());
      squares = move (temp);
    }

    cout << answer.size () << endl;
    for (auto iter = answer.rbegin (); iter != answer.rend (); ++iter) {
      cout << iter->left << " " << iter->top << " " << iter->side << endl;
    }
  }
};

int main ()
{
  Main app;

  for (int y = 0; y < 20; ++y) {
    for (int x = 0; x < 50; ++x) {
      unsigned char c = getchar ();
      app.set (y, x, c);
    }
    unsigned char c = getchar ();
    if (c != '\n') {
      app.print();
      throw std::runtime_error (
        "invalid input at " + to_string (y) + ": " + to_string (c));
    }
  }

  app.solve ();

  return 0;
}
