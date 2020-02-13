#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

template <typename T>
class Deque {
private:
  vector<T> front;
  vector<T> back;

public:
  Deque() {}

  bool Empty() const {
    return front.empty() && back.empty();
  }

  size_t Size() const {
    return front.size() + back.size();
  }

  T& operator [] (size_t ind) {
    if (ind < front.size()) {
      return front[front.size() - 1 - ind];
    } else {
      return back[ind - front.size()];
    }
  }

  const T& operator [] (size_t ind) const {
    if (ind < front.size()) {
      return front[front.size() - 1 - ind];
    } else {
      return back[ind - front.size()];
    }
  }

  T& At (size_t ind) {
    if (ind >= front.size() + back.size()) {
      throw out_of_range("");
    } else {
      if (ind < front.size()) {
        return front[front.size() - 1 - ind];
      } else {
        return back[ind - front.size()];
      }
    }
  }

  const T& At (size_t ind) const {
    if (ind >= front.size() + back.size()) {
      throw out_of_range("");
    } else {
      if (ind < front.size()) {
        return front[front.size() - 1 - ind];
      } else {
        return back[ind - front.size()];
      }
    }
  }

  T& Front () {
    return !front.empty() ? front.back() : back.front();
  }

  const T& Front () const {
    return !front.empty() ? front.back() : back.front();
  }

  T& Back () {
    return !back.empty() ? back.back() : front.front();
  }

  const T& Back () const {
    return !back.empty() ? back.back() : front.front();
  }

  void PushFront(T val) {
    front.push_back(val);
  }

  void PushBack(T val) {
    back.push_back(val);
  }
};


//int main() {
//
//	return 0;
//}
