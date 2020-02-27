#include "search_server.h"
#include "iterator_range.h"

#include <set>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }

  index = move(new_index);
}

void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
  vector<int> docid_count(index.IndexSize());
  for (string current_query; getline(query_input, current_query); ) {

    for (auto& word : SplitIntoWords(current_query)) {
      for (const size_t docid : index.Lookup(move(word))) {
        docid_count[docid]++;
      }
    }

    vector<pair<size_t, size_t>> search_results;
    for (size_t i = 0; i < index.IndexSize(); ++i) {
      if (docid_count[i] > 0) {
        search_results.push_back({i, docid_count[i]});
        docid_count[i] = 0;
      }
    }
    partial_sort(
      begin(search_results),
      next(begin(search_results), min<size_t>(5, search_results.size())),
      end(search_results),
      [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
        int64_t lhs_docid = lhs.first;
        auto lhs_hit_count = lhs.second;
        int64_t rhs_docid = rhs.first;
        auto rhs_hit_count = rhs.second;
        return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
      }
    );

    search_results_output << current_query << ':';
    for (auto [docid, hitcount] : Head(search_results, 5)) {
      search_results_output << " {"
        << "docid: " << docid << ", "
        << "hitcount: " << hitcount << '}';
    }
//    set<pair<int64_t, int64_t>> search_results_;
//    for (const auto& item : docid_count) {
//      search_results_.insert({item.second, -item.first});
//    }
//
//    search_results_output << current_query << ':';
//    for(auto& item : Tail(search_results_, 5)) {
//      search_results_output << " {"
//          << "docid: " << -item.second << ", "
//          << "hitcount: " << item.first << '}';
//    }
    search_results_output << endl;
  }

}

void InvertedIndex::Add(const string& document) {
  docs.push_back(document);

  const size_t docid = docs.size() - 1;
  for (const auto& word : SplitIntoWords(document)) {
    index[word].push_back(docid);
  }
}

list<size_t> InvertedIndex::Lookup(const string& word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return {};
  }
}
