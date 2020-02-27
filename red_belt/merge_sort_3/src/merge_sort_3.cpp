#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <iterator>

using namespace std;

template <typename RandomIt>
using It_type = typename RandomIt::value_type;


template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  int size = range_end - range_begin;
  if (size < 2) return;
  vector<It_type<RandomIt>> elements(
      make_move_iterator(range_begin),
      make_move_iterator(range_end)
      );
  auto it_first_third = elements.begin() + size / 3;
  auto it_second_third = elements.begin() + size * 2 / 3;

  MergeSort(elements.begin(), it_first_third);
  MergeSort(it_first_third, it_second_third);
  MergeSort(it_second_third, elements.end());

  vector<It_type<RandomIt>> inter;
  merge(
      make_move_iterator(elements.begin()), make_move_iterator(it_first_third),
      make_move_iterator(it_first_third), make_move_iterator(it_second_third),
      back_inserter(inter)
      );

  merge(
      make_move_iterator(inter.begin()), make_move_iterator(inter.end()),
      make_move_iterator(it_second_third), make_move_iterator(elements.end()),
        range_begin);
  // Напишите реализацию функции,
  // не копируя сортируемые элементы
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
