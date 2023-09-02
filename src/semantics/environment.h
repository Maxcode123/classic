#pragma once

#include <map>
#include <string>
#include <unordered_set>

#include "../syntax/ast/nodes.h"

template <class T>
class Environment {
 public:
  void update(std::string name, T t) {
    auto itr = map.find(name);
    if (itr == map.end())
      map.insert(std::make_pair(name, t));
    else
      itr->second = s;
  }
  bool contains(std::string name) { return map.find(name) != map.end(); }
  T get(std::string name) {
    auto v = map.find(name);
    if (v != map.end()) return v->second;
    return nullptr;
  }
  void clear() { this->map.clear(); }

 protected:
  std::map<std::string, T> map;
};

template <class T>
class EnvironmentProxy {
 public:
  void update(std::string name, T t) { env->update(name, t); }
  bool contains(std::string name) { return env->contains(name); }
  T get(std::string name) { return env->get(name); }
  void clear() { env->clear(); }

 protected:
  T env;
};

typedef class VariableEnvironment_
    : public Environment<ClassicType>{public : VariableEnvironment_(){}} *
      VariableEnvironment;

typedef class FunctionSignature_ {
 public:
  FunctionSignature_(Function func)
      : FunctionSignature_(func->name, func->return_type, func->param_list) {}
  FunctionSignature_(std::string n, ClassicType t, ParamList p) {
    name = n;
    return_type = t;
    param_list = p;
  }
  std::string name;
  ClassicType return_type;
  ParamList param_list;
}* FunctionSignature;

typedef class FunctionEnvironment_
    : public Environment<FunctionSignature>{public : FunctionEnvironment_(){}} *
      FunctionEnvironment;

class VariableEnvironmentProxy
    : protected EnvironmentProxy<VariableEnvironment> {
 public:
  VariableEnvironmentProxy(VariableEnvironment e) { env = e; }
  VariableEnvironmentProxy() {}
};

class FunctionEnvironmentProxy
    : protected EnvironmentProxy<FunctionEnvironment> {
 public:
  FunctionEnvironmentProxy(FunctionEnvironment e) { env = e; }
  FunctionEnvironmentProxy() {}
};
