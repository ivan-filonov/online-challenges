#include <fstream>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>

//#define TEST
//#define VERBOSE_TEST

namespace {
#ifdef TEST
  void test();
#endif //#ifdef TEST
  void process_file(char*);
}

int main(int argc, char ** argv) {
#ifdef TEST
  test();
#else //#ifdef TEST
  process_file(argv[1]);
#endif //#ifdef TEST
	return 0;
}

namespace {
  void process(std::string s);

#ifdef TEST
  void test() {
    std::vector<std::string> v_test {
      "###########        ##  o  o  ##    o o ## o   *o ## o o    ## * * *o ##        ##        ####/######",
      "###########        ##    *   ## *      ##    *   ##   *    ##     ** ##     ** ##        ####/######",
      "###########        ##    * o ##  o     #/    o   ## o *    ##        ##        ##        ###########",
    };
    std::vector<std::string> v_expect {
      "###########\\   /\\  ## \\o/ o\\ ##  X o o\\## o \\ *o/## o o\\ / ## * * *o ##    / \\ ##   /   \\####/######",
      "###########\\ /\\/X\\ ## X /* \\\\##/*X/ \\ X##\\//\\* X/##/X *\\//\\##X \\ /**/##\\\\ X/** ## \\X/\\/ \\####X######",
      "###########  /\\ /\\ ## /  * o\\##/ o/ \\ /#/  / o X ## o * / \\##/ \\ /  /##\\  X  / ## \\/ \\/  ###########",
    };
    for(int i = 0, j = std::min(v_test.size(), v_expect.size()); i < j; ++i) {
      process(v_test[i]);
      std::cout << v_expect[i] << "\n";
#ifdef VERBOSE_TEST
      for(int j = 0; j < 10; ++j) {
        std::cout << "e[p." << j << "]: " << v_expect[i].substr(j * 10, 10) << "\n";
      }
#endif//#ifdef VERBOSE_TEST
    }
  }
#endif //#ifdef TEST

  void process_file(char* path) {
    std::ifstream stream(path);
    for(std::string line; std::getline(stream, line); ) {
      process(line);
    }
  }

