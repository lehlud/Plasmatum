#pragma once

#include <map>
#include <memory>
#include <string>

namespace plsm {

class value;
class execution_engine;
class constant;
class function;

class Type {
private:
  std::string name;

  std::map<std::shared_ptr<Type>, function *> castFunctions;

  std::map<std::shared_ptr<Type>, function *> addFunctions;
  std::map<std::shared_ptr<Type>, function *> subFunctions;
  std::map<std::shared_ptr<Type>, function *> mulFunctions;
  std::map<std::shared_ptr<Type>, function *> divFunctions;
  std::map<std::shared_ptr<Type>, function *> modFunctions;

  std::map<std::shared_ptr<Type>, function *> eqFunctions;
  std::map<std::shared_ptr<Type>, function *> neFunctions;
  std::map<std::shared_ptr<Type>, function *> gtFunctions;
  std::map<std::shared_ptr<Type>, function *> geFunctions;
  std::map<std::shared_ptr<Type>, function *> ltFunctions;
  std::map<std::shared_ptr<Type>, function *> leFunctions;

public:
  Type(const std::string &name) : name(name) {}

  static inline std::shared_ptr<Type> get(const std::string &name) {
    return std::make_shared<Type>(name);
  }

  inline std::string &getName() { return name; }

  inline void registerCast(std::shared_ptr<Type> type, function *function) {
    castFunctions[type] = function;
  }

  inline void registerAdd(std::shared_ptr<Type> type, function *function) {
    addFunctions[type] = function;
  }

  inline void registerSub(std::shared_ptr<Type> type, function *function) {
    subFunctions[type] = function;
  }

  inline void registerMul(std::shared_ptr<Type> type, function *function) {
    mulFunctions[type] = function;
  }

  inline void registerDiv(std::shared_ptr<Type> type, function *function) {
    divFunctions[type] = function;
  }

  inline void registerMod(std::shared_ptr<Type> type, function *function) {
    modFunctions[type] = function;
  }

  inline void registerEQ(std::shared_ptr<Type> type, function *function) {
    eqFunctions[type] = function;
  }

  inline void registerNE(std::shared_ptr<Type> type, function *function) {
    neFunctions[type] = function;
  }

  inline void registerGT(std::shared_ptr<Type> type, function *function) {
    gtFunctions[type] = function;
  }

  inline void registerGE(std::shared_ptr<Type> type, function *function) {
    geFunctions[type] = function;
  }

  inline void registerLT(std::shared_ptr<Type> type, function *function) {
    ltFunctions[type] = function;
  }

  inline void registerLE(std::shared_ptr<Type> type, function *function) {
    leFunctions[type] = function;
  }

  inline bool hasRegisteredFunctions() {
    return addFunctions.size() || subFunctions.size() || mulFunctions.size() ||
           divFunctions.size() || modFunctions.size();
  }

  void cast(execution_engine *engine, std::shared_ptr<Type> type);

  void binexpr(
      execution_engine *engine,
      std::map<std::shared_ptr<Type>, function *> &functions);

  inline void add(execution_engine *engine) { binexpr(engine, addFunctions); }

  inline void sub(execution_engine *engine) { binexpr(engine, subFunctions); }

  inline void mul(execution_engine *engine) { binexpr(engine, mulFunctions); }

  inline void div(execution_engine *engine) { binexpr(engine, divFunctions); }

  inline void mod(execution_engine *engine) { binexpr(engine, modFunctions); }

  inline void eq(execution_engine *engine) { binexpr(engine, eqFunctions); }

  inline void ne(execution_engine *engine) { binexpr(engine, neFunctions); }

  inline void gt(execution_engine *engine) { binexpr(engine, gtFunctions); }

  inline void ge(execution_engine *engine) { binexpr(engine, geFunctions); }

  inline void lt(execution_engine *engine) { binexpr(engine, ltFunctions); }

  inline void le(execution_engine *engine) { binexpr(engine, leFunctions); }

  static inline std::shared_ptr<Type> getUndefinedType() {
    static std::shared_ptr<Type> undefinedType;
    return undefinedType ? undefinedType
                         : (undefinedType = Type::get("Undefined"));
  }

  static inline std::shared_ptr<Type> getFloatType() {
    static std::shared_ptr<Type> floatType;
    return floatType ? floatType : (floatType = Type::get("Float"));
  }

  static inline std::shared_ptr<Type> getIntegerType() {
    static std::shared_ptr<Type> integerType;
    return integerType ? integerType : (integerType = Type::get("Int"));
  }

  static inline std::shared_ptr<Type> getBooleanType() {
    static std::shared_ptr<Type> boolType;
    return boolType ? boolType : (boolType = Type::get("Bool"));
  }

  static inline std::shared_ptr<Type> getFunctionType() {
    static std::shared_ptr<Type> functionType;
    return functionType ? functionType : (functionType = Type::get("Function"));
  }

  static std::map<std::string, std::shared_ptr<Type>> getStandardTypes();

private:
  static void setupFloatType(std::shared_ptr<Type> intType,
                             std::shared_ptr<Type> floatType,
                             std::shared_ptr<Type> boolType);
  static void setupIntegerType(std::shared_ptr<Type> intType,
                               std::shared_ptr<Type> floatType,
                               std::shared_ptr<Type> boolType);
  static void setupBooleanType(std::shared_ptr<Type> intType,
                               std::shared_ptr<Type> floatType,
                               std::shared_ptr<Type> boolType);
};

} // namespace plsm
