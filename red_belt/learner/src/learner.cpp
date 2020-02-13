#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <set>

#include <profile.h>
#include <test_runner.h>

using namespace std;

class Learner {
 private:
  set<string> dict;

 public:
  int Learn(const vector<string>& words) {
    int old_size = dict.size();
    dict.insert(words.begin(), words.end());
    return dict.size() - old_size;
  }

  vector<string> KnownWords() {
    return {dict.begin(), dict.end()};
  }
};


void MakeTestFile() {
  ofstream o("test.txt");
  for (int i = 0; i < 10000; ++i) {
    o << to_string(i) << " ";
  }
  o.close();
}

int main() {

//  MakeTestFile();
  Learner learner;
  string line;
  ifstream fin("test.txt");
  while (getline(fin, line)) {
    vector<string> words;
    stringstream ss(line);
    string word;
    while (ss >> word) {
      words.push_back(word);
    }
    {
      LOG_DURATION("Learn");
      cout << learner.Learn(words) << "\n";
    }
  }
  vector<string> knownWords;
  {
    LOG_DURATION("KnownWords")
    knownWords = learner.KnownWords();
  }
  cout << "=== known words ===\n";
  for (auto word : learner.KnownWords()) {
    cout << word << "\n";
  }
  fin.close();
}
