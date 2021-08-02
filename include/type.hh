#pragma once

#include <map>
#include <memory>
#include <string>

namespace plsm {

class Value;
class Engine;
class Constant;
class FunctionValue;

class Type {
private:
  std::string name;

  std::map<std::shared_ptr<Type>, std::shared_ptr<FunctionValue>> castFunctions;

  std::map<std::shared_ptr<Type>, std::shared_ptr<FunctionValue>> addFunctions;
  std::map<std::shared_ptr<Type>, std::shared_ptr<FunctionValue>> subFunctions;
  std::map<std::shared_ptr<Type>, std::shared_ptr<FunctionValue>> mulFunctions;
  std::map<std::shared_ptr<Type>, std::shared_ptr<FunctionValue>> divFunctions;
  std::map<std::shared_ptr<Type>, std::shared_ptr<FunctionValue>> modFunctions;

  std::map<std::shared_ptr<Type>, std::shared_ptr<FunctionValue>> eqFunctions;
  std::map<std::shared_ptr<Type>, std::shared_ptr<FunctionValue>> neFunctions;
  std::map<std::shared_ptr<Type>, std::shared_ptr<FunctionValue>> gtFunctions;
  std::map<std::shared_ptr<Type>, std::shared_ptr<FunctionValue>> geFunctions;
  std::map<std::shared_ptr<Type>, std::shared_ptr<FunctionValue>> ltFunctions;
  std::map<std::shared_ptr<Type>, std::shared_ptr<FunctionValue>> leFunctions;

public:
  Type(const std::string &name) : name(name) {}

  static inline std::shared_ptr<Type> get(const std::string &name) {
    return std::make_shared<Type>(name);
  }

  inline std::string &getName() { return name; }

  inline void registerCast(std::shared_ptr<Type> type,
                           std::shared_ptr<FunctionValue> function) {
    castFunctions[type] = function;
  }

  inline void registerAdd(std::shared_ptr<Type> type,
                          std::shared_ptr<FunctionValue> function) {
    addFunctions[type] = function;
  }

  inline void registerSub(std::shared_ptr<Type> type,
                          std::shared_ptr<FunctionValue> function) {
    subFunctions[type] = function;
  }

  inline void registerMul(std::shared_ptr<Type> type,
                          std::shared_ptr<FunctionValue> function) {
    mulFunctions[type] = function;
  }

  inline void registerDiv(std::shared_ptr<Type> type,
                          std::shared_ptr<FunctionValue> function) {
    divFunctions[type] = function;
  }

  inline void registerMod(std::shared_ptr<Type> type,
                          std::shared_ptr<FunctionValue> function) {
    modFunctions[type] = function;
  }

  inline bool hasRegisteredFunctions() {
    return addFunctions.size() || subFunctions.size() || mulFunctions.size() ||
           divFunctions.size() || modFunctions.size();
  }

  void cast(Engine *engine, std::shared_ptr<Type> type);

  void binexpr(Engine *engine,
               std::map<std::shared_ptr<Type>, std::shared_ptr<FunctionValue>>
                   &functions);

  inline void add(Engine *engine) { binexpr(engine, addFunctions); }

  inline void sub(Engine *engine) { binexpr(engine, subFunctions); }

  inline void mul(Engine *engine) { binexpr(engine, mulFunctions); }

  inline void div(Engine *engine) { binexpr(engine, divFunctions); }

  inline void mod(Engine *engine) { binexpr(engine, modFunctions); }

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
