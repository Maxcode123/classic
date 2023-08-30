#pragma once

#include <llvm/IR/Value.h>

#include <map>

typedef class SymbolTable_ {
 public:
  SymbolTable_() {}

  bool look_up(std::string n) { return (map.find(n) != map.end()); }
  llvm::Value* get(std::string n) {
    auto v = map.find(n);
    if (v != map.end()) return v->second;
    return nullptr;
  }
  void update(std::string n, llvm::Value* v) {
    auto itr = map.find(n);
    if (itr == map.end()) {
      map.insert(std::make_pair(n, v));
    } else {
      itr->second = v;
    }
  }
  void clear() { map.clear(); }

 private:
  std::map<std::string, llvm::Value*> map;
}* SymbolTable;

class SymbolTableProxy {
 public:
  SymbolTableProxy(SymbolTable st) { symbol_table = st; }
  SymbolTableProxy() {}

  bool look_up(std::string n) { return symbol_table->look_up(n); }
  llvm::Value* get(std::string n) { return symbol_table->get(n); }
  void update(std::string n, llvm::Value* v) { symbol_table->update(n, v); }
  void clear() { symbol_table->clear(); }

 private:
  SymbolTable symbol_table;
};
