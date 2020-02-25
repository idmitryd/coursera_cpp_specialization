#pragma once

#include <cstdlib>
#include <algorithm>

using namespace std;

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector();
  explicit SimpleVector(size_t size);
  SimpleVector(const SimpleVector& other);
  ~SimpleVector();

  void operator=(const SimpleVector& other);
  T& operator[](size_t index);

  T* begin();
  T* end();
  T* begin() const;
  T* end() const;

  size_t Size() const;
  size_t Capacity() const;
  void PushBack(const T& value);

private:
  size_t capacity_;
  size_t size_;
  T* begin_;
  T* end_;
  void SetSize(size_t x) {
    size_ = x;
  }

  void SetCapacity(size_t x) {
    capacity_ = x;
  }

};

template <typename T>
SimpleVector<T>::SimpleVector()
    : capacity_(0)
    , size_(0)
    , begin_(nullptr)
    , end_(nullptr) {
}

template <typename T>
SimpleVector<T>::SimpleVector(size_t size) {
  size_ = size;
  capacity_ = size;
  begin_ = new T[size_];
  end_ = begin_ + size_;
}

template <typename T>
SimpleVector<T>::SimpleVector(const SimpleVector<T>& other) {
  size_ = other.size_;
  capacity_ = other.capacity_;
  begin_ = new T[other.capacity_];
  end_ = begin_ + other.size_;
  copy(other.begin(), other.end(), begin());
}

template <typename T>
SimpleVector<T>::~SimpleVector() {
  delete[] begin_;
}

template <typename T>
void SimpleVector<T>::operator=(const SimpleVector<T>& other) {
  if (other.size_ <= capacity_) {
       // У нас достаточно памяти - просто копируем элементы
       copy(other.begin(), other.end(), begin());
       size_ = other.size_;
     } else {
       // Это так называемая идиома copy-and-swap.
       // Мы создаём временный вектор с помощью
       // конструктора копирования, а затем обмениваем его поля со своими.
       // Так мы достигаем двух целей:
       //  - избегаем дублирования кода в конструкторе копирования
       //    и операторе присваивания
       //  - обеспечиваем согласованное поведение конструктора копирования
       //    и оператора присваивания
       SimpleVector<T> tmp(other);
       swap(tmp.begin_, begin_);
       swap(tmp.end_, end_);
       swap(tmp.size_, size_);
       swap(tmp.capacity_, capacity_);

     }
}

template <typename T>
T& SimpleVector<T>::operator[] (size_t index) {
  return begin_[index];
}

template <typename T>
T* SimpleVector<T>::begin() {
  return begin_;
}

template <typename T>
T* SimpleVector<T>::end() {
  return end_;
}

template <typename T>
T* SimpleVector<T>::begin() const {
  return begin_;
}

template <typename T>
T* SimpleVector<T>::end() const {
  return end_;
}

template <typename T>
size_t SimpleVector<T>::Size() const {
  return size_;
}

template <typename T>
size_t SimpleVector<T>::Capacity() const {
  return capacity_;
}

template <typename T>
void SimpleVector<T>::PushBack(const T& value) {
  if (Size() == Capacity()) {

    SetCapacity(Capacity() == 0 ? 1 : Capacity() * 2);
    T* new_begin = new T[Capacity()];
    std::copy(begin_, end_, new_begin);
    delete[] begin_;
    begin_ = new_begin;
    SetSize(Size() + 1);
    begin_[Size() - 1] = value;
    end_ = begin_ + Size();
  } else {
    SetSize(Size() + 1);
    begin_[Size() - 1] = value;
    end_ = begin_ + Size();
  }
}

