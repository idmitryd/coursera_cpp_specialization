#include <test_runner.h>
#include <profile.h>

#include <iomanip>
#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

//class ReadingManager {
//public:
//  ReadingManager()
//      : user_page_counts_(MAX_USER_COUNT_ + 1, 0),
//        sorted_users_(),
//        user_positions_(MAX_USER_COUNT_ + 1, -1) {}
//
//  void Read(int user_id, int page_count) {
//    if (user_page_counts_[user_id] == 0) {
//      AddUser(user_id);
//    }
//    user_page_counts_[user_id] = page_count;
//    int& position = user_positions_[user_id];
//    while (position > 0 && page_count > user_page_counts_[sorted_users_[position - 1]]) {
//      SwapUsers(position, position - 1);
//    }
//  }
//
//  double Cheer(int user_id) const {
//    if (user_page_counts_[user_id] == 0) {
//      return 0;
//    }
//    const int user_count = GetUserCount();
//    if (user_count == 1) {
//      return 1;
//    }
//    const int page_count = user_page_counts_[user_id];
//    int position = user_positions_[user_id];
//    while (position < user_count &&
//      user_page_counts_[sorted_users_[position]] == page_count) {
//      ++position;
//    }
//    if (position == user_count) {
//        return 0;
//    }
//    // По умолчанию деление целочисленное, поэтому
//    // нужно привести числитель к типу double.
//    // Простой способ сделать это — умножить его на 1.0.
//    return (user_count - position) * 1.0 / (user_count - 1);
//  }
//
//private:
//  // Статическое поле не принадлежит какому-то конкретному
//  // объекту класса. По сути это глобальная переменная,
//  // в данном случае константная.
//  // Будь она публичной, к ней можно было бы обратиться снаружи
//  // следующим образом: ReadingManager::MAX_USER_COUNT.
//  static const int MAX_USER_COUNT_ = 100'000;
//
//  vector<int> user_page_counts_;
//  vector<int> sorted_users_;   // отсортированы по убыванию количества страниц
//  vector<int> user_positions_; // позиции в векторе sorted_users_
//
//  int GetUserCount() const {
//    return sorted_users_.size();
//  }
//  void AddUser(int user_id) {
//    sorted_users_.push_back(user_id);
//    user_positions_[user_id] = sorted_users_.size() - 1;
//  }
//  void SwapUsers(int lhs_position, int rhs_position) {
//    const int lhs_id = sorted_users_[lhs_position];
//    const int rhs_id = sorted_users_[rhs_position];
//    swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
//    swap(user_positions_[lhs_id], user_positions_[rhs_id]);
//  }
//};

////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class ReadingManager {
public:
  ReadingManager() {}

  void Read(int user_id, int page_count) {
    if (users_.count(user_id) == 1) {
      int& user_page = users_.at(user_id);
      int& number_of_users = pages_.at(user_page);
      --number_of_users;
      if (number_of_users <= 0) {
        pages_.erase(user_page);
      }
    }
    users_[user_id] = page_count;
    ++pages_[page_count];
  }

  double Cheer(int user_id) const {
    if (users_.count(user_id) == 0) {
      return 0.;
    }
    const int user_count = users_.size();
    if (user_count == 1) {
      return 1.;
    }
    const int page_count = users_.at(user_id);
    int counter = 0;
    const auto it_end = pages_.lower_bound(page_count);
    for (auto it = pages_.begin(); it != it_end; it = next(it)) {
      counter += it->second;
    }
    return 1.0 * (counter) / (1.0 * (user_count - 1));
  }

private:
  map<int, int> users_;
  map <int, int> pages_;
};


