#include <test_runner.h>
#include <profile.h>

#include <iostream>
#include <string>
#include <deque>
#include <utility>
#include <set>
#include <cstdint>
using namespace std;

struct Event {
  int64_t time;
  string hotel_name;
  int client_id;
  int room_count;
};

class BookingManager {
public:
  BookingManager()
    : current_time(0) {}

  void Book(const Event& ev);
  int Clients (const string& hotel_name);
  int Rooms (const string& hotel_name);

private:
  int64_t current_time;
  deque<Event> events;
  map<string, int> booked_rooms;
  map<string, set<int>> clients;

  void Clean();
  void DeleteEvent(const Event& ev);
};


void BookingManager::Book(const Event& ev) {
  current_time = ev.time;
  Clean();
  events.push_back(ev);
  booked_rooms[ev.hotel_name] += ev.room_count;
  clients[ev.hotel_name].insert(ev.client_id);
}

void BookingManager::Clean() {
  deque<Event>::iterator it = events.begin();
  for(; it != events.end(); ) {
    if (current_time - it->time >= 86400) {
      DeleteEvent(*it);
      events.pop_front();
      it = events.begin();
    } else {
      break;
    }
  }
}

void BookingManager::DeleteEvent(const Event& ev) {
  booked_rooms[ev.hotel_name] -= ev.room_count;
  clients[ev.hotel_name].erase(ev.client_id);
}

int BookingManager::Clients (const string& hotel_name) {
  if(clients.count(hotel_name) == 0) return 0;
  return clients[hotel_name].size();
}

int BookingManager::Rooms (const string& hotel_name) {
  return booked_rooms[hotel_name];
}

void TestClients() {
  BookingManager bm;
  Event e1{ 0, "q", 0, 1 };
  bm.Book(e1);
  ASSERT_EQUAL(bm.Clients("q"), 1);
  ASSERT_EQUAL(bm.Rooms("q"), 1);
  Event e2{ 10, "q", 0, 3 };
  bm.Book(e2);
  ASSERT_EQUAL(bm.Clients("q"), 1);
  ASSERT_EQUAL(bm.Rooms("q"), 4);
  Event e3{ 86411, "q", 3, 1 };
  bm.Book(e3);
  ASSERT_EQUAL(bm.Clients("q"), 1);
  ASSERT_EQUAL(bm.Rooms("q"), 1);
}

void Test() {
    BookingManager bm;

    // Test empty bookings
    ASSERT_EQUAL(bm.Clients("mariott"), 0);
    ASSERT_EQUAL(bm.Rooms("mariott"), 0);

    bm.Book({0, "mariott", 1, 10});
    bm.Book({0, "mariott", 2, 1});
    bm.Book({0, "mariott", 3, 1});
    bm.Book({0, "mariott", 4, 1});
    bm.Book({0, "hilton", 1, 1});
    bm.Book({1, "hilton", 2, 1});

    // Test correctness
    ASSERT_EQUAL(bm.Clients("mariott"), 4);
    ASSERT_EQUAL(bm.Rooms("mariott"), 13);
    ASSERT_EQUAL(bm.Clients("hilton"), 2);
    ASSERT_EQUAL(bm.Rooms("hilton"), 2);

    // Test event past 1 day resets statics
    bm.Book({86400, "mariott", 1, 1});
    ASSERT_EQUAL(bm.Clients("mariott"), 1);
    ASSERT_EQUAL(bm.Rooms("mariott"), 1);
    ASSERT_EQUAL(bm.Clients("hilton"), 1);
    ASSERT_EQUAL(bm.Rooms("hilton"), 1);

    // Test no clients and room for the last day
    bm.Book({86401, "mariott", 5, 1});
    ASSERT_EQUAL(bm.Clients("mariott"), 2);
    ASSERT_EQUAL(bm.Rooms("mariott"), 2);
    ASSERT_EQUAL(bm.Clients("hilton"), 0);
    ASSERT_EQUAL(bm.Rooms("hilton"), 0);
}

int main() {
//  TestRunner tr;
//  RUN_TEST(tr, TestClients);
//  RUN_TEST(tr, Test);
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  BookingManager bm;
  int query_count;
  cin >> query_count;
  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    if(query_type == "BOOK") {
      int64_t time;
      string hotel_name;
      int client_id;
      int room_count;
      cin >> time >> hotel_name >> client_id >> room_count;
      bm.Book({time, hotel_name, client_id, room_count});
    } else if(query_type == "CLIENTS") {
      string hotel_name;
      cin >> hotel_name;
      cout << bm.Clients(hotel_name) << '\n';
    } else if (query_type == "ROOMS") {
      string hotel_name;
      cin >> hotel_name;
      cout << bm.Rooms(hotel_name) << '\n';
    }
  }
	return 0;
}
