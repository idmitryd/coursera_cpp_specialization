#include <stats.h>

void Stats::AddMethod(string_view method) {
  auto it = find(methods_.begin(), methods_.end(), method);
  if (it == methods_.end()) {
    methods["UNKNOWN"]++;
    return;
  }
  methods[*it]++;
}

void Stats::AddUri(string_view uri) {
  auto it = find(uris_.begin(), uris_.end(), uri);
  if (it == uris_.end()) {
    uris["unknown"]++;
    return;
  }
  uris[*it]++;

}

const map<string_view, int>& Stats::GetMethodStats() const {
  return methods;
}
const map<string_view, int>& Stats::GetUriStats() const {
  return uris;
}

HttpRequest ParseRequest(string_view line) {
  HttpRequest req;
  size_t pos = line.find_first_not_of(" ", 0);
  size_t space = line.find(' ', pos);
  req.method = line.substr(pos, space - pos);
  pos = space + 1;
  space = line.find(' ', pos);
  req.uri = line.substr(pos, space - pos);
  pos = space + 1;
  space = line.find(' ', pos);
  req.protocol = line.substr(pos, space - pos);
  return req;
}
