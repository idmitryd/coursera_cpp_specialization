#include "test_runner.h"
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

template <typename T>
class Table {
public:
  Table(size_t cols, size_t rows) {
    table.resize(cols);
    for (auto& v : table) {
      v.resize(rows, T());
    }
  }

  pair<size_t, size_t> Size() const {
    return {table.size(), table.empty() ? 0 : table[0].size()};
  }

  void Resize(size_t new_cols, size_t new_rows) {
    table.resize(new_cols);
    for (auto& v : table) {
      v.resize(new_rows, T());
    }
  }

  std::vector<T>& operator [] (size_t ind) {
    return table[ind];
  }

  const std::vector<T>& operator [] (size_t ind) const {
    return table[ind];
  }

private:
  std::vector<vector<T>> table;
};



void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTable);
  return 0;
}