//void BasicTestOld() {
//  ReadingManager manager;
//  ifstream fin("test.txt");
//  ostringstream outs;
//  int query_count;
//  fin >> query_count;
//  string command;
//  while (getline(fin, command)) {
//        istringstream is(command);
//        string query_type;
//        is >> query_type;
//        int user_id;
//        is >> user_id;
//
//        if (query_type == "READ") {
//          int page_count;
//          is >> page_count;
//          manager.Read(user_id, page_count);
//        } else if (query_type == "CHEER") {
//          outs << setprecision(6) << manager.Cheer(user_id) << "\n";
//        }
//      }
//  fin.close();
//  string res = "0\n1\n0\n0.5\n0.5\n1\n0.5\n";
//  ASSERT_EQUAL(outs.str(), res);
//}
//
//void BasicTestNew() {
//  NewReadingManager manager;
//  ifstream fin("test.txt");
//  ostringstream outs;
//  int query_count;
//  fin >> query_count;
//  string command;
//  while (getline(fin, command)) {
//        istringstream is(command);
//        string query_type;
//        is >> query_type;
//        int user_id;
//        is >> user_id;
//
//        if (query_type == "READ") {
//          int page_count;
//          is >> page_count;
//          manager.Read(user_id, page_count);
////          manager.Finalize();
//        } else if (query_type == "CHEER") {
//
//          outs << setprecision(6) << manager.Cheer(user_id) << "\n";
//        }
//      }
//  fin.close();
//  string res = "0\n1\n0\n0.5\n0.5\n1\n0.5\n";
//
//  ASSERT_EQUAL(outs.str(), res);
//}
//
//void MakeBigFile() {
//  const int MAX = 5000;
//  ofstream of("big.txt");
//  of << 2 * MAX << '\n';
//  for(int i = 0; i < MAX; ++i) {
//    of << "READ " << i * 10 << ' ' << i * 100 << endl;
//  }
//  for(int i = 0; i < MAX; ++i) {
//      of << "CHEER " << i * 10 << endl;
//    }
//
//}
//
//void TimeTestOld() {
//  ReadingManager manager;
//  ifstream fin("big.txt");
//  LOG_DURATION("Old time");
//  int query_count;
//  fin >> query_count;
//  string command;
//  while (getline(fin, command)) {
//        istringstream is(command);
//        string query_type;
//        is >> query_type;
//        int user_id;
//        is >> user_id;
//        if (query_type == "READ") {
//          int page_count;
//          is >> page_count;
//          manager.Read(user_id, page_count);
//        } else if (query_type == "CHEER") {
//          manager.Cheer(user_id);
//        }
//      }
//  fin.close();
//}
//
//void TimeTestNew() {
//  NewReadingManager manager;
//  ifstream fin("big.txt");
//  LOG_DURATION("New time");
//  int query_count;
//  fin >> query_count;
//  string command;
//  while (getline(fin, command)) {
//        istringstream is(command);
//        string query_type;
//        is >> query_type;
//        int user_id;
//        is >> user_id;
//        if (query_type == "READ") {
//          int page_count;
//          is >> page_count;
//          manager.Read(user_id, page_count);
//        } else if (query_type == "CHEER") {
//          manager.Cheer(user_id);
//        }
//      }
//  fin.close();
//
//}
//
//int main() {
////  MakeBigFile();
//  TestRunner tr;
//  RUN_TEST(tr, BasicTestOld);
//  TimeTestOld();
//  RUN_TEST(tr, BasicTestNew);
//  TimeTestNew();
//    // Для ускорения чтения данных отключается синхронизация
//    // cin и cout с stdio,
//    // а также выполняется отвязка cin от cout
////    ios::sync_with_stdio(false);
////    cin.tie(nullptr);
////
////    ReadingManager manager;
////    int query_count;
////    ifstream fin("test.txt");
////    fin >> query_count;
////    string command;
////    while (getline(fin, command)) {
////      istringstream is(command);
////      string query_type;
////      is >> query_type;
////      int user_id;
////      is >> user_id;
////
////      if (query_type == "READ") {
////        int page_count;
////        is >> page_count;
////        manager.Read(user_id, page_count);
////      } else if (query_type == "CHEER") {
////        cout << setprecision(6) << manager.Cheer(user_id) << "\n";
////      }
////    }
////    fin.close();
//  return 0;
//}


int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;

  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}
