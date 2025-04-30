#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

struct Task {
  std::vector<std::string> removed;
  std::vector<std::string> sought;
};

class Env
{
public:
  virtual Task read () = 0;
  virtual void write (float pct) = 0;
};

class TestEnv : public Env
{
  Task read () override
  {
    return {{"45C"}, {"7H"}};
  }
  void write (float pct) override
  {
    std::cout << std::roundf (pct) << "%\n";
    if (std::abs (pct - 2) > 0.1f) {
      std::cerr << "FAIL\n";
    }
  }
};

class CgEnv : public Env
{
  Task read () override
  {
    int num_removed;
    int num_sought;
    std::cin >> num_removed >> num_sought;
    std::cin.ignore ();
    std::vector<std::string> removed;
    std::vector<std::string> sought;
    removed.resize (num_removed);
    sought.resize (num_sought);
    for (auto& line : removed) {
      std::getline (std::cin, line);
    }
    for (auto& line : sought) {
      std::getline (std::cin, line);
    }
    return {removed, sought};
  }
  void write (float pct) override
  {
    std::cout << std::roundf (pct) << "%\n";
  }
};

std::unique_ptr<Env> create_env ()
{
#ifdef TEST
  return std::make_unique<TestEnv> ();
#else
  return std::make_unique<CgEnv> ();
#endif
}

static constexpr char ranks[] = "23456789TJQKA";
static constexpr char suits[] = "CDHS";

class Solver
{
public:
  std::unordered_set<std::string> build_set (const std::string& desc)
  {
    std::unordered_set<char> rs;
    std::unordered_set<char> ss;

    bool rf = true;
    for (auto c : desc) {
      if (rf) {
        rf = ranksSet.count (c) > 0;
        if (rf) {
          rs.insert (c);
          continue;
        }
      }
      if (suitsSet.count (c) == 0) {
        throw 1;
      }
      ss.insert (c);
    }
    if (rs.empty ()) {
      rs = ranksSet;
    }
    if (ss.empty ()) {
      ss = suitsSet;
    }
    std::unordered_set<std::string> res;
    for (auto r : rs) {
      for (auto s : ss) {
        res.insert (std::string () + r + s);
      }
    }
    return res;
  }

  std::unordered_set<char> ranksSet;
  std::unordered_set<char> suitsSet;

  void init_sets ()
  {
    for (auto c : ranks) {
      if (std::isalnum (c))
        ranksSet.insert (c);
    }
    for (auto c : suits) {
      if (std::isalnum (c))
        suitsSet.insert (c);
    }
    std::cerr << __PRETTY_FUNCTION__ << " sizes: ranks: " << ranksSet.size ()
              << " suits: " << suitsSet.size () << "\n";
  }

  template <typename C> void dump (std::string msg, const C& strings)
  {
    std::cerr << msg << " (size=" << strings.size () << ")";
    for (const auto& s : strings) {
      std::cerr << " " << s;
    }
    std::cerr << std::endl;
  }

  float solve (Task task)
  {
    dump ("removed:", task.removed);
    dump ("sought:", task.sought);

    init_sets ();

    auto cards = build_set ("");
    auto sought = std::unordered_set<std::string>{};
    for (const auto& desc : task.removed) {
      const auto set = build_set (desc);
      for (const auto& c : set) {
        cards.erase (c);
      }
    }
    for (const auto& desc : task.sought) {
      const auto set = build_set (desc);
      for (const auto& c : set) {
        sought.insert (c);
      }
    }
    for (auto iter = sought.begin (); iter != sought.end ();) {
      if (cards.count (*iter) > 0) {
        ++iter;
      } else {
        iter = sought.erase (iter);
      }
    }
    if (cards.empty ()) {
      return 0;
    }
    dump ("cards", cards);
    dump ("sought", sought);
    return (float)sought.size () * 100.f / cards.size ();
  }
};

int main ()
{
  auto env = create_env ();
  auto solver = Solver{};
  env->write (solver.solve (env->read ()));
  return 0;
}
