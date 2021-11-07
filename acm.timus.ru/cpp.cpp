#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int main ()
{
#if ONLINE_JUDGE
  cin.sync_with_stdio (false);
#else
  istringstream cin;
#endif
  return 0;
}
