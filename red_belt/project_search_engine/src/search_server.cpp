#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <iterator>

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(move(line));
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

vector<string_view> SplitIntoWordsSV(string_view line, string delims = " ") {
  vector<string_view> output;
  size_t first = 0;
  while(first < line.size()) {
    const auto second = line.find_first_of(delims, first);
    if (first != second) {
      output.emplace_back(line.substr(first, second - first));
    }

    if (second == string_view::npos){
      break;
    }
    first = second + 1;
  }
  return output;
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  vector<string> docs;
  for (string current_document; getline(document_input, current_document); ) {
    docs.push_back(move(current_document));
  }
  InvertedIndex new_index;
  new_index.Add(move(docs));
  index = move(new_index);
}

void SearchServer::AddQueriesStream (
  istream& query_input, ostream& search_results_output
) const {

  vector<int> docid_count(index.IndexSize());
  for (string current_query; getline(query_input, current_query); ) {

    for (auto& word : SplitIntoWords(current_query)) {
      vector<pair<size_t, size_t>> docid_for_word = index.Lookup(move(word));
      for (const auto& item : docid_for_word) {
        docid_count[item.first] += item.second;
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

    search_results_output << move(current_query) << ':';
    for (const auto& [docid, hitcount] : Head(search_results, 5)) {
      search_results_output << " {"
        << "docid: " << docid << ", "
        << "hitcount: " << hitcount << '}';
    }
    search_results_output << endl;
  }

}

void InvertedIndex::Add(vector<string> documents) {
  docs = move(documents);
  map<string_view, map<int,int>> temp;
  int doc_id = -1;
  for (const string& doc : docs) {
    doc_id ++;
    for (auto word : SplitIntoWordsSV(string_view(doc))) {
      temp[word][doc_id]++;
    }
  }
  for (auto& item : temp) {
    index[item.first].insert(
        index[item.first].begin(),
        make_move_iterator(item.second.begin()),
        make_move_iterator(item.second.end()));
  }
}


const vector<pair<size_t, size_t>>& InvertedIndex::Lookup(string word) const{
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return empty_vector;
  }
}
