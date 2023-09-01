#pragma once

#include <map>
#include <string>
#include <unordered_set>

#include "../syntax/ast/nodes.h"

class VariableEnvironment {
 public:
  VariableEnvironment() {}

  void update(std::string name) { this->set.insert(name); }
  bool contains(std::string name) {
    return this->set.find(name) != this->set.end();
  }
  void clear() { this->set.clear(); }

 private:
  std::unordered_set<std::string> set;
};

class FunctionEnvironment {
 public:
  FunctionEnvironment() {}

  void update(std::string name, FunctionSignature s) {
    auto itr = map.find(name);
    if (itr == map.end())
      map.insert(std::make_pair(name, s));
    else
      itr->second = s;
  }
  bool contains(std::string name) { return map.find(name) != map.end(); }
  void clear() { this->map.clear(); }

 private:
  std::map<std::string, FunctionSignature> map;
};
