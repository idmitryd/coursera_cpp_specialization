#include "test_runner.h"
#include "profile.h"

#include <algorithm>
#include <numeric>
#include <vector>
#include <string>
#include <random>
#include <mutex>
#include <future>
//#include <type_traits>
using namespace std;

template <typename T>
class Synchronized {
public:
  explicit Synchronized(T initial = T()) : value(move(initial)) {
  }

  struct Access {
    T& ref_to_value;
    lock_guard<mutex> g;
  };

  Access GetAccess() {
    return {value, lock_guard<mutex>(m)};
  }
private:
  mutex m;
  T value;
};

template <typename K, typename V>
class ConcurrentMap {
public:
//  static_assert(is_integral_v<K>, "ConcurrentMap supports only integer keys");

  struct Access {
    V& ref_to_value;
    lock_guard<mutex> g;
  };

  explicit ConcurrentMap(size_t bucket_count)
      : bucket_count_(bucket_count)
      , buckets(bucket_count)
      , map_mutexes(bucket_count)
      , value_mutexes(bucket_count) {
   }

  Access operator[](const K& key) {
    int bucket_number = key % bucket_count_;
    lock_guard<mutex> g(map_mutexes[bucket_number]);
    map<K, V>& bucket = buckets[bucket_number];
    if (bucket.count(key) == 1) {
      return {bucket[key], lock_guard<mutex>(value_mutexes[bucket_number])};
    } else {
      bucket[key] = V();
      return {bucket[key], lock_guard<mutex>(value_mutexes[bucket_number])};
    }
  }

  map<K, V> BuildOrdinaryMap() {
    map<K, V> OrdinaryMap;
    for (size_t i = 0; i < bucket_count_; ++i) {
      lock_guard<mutex> g_map(map_mutexes[i]);
      lock_guard<mutex> g_value(value_mutexes[i]);
      map<K, V>& bucket = buckets[i];
      for (auto item : bucket) {
        OrdinaryMap[item.first] = item.second;
      }
    }
    return OrdinaryMap;
  }
private:
  const size_t bucket_count_;
  vector<map<K, V>> buckets;
  vector<mutex> map_mutexes;
  vector<mutex> value_mutexes;
};


void RunConcurrentUpdates(
    ConcurrentMap<int, int>& cm, size_t thread_count, int key_count
) {
  auto kernel = [&cm, key_count](int seed) {
    vector<int> updates(key_count);
    iota(begin(updates), end(updates), -key_count / 2);
    shuffle(begin(updates), end(updates), default_random_engine(seed));

    for (int i = 0; i < 2; ++i) {
      for (auto key : updates) {
        cm[key].ref_to_value++;
      }
    }
  };

  vector<future<void>> futures;
  for (size_t i = 0; i < thread_count; ++i) {
    futures.push_back(async(kernel, i));
  }
}

void TestConcurrentUpdate() {
  const size_t thread_count = 3;
  const size_t key_count = 50000;

  ConcurrentMap<int, int> cm(thread_count);
  RunConcurrentUpdates(cm, thread_count, key_count);

  const auto result = cm.BuildOrdinaryMap();
  ASSERT_EQUAL(result.size(), key_count);
  for (auto& [k, v] : result) {
    AssertEqual(v, 6, "Key = " + to_string(k));
  }
}

void TestReadAndWrite() {
  ConcurrentMap<size_t, string> cm(5);

  auto updater = [&cm] {
    for (size_t i = 0; i < 50000; ++i) {
      cm[i].ref_to_value += 'a';
    }
  };
  auto reader = [&cm] {
    vector<string> result(50000);
    for (size_t i = 0; i < result.size(); ++i) {
      result[i] = cm[i].ref_to_value;
    }
    return result;
  };

  auto u1 = async(updater);
  auto r1 = async(reader);
  auto u2 = async(updater);
  auto r2 = async(reader);

  u1.get();
  u2.get();

  for (auto f : {&r1, &r2}) {
    auto result = f->get();
    ASSERT(all_of(result.begin(), result.end(), [](const string& s) {
      return s.empty() || s == "a" || s == "aa";
    }));
  }
}

void TestSpeedup() {
  {
    ConcurrentMap<int, int> single_lock(1);

    LOG_DURATION("Single lock");
    RunConcurrentUpdates(single_lock, 4, 50000);
  }
  {
    ConcurrentMap<int, int> many_locks(100);

    LOG_DURATION("100 locks");
    RunConcurrentUpdates(many_locks, 4, 50000);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestConcurrentUpdate);
  RUN_TEST(tr, TestReadAndWrite);
  RUN_TEST(tr, TestSpeedup);
}
