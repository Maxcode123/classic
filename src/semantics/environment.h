#pragma once

#include <map>
#include <string>
#include <unordered_set>

#include "../syntax/ast/nodes.h"
#include "../utils/macros.h"

DECL_TYPEDEF(VariableEnvironment)
DECL_TYPEDEF(FunctionSignature)
DECL_TYPEDEF(FunctionEnvironment)

template <class T>
class Environment {
 public:
  typedef T T_type;
  void update(std::string name, T t) {
    auto itr = map.find(name);
    if (itr == map.end())
      map.insert(std::make_pair(name, t));
    else
      itr->second = t;
  }
  bool contains(std::string name) { return map.find(name) != map.end(); }
  T get(std::string name) {
    auto v = map.find(name);
    if (v != map.end()) return v->second;
    return nullptr;
  }
  void clear() { this->map.clear(); }
  int size() { return this->map.size(); }

 protected:
  std::map<std::string, T> map;
};

template <class T_env>
class EnvironmentProxy {
 public:
  void update(std::string name, typename T_env::T_type t) {
    env->update(name, t);
  }
  bool contains(std::string name) { return env->contains(name); }
  typename T_env::T_type get(std::string name) { return env->get(name); }
  void clear() { env->clear(); }

 protected:
  T_env* env;
};

class VariableEnvironment_ : public Environment<ClassicType> {
 public:
  VariableEnvironment_() {}
};

class FunctionSignature_ {
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
};

class FunctionEnvironment_ : public Environment<FunctionSignature> {
 public:
  FunctionEnvironment_() {}
};

class VariableEnvironmentProxy : public EnvironmentProxy<VariableEnvironment_> {
 public:
  VariableEnvironmentProxy(VariableEnvironment e) { env = e; }
  VariableEnvironmentProxy() {}
};

class FunctionEnvironmentProxy : public EnvironmentProxy<FunctionEnvironment_> {
 public:
  FunctionEnvironmentProxy(FunctionEnvironment e) { env = e; }
  FunctionEnvironmentProxy() {}
};

class EnvironmentsProxy {
 public:
  EnvironmentsProxy(VariableEnvironmentProxy vars,
                    FunctionEnvironmentProxy funcs) {
    variables_proxy = vars;
    functions_proxy = funcs;
  }
  EnvironmentsProxy() {}

  void update_type(std::string name, void*) {}
  bool contains_type(std::string name) { return false; }
  void* get_type(std::string name) { return nullptr; }
  void clear_types() {}

  void update_function(std::string name, FunctionSignature fs) {
    functions_proxy.update(name, fs);
  }
  bool contains_function(std::string name) {
    return functions_proxy.contains(name);
  }
  FunctionSignature get_function(std::string name) {
    return functions_proxy.get(name);
  }
  void clear_functions() { functions_proxy.clear(); }

  void update_variable(std::string name, ClassicType t) {
    variables_proxy.update(name, t);
  }
  bool contains_variable(std::string name) {
    return variables_proxy.contains(name);
  }
  ClassicType get_variable(std::string name) {
    return variables_proxy.get(name);
  }
  void clear_variables() { variables_proxy.clear(); }

 private:
  VariableEnvironmentProxy variables_proxy;
  FunctionEnvironmentProxy functions_proxy;
};
