//#include <test_runner.h>
//#include <profile.h>

#include <set>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class RouteManager {
public:
  void AddRoute(int start, int finish) {
    reachable_lists__[start].insert(finish);
    reachable_lists__[finish].insert(start);
  }

  int FindNearestFinish(int start, int finish) const {
      int result = abs(start - finish);
      if (reachable_lists__.count(start) < 1) {
          return result;
      }
      const set<int>& reachable_stations = reachable_lists__.at(start);
      const auto finish_pos = reachable_stations.lower_bound(finish);
         if (finish_pos != end(reachable_stations)) {
           result = min(result, abs(finish - *finish_pos));
         }
         if (finish_pos != begin(reachable_stations)) {
           result = min(result, abs(finish - *prev(finish_pos)));
         }
         return result;
    }

private:
  map<int, set<int>> reachable_lists__;
};

//void MakeTestFile() {
//  ofstream ofs("test.txt");
//  const int q = 1000000;
//  ofs << q << '\n';
//  for (int i = 0; i < q/2 - 1; ++i) {
//    ofs << "ADD " << pow((-1), i) * i * 5 * 3 << ' ' << i * 7 * 2 << '\n';
//  }
//  for (int i = 0; i < q/2 - 2; ++i) {
//    ofs << "GO " << pow((-1), i) * i * 3 << ' ' << i * 7 << '\n';
//  }
//  ofs << "GO " << 0 << ' ' << 7;
//}

//int main() {
////  MakeTestFile();
//  {
//  RouteManager routes;
//  int query_count;
//  ifstream fin("test.txt");
//  fin >> query_count;
//    LOG_DURATION("Old");
//  string com;
//  while(getline(fin, com)){
//    istringstream is(com);
//    string query_type;
//    is >> query_type;
//    int start, finish;
//    is >> start >> finish;
//    if (query_type == "ADD") {
//      routes.OldAddRoute(start, finish);
//    } else if (query_type == "GO") {
//      cout << routes.OldFindNearestFinish(start, finish) << "\n";
//    }
//  }
//  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
//  fin.close();
//  }
//
//  {
//  RouteManager routes;
//  int query_count;
//  ifstream fin("test.txt");
//  fin >> query_count;
//    LOG_DURATION("New");
//  string com;
//  while(getline(fin, com)){
//    istringstream is(com);
//    string query_type;
//    is >> query_type;
//    int start, finish;
//    is >> start >> finish;
//    if (query_type == "ADD") {
//      routes.AddRoute(start, finish);
//    } else if (query_type == "GO") {
////      cout << routes.FindNearestFinish(start, finish) << "\n";
//    }
//  }
//  fin.close();
//  }

//  return 0;
//}

int main() {
  RouteManager routes;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int start, finish;
    cin >> start >> finish;
    if (query_type == "ADD") {
      routes.AddRoute(start, finish);
    } else if (query_type == "GO") {
      cout << routes.FindNearestFinish(start, finish) << "\n";
    }
  }

  return 0;
}
