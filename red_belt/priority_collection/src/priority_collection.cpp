#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <list>
#include <map>

using namespace std;

int generateID()
{
    static int s_id = 0;
    return ++s_id;
}

template <typename T>
struct Entries {
  Entries(int pr) {
    priority = pr;
  }
  int priority;
  list<T> value;
};

template <typename T>
struct Element {
  int prio;
  T value;
  int id = generateID();
};

template <typename T>
bool operator < (const Entries<T>& lhs, const Entries<T>& rhs) {
  return lhs.priority < rhs.priority;
}


template <typename T>
class PriorityCollection {
public:
//  using Id = typename list<T>::iterator;/* тип, используемый для идентификаторов */;
  using Id = typename list<Element<T>>::iterator;
  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object) {
    auto it = data[0].insert(data[0].end(), {0, move(object)});
    return it;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin) {
    for (auto it = range_begin; it != range_end; it = next(it)) {
      Id id = Add(move(*it));
      *(ids_begin++) = id;
    }
  }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const {
    int prior = id->prio;
    if (data.count(prior) == 1) {
      for(auto it = data.at(prior).begin(); it != data.at(prior).end(); it++) {
        if (it == id) {
          return true;
        }
      }
    } else {
      return false;
    }
    return false;
  }

  // Получить объект по идентификатору
  const T& Get(Id id) const {
    return id->value;
  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
    id->prio++;
    data[id->prio].splice(data[id->prio].end(), data[id->prio - 1], id, next(id));
    if (data[id->prio - 1].size() == 0) {
      data.erase(id->prio - 1);
    }
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const {
    auto el = min_element(data.rbegin()->second.begin(), data.rbegin()->second.end(),
        [](auto& lhs, auto& rhs){
      return lhs.id > rhs.id;
    });
//    cout << el.value << " value" << endl;
    const T& t = el->value;
    return {t, data.rbegin()->first};
  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax() {
    auto el = min_element(data.rbegin()->second.begin(), data.rbegin()->second.end(),
        [](auto& lhs, auto& rhs){
      return lhs.id > rhs.id;
    });
//    int prior = data.rbegin()->first;
//    T val = move(data.rbegin()->second.back().value);
    pair<T, int> p = make_pair(move(el->value), data.rbegin()->first);
    data.rbegin()->second.erase(el);
    if (data.rbegin()->second.size() == 0) {
      data.erase(data.rbegin()->first);
    }
 return p;
  }

  const map<int, list<Element<T>>>& GetMap() {
    return data;
  }

private:
  map<int, list<Element<T>>> data;
//  list<T> data;
//  set<Entries<T>> data;
//  list<T> data;
  // Приватные поля и методы
};




class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}


int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}
