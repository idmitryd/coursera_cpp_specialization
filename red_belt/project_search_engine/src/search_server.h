#pragma once

#include <istream>
#include <ostream>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include <future>
#include <mutex>
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

class InvertedIndex {
public:
  void Add(vector<string> documents);
  const vector<pair<size_t, size_t>>& Lookup(string word) const;

  const string& GetDocument(size_t id) const {
    return docs[id];
  }

  size_t IndexSize() const {
    return docs.size();
  }

private:
  vector<string> docs;
  vector<pair<size_t, size_t>> empty_vector = {};
  map<string_view, vector<pair<size_t, size_t>>> index;

};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output) const;

private:
  InvertedIndex index;
};
