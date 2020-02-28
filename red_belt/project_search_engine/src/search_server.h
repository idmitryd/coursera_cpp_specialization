#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <utility>
using namespace std;

class InvertedIndex {
public:
//  InvertedIndex() {}
//  InvertedIndex(vector<string>& documents) {
//    docs = move(documents);
//
//  }
//  void Add(string document);

  void Add(vector<string> documents);

  const vector<pair<size_t, size_t>>& Lookup(string word) const;

  const string& GetDocument(size_t id) const {
    return docs[id];
  }

  size_t IndexSize() const {
    return docs.size();
  }

private:
  vector<pair<size_t, size_t>> empty_vector = {};
//  map<string, list<size_t>> index; //Почему не вектор?
  vector<string> docs;
//  map<string, map<int, int>> index_;
//  map<string, vector<int>> index__;
  map<string_view, vector<pair<size_t, size_t>>> index;

};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  InvertedIndex index;
};
