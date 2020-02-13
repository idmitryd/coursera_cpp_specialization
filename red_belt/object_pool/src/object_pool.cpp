#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
  T* Allocate() {
    if (!free.empty()) {
      T* sel = free.front();
      free.pop();
      selected.insert(sel);
      return sel;
    }
    T* t = new T;
    selected.insert(t);
    return t;
  }
  T* TryAllocate() {
    if (free.empty()) {
      return nullptr;
    }
    return Allocate();
  }

  void Deallocate(T* object) {
      auto it = selected.find(object);
      if (it == selected.end()) {
        throw invalid_argument("");
      }
      free.push(*it);
      selected.erase(it);
  }

  ~ObjectPool() {
    for (auto& p : selected) {
      delete p;
    }
    while(!free.empty()) {
      delete free.front();
      free.pop();
    }
  }

private:
  set<T*> selected;
  queue<T*> free;
};


//void TestObjectPool() {
//  ObjectPool<string> pool;
//
//  auto p1 = pool.Allocate();
//  auto p2 = pool.Allocate();
//  auto p3 = pool.Allocate();
//
//  *p1 = "first";
//  *p2 = "second";
//  *p3 = "third";
//
//  pool.Deallocate(p2);
//  ASSERT_EQUAL(*pool.Allocate(), "second");
//
//  pool.Deallocate(p3);
//  pool.Deallocate(p1);
//  ASSERT_EQUAL(*pool.Allocate(), "third");
//  ASSERT_EQUAL(*pool.Allocate(), "first");
//
//  pool.Deallocate(p1);
//}
//
//int main() {
//  TestRunner tr;
//  RUN_TEST(tr, TestObjectPool);
//  return 0;
//}

