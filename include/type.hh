#pragma once

#include <map>
#include <string>

namespace plsm {

class Value;
class Engine;
class Constant;
class FunctionValue;

class Type {
private:
  std::string name;

  std::map<Type *, FunctionValue *> castFunctions;

  std::map<Type *, FunctionValue *> addFunctions;
  std::map<Type *, FunctionValue *> subFunctions;
  std::map<Type *, FunctionValue *> mulFunctions;
  std::map<Type *, FunctionValue *> divFunctions;
  std::map<Type *, FunctionValue *> modFunctions;

  std::map<Type *, FunctionValue *> eqFunctions;
  std::map<Type *, FunctionValue *> neFunctions;
  std::map<Type *, FunctionValue *> gtFunctions;
  std::map<Type *, FunctionValue *> geFunctions;
  std::map<Type *, FunctionValue *> ltFunctions;
  std::map<Type *, FunctionValue *> leFunctions;

public:
  Type(const std::string &name) : name(name) {}

  inline std::string &getName() { return name; }

  inline void registerCast(Type *type, FunctionValue *function) {
    castFunctions[type] = function;
  }

  inline void registerAdd(Type *type, FunctionValue *function) {
    addFunctions[type] = function;
  }

  inline void registerSub(Type *type, FunctionValue *function) {
    subFunctions[type] = function;
  }

  inline void registerMul(Type *type, FunctionValue *function) {
    mulFunctions[type] = function;
  }

  inline void registerDiv(Type *type, FunctionValue *function) {
    divFunctions[type] = function;
  }

  inline void registerMod(Type *type, FunctionValue *function) {
    modFunctions[type] = function;
  }

  inline bool hasRegisteredFunctions() {
    return addFunctions.size() || subFunctions.size() || mulFunctions.size() ||
           divFunctions.size() || modFunctions.size();
  }

  void cast(Engine *engine, Type *type);

  void binexpr(Engine *engine, std::map<Type *, FunctionValue *> &functions);
 
  inline void add(Engine *engine) {
    binexpr(engine, addFunctions);
  }

  inline void sub(Engine *engine) {
    binexpr(engine, subFunctions);
  }

  inline void mul(Engine *engine) {
    binexpr(engine, mulFunctions);
  }

  inline void div(Engine *engine) {
    binexpr(engine, divFunctions);
  }

  inline void mod(Engine *engine) {
    binexpr(engine, modFunctions);
  }

  static inline Type *getUndefinedType() {
    static Type *undefinedType;
    return undefinedType ? undefinedType : (undefinedType = new Type("Undefined"));
  }

  static inline Type *getFloatType() {
    static Type *floatType;
    return floatType ? floatType : (floatType = new Type("Float"));
  }

  static inline Type *getIntegerType() {
    static Type *integerType;
    return integerType ? integerType : (integerType = new Type("Int"));
  }

  static inline Type *getBooleanType() {
    static Type *boolType;
    return boolType ? boolType : (boolType = new Type("Bool"));
  }

  static inline Type *getFunctionType() {
    static Type *functionType;
    return functionType ? functionType : (functionType = new Type("Function"));
  }

  static std::map<std::string, Type *> getStandardTypes();

private:
  static void setupFloatType(Type *intType, Type *floatType, Type *boolType);
  static void setupIntegerType(Type *intType, Type *floatType, Type *boolType);
  static void setupBooleanType(Type *intType, Type *floatType, Type *boolType);
};

}
