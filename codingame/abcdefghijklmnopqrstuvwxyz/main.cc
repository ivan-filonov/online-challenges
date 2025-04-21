#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Test {
  std::vector<std::string> lines;
};

struct Env {
public:
  virtual Test read () = 0;
  virtual void write (Test) = 0;
};

struct CgEnv : public Env {
  Test read () override
  {
    Test res;
    int  n;
    std::cin >> n;
    std::cin.ignore ();
    for (int i = 0; i < n; ++i) {
      std::string s;
      std::cin >> s;
      std::cin.ignore ();
      res.lines.push_back (s);
    }
    return res;
  }
  void write (Test t) override
  {
    for (auto& s : t.lines) {
      std::cout << s << std::endl;
    }
  }
};

struct TestEnv : public Env {
  int opt;
  TestEnv (int opt)
  : opt (opt)
  {
  }

  Test read () override
  {
    return Test{{
      "qadnhwbnyw",
      "iiopcygydk",
      "bahlfiojdc",
      "cfijtdmkgf",
      "dzhkliplzg",
      "efgrmpqryx",
      "loehnovstw",
      "jrsacymeuv",
      "fpnocpdkrs",
      "jlmsvwvuih",
    }};
  }
  void write (Test test) override
  {
    std::vector<std::string> expected = {
      "----------",
      "----------",
      "ba--------",
      "c-ij------",
      "d-hkl---z-",
      "efg-mpqryx",
      "----no-stw",
      "--------uv",
      "----------",
      "----------",
    };
    assert (test.lines.size () == expected.size ());
    auto it1 = test.lines.begin ();
    auto it2 = expected.begin ();
    while (it2 != expected.end ()) {
      if (*it1 != *it2) {
        std::cerr << "error: have \"" << *it1 << "\", expected \"" << *it2 << "\"\n";
      }
      assert (*it1 == *it2);
      ++it1;
      ++it2;
    }
  }
};

std::unique_ptr<Env> create_env (int opt)
{
#ifdef TEST
  return std::make_unique<TestEnv> (opt);
#else
  return std::make_unique<CgEnv> ();
#endif
}

class Solver
{
public:
  std::vector<std::vector<bool>> visited;
  std::vector<std::vector<bool>> answer;
  std::vector<std::string>*      lines;
  size_t                         size;

  char get (size_t y, size_t x)
  {
    return (*lines)[y][x];
  }

  bool dfs (size_t y, size_t x)
  {
    if (visited[y][x]) {
      return false;
    }
    visited[y][x] = true;
    const auto cur = get (y, x);
    if (cur == 'z') {
      answer[y][x] = true;
      return true;
    }
    if (x > 0 && get (y, x - 1) == cur + 1) {
      if (dfs (y, x - 1)) {
        answer[y][x] = true;
        return true;
      }
    }
    if (y > 0 && get (y - 1, x) == cur + 1) {
      if (dfs (y - 1, x)) {
        answer[y][x] = true;
        return true;
      }
    }
    if (x + 1 < size && get (y, x + 1) == cur + 1) {
      if (dfs (y, x + 1)) {
        answer[y][x] = true;
        return true;
      }
    }
    if (y + 1 < size && get (y + 1, x) == cur + 1) {
      if (dfs (y + 1, x)) {
        answer[y][x] = true;
        return true;
      }
    }
    return false;
  }

  void solve (Test& test)
  {
    lines = &test.lines;

    for (auto& s : *lines) {
      assert (s.size () == lines->size ());
    }
    size = lines->size ();
    visited.resize (size);
    for (auto& v : visited) {
      v.resize (size, false);
    }
    answer.resize (size);
    for (auto& a : answer) {
      a.resize (size, false);
    }

    for (size_t y = 0; y < size; ++y) {
      for (size_t x = 0; x < size; ++x) {
        if (visited[y][x]) {
          continue;
        }
        if (get (y, x) != 'a') {
          continue;
        }
        std::cerr << "test: " << y << ":" << x << "\n";
        if (dfs (y, x)) {
          std::cerr << "solved!\n";
          for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
              if (!answer[i][j]) {
                (*lines)[i][j] = '-';
              }
            }
          }
          return;
        }
      }
    }
  }
};

int main ()
{
  auto   env = create_env (0);
  auto   test = env->read ();
  Solver solver;
  solver.solve (test);
  env->write (std::move (test));
  return 0;
}
