#include <iostream>
#include <list>
#include <string>
using namespace std;

template <typename T>
void PrintList(const list<T>& l) {
  for (const auto& x : l) {
    cout << x << ' ';
  }
  cout << endl;
}

template <typename T>
void SwapForList(list<T>& l, typename list<T>::iterator it1, typename list<T>::iterator it2) {
  auto it2_next = next(it2);
  l.splice(it1, l, it2);
  l.splice(it2_next, l, it1);
}

int main() {
  list<int> l = {1, 2, 3, 4, 5}; //expect = {1, 5, 3, 4, 2}
  auto it1 = ++l.begin();
  auto it2 = (--l.end());
  SwapForList(l, it1, it2);
  PrintList(l);
//  l.splice(it1, l, it2);
//  PrintList(l);
  cout << *it1 << ' ' << *it2 << endl;
  list<string> s = {"11", "22", "33", "44", "55"};
  auto it3 = ++s.begin();
  auto it4 = (--s.end());
  SwapForList(s, it3, it4);
  PrintList(s);
  return 0;
}
