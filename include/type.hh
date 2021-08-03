#pragma once

#include <map>
#include <memory>
#include <string>

namespace plsm {

class value;
class execution_engine;
class constant;
class function;

#define _DEF_TYPE_OP_MAP(name)                                                 \
private:                                                                       \
  std::map<Type *, function *> name##_functions;                               \
                                                                               \
public:                                                                        \
  inline void register_##name(Type *type, function *function) {                \
    name##_functions[type] = function;                                         \
  }

#define _FULL_DEF_TYPE_OP(name)                                                \
  _DEF_TYPE_OP_MAP(name)                                                       \
public:                                                                        \
  void name(execution_engine *engine);

class Type {
private:
  std::string name;

public:
  Type(const std::string &name) : name(name) {}

  static inline std::shared_ptr<Type> get(const std::string &name) {
    return std::make_shared<Type>(name);
  }

  inline std::string &getName() { return name; }

public:
  void cast(execution_engine *engine, Type *type);
  _DEF_TYPE_OP_MAP(cast);

  _FULL_DEF_TYPE_OP(add);
  _FULL_DEF_TYPE_OP(sub);
  _FULL_DEF_TYPE_OP(mul);
  _FULL_DEF_TYPE_OP(div);
  _FULL_DEF_TYPE_OP(mod);

  _FULL_DEF_TYPE_OP(eq);
  _FULL_DEF_TYPE_OP(ne);
  _FULL_DEF_TYPE_OP(gt);
  _FULL_DEF_TYPE_OP(ge);
  _FULL_DEF_TYPE_OP(lt);
  _FULL_DEF_TYPE_OP(le);

public:
  inline bool hasRegisteredFunctions() { return false; }

  static inline Type *getUndefinedType() {
    static Type *undefinedType;
    return undefinedType ? undefinedType
                         : (undefinedType = new Type("Undefined"));
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

} // namespace plsm
