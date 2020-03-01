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

void UpdateDataBase(
    istream& document_input,
    Synchronized<InvertedIndex>& index_sync
)  {
  InvertedIndex new_index(document_input);
  swap(index_sync.GetAccess().ref_to_value, new_index);
}

void ProcessQueries(
    istream& query_input,
    ostream& search_results_output,
    Synchronized<InvertedIndex>& index_sync
) {
  vector<int> docid_count;
  vector<pair<size_t, size_t>> search_results;
  for (string current_query; getline(query_input, current_query); ) {
    vector<string_view> words = SplitIntoWordsSV(current_query);
    {
      auto ind_acc = index_sync.GetAccess();
      size_t doc_size = ind_acc.ref_to_value.IndexSize();
      docid_count.assign(doc_size, 0);
      for (auto& word : words) {
        for (const auto& item : ind_acc.ref_to_value.Lookup(move(word))) {
          docid_count[item.first] += item.second;
        }
      }
    }
    search_results.clear();
    for (size_t i = 0; i < docid_count.size(); ++i) {
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
      search_results_output << '\n';
    }
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  futures.push_back(
      async(
          UpdateDataBase, ref(document_input), ref(index_sync)
          )
  );
}

void SearchServer::AddQueriesStream (
  istream& query_input, ostream& search_results_output
)  {
  futures.push_back(
      async(
          ProcessQueries, ref(query_input), ref(search_results_output), ref(index_sync)
      )
  );
//  ProcessQueries(query_input, search_results_output, index_sync);
}

InvertedIndex::InvertedIndex(istream& document_input) {
  for (string current_document; getline(document_input, current_document); ) {
    docs.push_back(move(current_document));
  }
  map<string_view, map<size_t,size_t>> temp;
    size_t doc_id = 0;
    for (const string& doc : docs) {
      for (auto word : SplitIntoWordsSV(string_view(doc))) {
        temp[word][doc_id]++;
      }
      doc_id++;
    }
    for (auto& item : temp) {
      index[item.first].insert(
          index[item.first].begin(),
          make_move_iterator(item.second.begin()),
          make_move_iterator(item.second.end()));
    }

}

const vector<pair<size_t, size_t>>& InvertedIndex::Lookup(string_view word) const{
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return empty_vector;
  }
}
