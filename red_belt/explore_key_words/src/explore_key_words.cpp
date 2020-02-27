#include "test_runner.h"
#include "profile.h"

#include <set>
#include <map>
#include <string>
#include <functional>
#include <future>
#include <sstream>
#include <algorithm>
#include <vector>
//#include <execution>
using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
    , size_(distance(first, last))
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

  size_t size() const {
    return size_;
  }

private:
  Iterator first, last;
  size_t size_;
};

template <typename Iterator>
class Paginator {
private:
  vector<IteratorRange<Iterator>> pages;

public:
  Paginator(Iterator begin, Iterator end, size_t page_size) {
    for (size_t left = distance(begin, end); left > 0; ) {
      size_t current_page_size = min(page_size, left);
      Iterator current_page_end = next(begin, current_page_size);
      pages.push_back({begin, current_page_end});

      left -= current_page_size;
      begin = current_page_end;
    }
  }

  auto begin() const {
    return pages.begin();
  }

  auto end() const {
    return pages.end();
  }

  size_t size() const {
    return pages.size();
  }
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
  return Paginator(begin(c), end(c), page_size);
}

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other) {
    for (auto& item : other.word_frequences) {
      word_frequences[item.first] += item.second;
    }
  }
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
  istringstream s(line);
  Stats stat;
  string word;
  while(getline(s ,word, ' ')) {
    if (key_words.count(word) == 1) {
      stat.word_frequences[word]++;
    }
  }
  return stat;
}

template <typename StringContainer>
Stats ExploreKeyWordsMultiThread(
    const set<string>& key_words, const StringContainer& strings)
{
  Stats sum;
  for (const auto& str : strings) {
    sum += ExploreLine(key_words, str);
  }
  return sum;
}


Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  vector<string> strings;
  for(string line; getline(input, line); ) {
    strings.push_back(move(line));
  }
  vector<future<Stats>> futures;
  for (auto page : Paginate(strings, 15000)) {
    futures.push_back(async([page, key_words]{
      return ExploreKeyWordsMultiThread(key_words, page);
    }));
  }
  Stats result;
  for (auto& f : futures) {
    result += f.get();
  }
  return result;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
