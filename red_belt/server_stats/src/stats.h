#pragma once

#include "http_request.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string_view>
#include <map>
using namespace std;

class Stats {
public:
  Stats() {
    for (const string& s : methods_) {
      methods[s] = 0;
    }
    for (const string& s : uris_) {
      uris[s] = 0;
    }
  }
  void AddMethod(string_view method);
  void AddUri(string_view uri);
  const map<string_view, int>& GetMethodStats() const;
  const map<string_view, int>& GetUriStats() const;
private:
  map<string_view, int> methods;
  map<string_view, int> uris;
  vector<string> methods_ = {"GET", "POST", "PUT", "DELETE", "UNKNOWN"};
  vector<string> uris_ = {"/", "/order", "/product", "/basket", "/help", "unknown"};
};

HttpRequest ParseRequest(string_view line);
