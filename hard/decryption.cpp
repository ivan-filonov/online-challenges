#include <iostream>
#include <vector>

using namespace std;

int main() {
  string message { "012222 1114142503 0313012513 03141418192102 0113 2419182119021713 06131715070119" };
  string key { "BHISOECRTMGWYVALUZDNFJKPQX" };
  int st = 0;
  int v = 0;
  for(char c : message) {
    switch(st) {
      case 0:
        if(' ' == c) {
          cout << ' ';
        } else {
          v = c - '0';
          st = 1;
        }
        break;
      case 1:
        cout << (char)('A' + key.find('A' + v * 10 + (c - '0')));
        v = 0;
        st = 0;
        break;
    }
  }
  return 0;
}
