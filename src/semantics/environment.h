#pragma once

#include <string>
#include <unordered_set>

class Environment {
 public:
  Environment() {}

  void update(std::string name) { this->set.insert(name); }
  bool contains(std::string name) {
    return this->set.find(name) != this->set.end();
  }
  void clear() { this->set.clear(); }

 private:
  std::unordered_set<std::string> set;
};
