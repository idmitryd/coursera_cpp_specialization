#include <iostream>
#include <list>
#include <map>
using namespace std;

int main() {
  int q;
  cin >> q;
  list<int> result;
  map<int, list<int>::iterator> pos;
  for (int i = 0; i < q; ++i) {
    int who, where;
    cin >> who >> where;
    if (pos.count(where) == 1) {
      pos[who] = result.insert(pos[where], who);
    } else {
      pos[who] = result.insert(result.end(), who);
    }
  }
  for(const int& x : result) {
    cout << x << ' ';
  }
  return 0;
}