  void process(std::string line) {
#ifdef TEST
#ifdef VERBOSE_TEST
    for(int i = 0; i < 10; ++i) {
      std::cout << "s[p." << i << "]: " << line.substr(i * 10, 10) << "\n";
    }
#endif//#ifdef VERBOSE_TEST
#endif //#ifdef TEST

    // 1. find entrance
    std::vector<int> vx, vy, vdx, vdy, active;
    {
      int ex = -1, ey;
      for(int i = 0; i < 10 && ex < 0; ++i) {
        int vv[4] = { 0, 9, i, i };
        int hh[4] = { i, i, 0, 9 };
        for(int j = 0; j < 4; ++j) {
          if('#' != line[10 * vv[j] + hh[j]]) {
            ex = hh[j];
            ey = vv[j];
          }
        }
      }
#ifdef VERBOSE_TEST
      std::cout << "ex, ey = " << ex << ", " << ey << "\n";
#endif//#ifdef VERBOSE_TEST
      vx.push_back( ex );
      vy.push_back( ey );
    }

    // 2. initial direction
    {
      int dx = 0, dy = 0;
      auto ec = line[vx.front() + 10 * vy.front()];
      switch(vx.front()) {
        case 0:
          dx = 1;
          dy = ('/' == ec) ? -1 : (('\\' == ec ) ? 1 : 0);
          break;
        case 9:
          dx = -1;
          dy = ('/' == ec) ? 1 : (('\\' == ec ) ? -1 : 0);
          break;
        default:
          switch(vy.front()) {
            case 0:
              dy = 1;
              dx = ('/' == ec) ? -1 : (('\\' == ec ) ? 1 : 0);
              break;
            case 9:
              dy = -1;
              dx = ('/' == ec) ? 1 : (('\\' == ec ) ? -1 : 0);
              break;
            default:
              throw std::exception();
          }
      }
#ifdef VERBOSE_TEST
//      std::cout << "dx, dy = " << dx << ", " << dy << "\n";
#endif//#ifdef VERBOSE_TEST
      vdx.push_back( dx );
      vdy.push_back( dy );
    }

    auto mark = [&](int x, int y, int dx, int dy) {
#ifdef VERBOSE_TEST
      std::cout << __func__ << "( " << x << ", " << y << ", " << dx << ", " << dy << " ): '" << line[10*y+x] << "' -> '";
#endif//#ifdef VERBOSE_TEST
      char m = (dx * dy > 0) ? '\\' : '/';
      char &d = line[10 * y + x];
      if(!('#' == d || '*' == d || 'o' == d || 'X' == d)) {
        if(('/' == d && '\\' == m) || ('/' == m && '\\' == d)) {
          d = 'X';
        } else {
          d = m;
        }
      }
#ifdef VERBOSE_TEST
      std::cout << d << "'\n";
#endif//#ifdef VERBOSE_TEST
    };

    // 3. trace it
    if( vdx.front() * vdy.front() ) {
      active.push_back(20);//< activate the first light
#ifdef VERBOSE_TEST
      int dist = 0;
#endif//#ifdef VERBOSE_TEST
      while(std::accumulate(active.begin(), active.end(), 0) > 0) {
#ifdef VERBOSE_TEST
        ++dist;
#endif//#ifdef VERBOSE_TEST
        for(int ray = 0, rmax = vx.size(); ray != rmax; ++ray) {
          int x = vx[ray], y = vy[ray], dx = vdx[ray], dy = vdy[ray];
          mark(x, y, dx, dy);
          if(!active[ray]--) {
            continue;
          }
          if( '*' == line[10 * y + x]) {
            active[ray]++;
            int nx, ny, ndx, ndy, na = active[ray];
            std::tie(nx, ny, ndx, ndy) = std::make_tuple( x + dx, y - dy, dx, -dy );
            char nc = line[10 * ny + nx];
            if('#' != nc && 'o' != nc) {
              vx.push_back(nx);
              vdx.push_back(ndx);
              vy.push_back(ny);
              vdy.push_back(ndy);
              active.push_back(na);
            }
            std::tie(nx, ny, ndx, ndy) = std::make_tuple( x - dx, y + dy, -dx, dy );
            nc = line[10 * ny + nx];
            if('#' != nc && 'o' != nc) {
              vx.push_back(nx);
              vdx.push_back(ndx);
              vy.push_back(ny);
              vdy.push_back(ndy);
              active.push_back(na);
            }
          }

          int new_x = x + dx, new_y = y + dy;
          char &new_c = line[new_y * 10 + new_x];
#ifdef VERBOSE_TEST
//          std::cout << "tst: x = " << new_x << ", y = " << new_y << ", dx = " << dx << ", dy = " << dy << ", c = '" << new_c << "'\n";
#endif//#ifdef VERBOSE_TEST
          // choices now: pass, split, finish, mirror
          switch(new_c) {
            case '#':
              if( (0 == new_x && (0 == new_y || 9 == new_y)) || (9 == new_x && (0 == new_y || 9 == new_y)) ) {
                active[ray] = 0;
              } else {
                if(0 == new_x || 9 == new_x) {
                  dx = -dx;
                  new_x += dx;
                } else {
                  dy = -dy;
                  new_y += dy;
                }
                std::tie(x,y)=std::make_tuple(new_x, new_y);
//                mark(new_x, new_y, dx, dy);
              }
              break;
            case 'o':
              active[ray] = 0;
              break;
            case '*':
            case ' ':
            case '/':
            case '\\':
            case 'X':
//              mark(new_x, new_y, dx, dy);
              std::tie(x,y)=std::make_tuple(new_x, new_y);
              break;
          }
          std::tie(vx[ray], vy[ray], vdx[ray], vdy[ray]) = std::make_tuple(x, y, dx, dy);
#ifdef VERBOSE_TEST
//          std::cout << "new: x = " << new_x << ", y = " << new_y << ", dx = " << dx << ", dy = " << dy << ", c = '" << new_c << "'\n";
#endif//#ifdef VERBOSE_TEST
        }
#ifdef TEST
#ifdef VERBOSE_TEST
        for(int z = 0; z < 10; ++z) {
          std::cout << "T[" << dist << "][" << z << "]: " << line.substr(10 * z, 10) << "\n";
        }
#endif//#ifdef VERBOSE_TEST
#endif//#ifdef TEST
      }
    } else throw std::exception();
    std::cout << line << "\n";
  }
}
