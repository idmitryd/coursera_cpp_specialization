#pragma once

#include <algorithm>
using namespace std;

template <typename It>
class IteratorRange {
public:
  IteratorRange(It first, It last) : first(first), last(last) {
  }

  It begin() const {
    return first;
  }

  It end() const {
    return last;
  }

  size_t size() const {
    return last - first;
  }

private:
  It first, last;
};

template <typename Container>
auto Head(Container& c, int top) {
  return IteratorRange(begin(c), next(begin(c), min<size_t>(max(top, 0), c.size())));
}

template <typename Container>
auto Tail(Container& c, int top) {
//  return IteratorRange(begin(c), next(begin(c), min<size_t>(max(top, 0), c.size())));
//  return IteratorRange(prev(end(c), min<size_t>(max(top, 0), c.size())), end(c));
  return IteratorRange(c.rbegin(), next(c.rbegin(), min<size_t>(max(top, 0), c.size())));
}