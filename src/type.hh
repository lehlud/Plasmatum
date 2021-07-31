#pragma once

#include <map>
#include <string>

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

public:
  Type(const std::string &name) : name(name) {}

  Type(const std::string &name,
       const std::map<Type *, FunctionValue *> &castFunctions,
       const std::map<Type *, FunctionValue *> &addFunctions,
       const std::map<Type *, FunctionValue *> &subFunctions,
       const std::map<Type *, FunctionValue *> &mulFunctions,
       const std::map<Type *, FunctionValue *> &divFunctions,
       const std::map<Type *, FunctionValue *> &modFunctions)
      : name(name), castFunctions(castFunctions), addFunctions(addFunctions),
        subFunctions(subFunctions), mulFunctions(mulFunctions),
        divFunctions(divFunctions), modFunctions(modFunctions) {}

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

  void cast(Engine *engine, Type *type);

  void add(Engine *engine);
  void sub(Engine *engine);
  void mul(Engine *engine);
  void div(Engine *engine);
  void mod(Engine *engine);

  static inline Type *getFloatType() {
    return new Type("Float");
  }

  static inline Type *getIntegerType() {
    return new Type("Int");
  }

  static inline Type *getBooleanType() {
    return new Type("Bool");
  }

  static std::map<std::string, Type *> getStandardTypes();

private:
  static void setupFloatType(Type *intType, Type *floatType, Type *boolType);
  static void setupIntegerType(Type *intType, Type *floatType, Type *boolType);
  static void setupBooleanType(Type *intType, Type *floatType, Type *boolType);
};
