#pragma once

#include <cstdlib>
#include <algorithm>
#include <iterator>

using namespace std;

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector();
  explicit SimpleVector(size_t size);
//  SimpleVector(const SimpleVector& other);
  ~SimpleVector();

//  void operator=(const SimpleVector& other);
  T& operator[](size_t index);

  T* begin();
  T* end();
  T* begin() const;
  T* end() const;

  size_t Size() const;
  size_t Capacity() const;
  void PushBack(T value);
//  void PushBack(T&& value);

private:
  size_t capacity_;
  size_t size_;
  T* begin_;
};

template <typename T>
SimpleVector<T>::SimpleVector()
    : capacity_(0)
    , size_(0)
    , begin_(nullptr)
{
}

template <typename T>
SimpleVector<T>::SimpleVector(size_t size)
    : size_(size)
    , capacity_(size)
    , begin_(new T[size])
{
}

//template <typename T>
//SimpleVector<T>::SimpleVector(const SimpleVector<T>& other) {
//  size_ = other.size_;
//  capacity_ = other.capacity_;
//  begin_ = new T[other.capacity_];
//  copy(other.begin(), other.end(), begin());
//}

template <typename T>
SimpleVector<T>::~SimpleVector() {
  delete[] begin_;
}

//template <typename T>
//void SimpleVector<T>::operator=(const SimpleVector<T>& other) {
//  if (other.size_ <= capacity_) {
//       // У нас достаточно памяти - просто копируем элементы
//       copy(other.begin(), other.end(), begin());
//       size_ = other.size_;
//     } else {
//       // Это так называемая идиома copy-and-swap.
//       // Мы создаём временный вектор с помощью
//       // конструктора копирования, а затем обмениваем его поля со своими.
//       // Так мы достигаем двух целей:
//       //  - избегаем дублирования кода в конструкторе копирования
//       //    и операторе присваивания
//       //  - обеспечиваем согласованное поведение конструктора копирования
//       //    и оператора присваивания
//       SimpleVector<T> tmp(other);
//       swap(tmp.begin_, begin_);
//       swap(tmp.size_, size_);
//       swap(tmp.capacity_, capacity_);
//
//     }
//}

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
  return begin_ + size_;
}

template <typename T>
T* SimpleVector<T>::begin() const {
  return begin_;
}

template <typename T>
T* SimpleVector<T>::end() const {
  return begin_ + size_;
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
void SimpleVector<T>::PushBack(T value) {
  if (size_ >= capacity_) {
    capacity_  = capacity_ == 0 ? 1 : capacity_  * 2;
    T* new_begin = new T[capacity_];
    std::move(
        begin(),
        end(),
        new_begin
        );
    delete[] begin_;
    begin_ = new_begin;
    begin_[size_++] = move(value);
  } else {
    begin_[size_++] = move(value);
  }
}
