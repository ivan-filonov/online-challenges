#include <deque>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Env
{
public:
  virtual std::vector<std::string> read () = 0;
  virtual void write (const std::vector<std::string>& maze) = 0;
};

class CgEnv : public Env
{
public:
  std::vector<std::string> read () override
  {
    int width;
    int height;

    std::vector<std::string> maze;

    std::cin >> width >> height;
    std::cin.ignore ();
    std::cerr << "width=" << width << " height=" << height << "\n";
    maze.resize (height);
    for (int i = 0; i < height; ++i) {
      std::getline (std::cin, maze[i]);
      if (maze[i].size () != width) {
        throw 0;
      }
      std::cerr << "line #" << i << ": " << maze[i] << "\n";
    }
    return maze;
  }
  void write (const std::vector<std::string>& maze) override
  {
    for (auto& line : maze) {
      std::cout << line << "\n";
    }
  }
};
class TestEnv : public Env
{
public:
  std::vector<std::string> read () override
  {
    std::vector<std::string> maze
      = {"##########", "#S.......#", "##.#####.#", "##.#.....#", "##########"};
    return maze;
  }
  void write (const std::vector<std::string>& maze) override
  {
    std::vector<std::string> expected
      = {"##########", "#01234567#", "##2#####8#", "##3#DCBA9#", "##########"};
    if (maze.size () != expected.size ()) {
      throw 1;
    }
    for (size_t i = 0; i < maze.size (); ++i) {
      const auto& a = maze[i];
      const auto& b = expected[i];
      if (a != b) {
        std::cerr << "line #" << (i + 1) << ": expected \"" << b << "\" got \""
                  << a << "\"\n";
        throw 2;
      }
    }
  }
};

class Solver
{
public:
  void solve (std::vector<std::string>& maze)
  {
    const int widht = maze[0].size ();
    const int height = maze.size ();

    int  startx;
    int  starty;
    bool found = false;
    for (int y = 0; !found && y < maze.size (); ++y) {
      const auto& line = maze[y];
      for (int x = 0; x < line.size (); ++x) {
        if (line[x] == 'S') {
          startx = x;
          starty = y;
          found = true;
          break;
        }
      }
    };
    std::vector<std::vector<bool>> visited;
    visited.resize (maze.size ());
    for (size_t i = 0; i < maze.size (); ++i) {
      visited[i].resize (maze[i].size (), false);
    }
    struct Pt {
      int x, y, d;
    };
    std::vector<std::pair<int, int>> delta = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};

    std::deque<Pt> queue;
    queue.emplace_back (Pt{startx, starty, 0});
    while (!queue.empty ()) {
      auto pt = queue.front ();
      queue.pop_front ();
      if (visited[pt.y][pt.x]) {
        continue;
      }
      visited[pt.y][pt.x] = true;
      maze[pt.y][pt.x] = pt.d < 10 ? '0' + pt.d : 'A' + pt.d - 10;
      for (auto [dx, dy] : delta) {
        auto p = Pt{pt.x + dx, pt.y + dy, pt.d + 1};
        p.x = (p.x + widht) % widht;
        p.y = (p.y + height) % height;
        if (maze[p.y][p.x] == '#') {
          continue;
        }
        queue.push_back (p);
      }
    }
  }
};

int main ()
{
  auto env =
#ifdef TEST
    std::make_unique<TestEnv> ();
#else
    std::make_unique<CgEnv> ();
#endif
  auto solver = Solver{};

  auto task = env->read ();
  solver.solve (task);
  env->write (task);

  return 0;
}
