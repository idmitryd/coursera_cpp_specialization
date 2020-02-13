#include "test_runner.h"
#include <string>
#include <deque>
#include <vector>
#include <string_view>
#include <map>
#include <algorithm>

using namespace std;

class Translator {
public:
  Translator() {}
  void Add(string_view source, string_view target);
  string_view TranslateForward(string_view source) const;
  string_view TranslateBackward(string_view target) const;

private:
  deque<string> words;
  map<string_view, vector<string_view>> source_to_target;
  map<string_view, vector<string_view>> target_to_source;
};

void Translator::Add(string_view source, string_view target) {
  auto it_s = find(words.begin(), words.end(), source);
  if (it_s == words.end()) {
    words.push_back(string(source));
    it_s = prev(words.end());
  }
  auto it_t = find(words.begin(), words.end(), target);
  if (it_t == words.end()) {
    words.push_back(string(target));
    it_t = prev(words.end());
  }
  string_view source_sv(*it_s);
  string_view target_sv(*it_t);

  source_to_target[source_sv].push_back(target_sv);
  target_to_source[target_sv].push_back(source_sv);
}

string_view Translator::TranslateForward(string_view source) const {
  if(source_to_target.count(source) == 0) {
    return "";
  }
  return source_to_target.at(source).back();
}

string_view Translator::TranslateBackward(string_view target) const {
  if(target_to_source.count(target) == 0) {
    return "";
  }
  return target_to_source.at(target).back();
}


void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}
