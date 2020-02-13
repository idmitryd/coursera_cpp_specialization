#pragma once

#include <stdexcept>
#include <array>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
  explicit StackVector(size_t a_size = 0)
      : size(a_size) {
    if(size > N) {
      throw invalid_argument("");
    }
  };

  T& operator[](size_t index) {
    if (index > size - 1) {
      throw invalid_argument("");
    }
    return sv[index];
  }
  const T& operator[](size_t index) const {
    if (index > size - 1) {
      throw invalid_argument("");
    }
    return sv[index];
  }

  T* begin() {
    return &sv[0];
  }
  T* end() {
    return &sv[size];
  }
  const T* begin() const {
    return &sv[0];
  }
  const T* end() const {
    return &sv[size];
  }

  size_t Size() const {
    return size;
  }
  size_t Capacity() const {
    return sv.size();
  }

  void PushBack(const T& value) {
    if (size == sv.size()) {
      throw overflow_error("");
    }
    sv[size++] = value;
  }

  T PopBack() {
    if (size == 0) {
      throw underflow_error("");
    }
    return sv[--size];
  }

private:
  size_t size;
  array<T, N> sv;
};

