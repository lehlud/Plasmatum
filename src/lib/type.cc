#include "type.hh"

#include <cmath>
#include <functional>
#include <iostream>

#include "engine.hh"
#include "instruction.hh"
#include "value.hh"

namespace plsm {

void Type::cast(execution_engine *engine, std::shared_ptr<Type> type) {
  function *castFunction = castFunctions[type];
  if (castFunction) {
    castFunction->call(engine);
  } else {
    engine->stackPop();
    engine->stackPush(new undefined());
  }
}

void Type::binexpr(execution_engine *engine,
                   std::map<std::shared_ptr<Type>, function *> &functions) {
  std::shared_ptr<Type> type = engine->stackPeek()->type;

  if (functions.count(type)) {
    functions[type]->call(engine);
  } else {
    engine->stackPop();
    engine->stackPop();
    engine->stackPush(new undefined());
  }
}

std::map<std::string, std::shared_ptr<Type>> Type::getStandardTypes() {
  std::map<std::string, std::shared_ptr<Type>> result;

  std::shared_ptr<Type> intT = getIntegerType();
  std::shared_ptr<Type> floatT = getFloatType();
  std::shared_ptr<Type> boolT = getBooleanType();

  setupFloatType(intT, floatT, boolT);
  setupIntegerType(intT, floatT, boolT);
  setupBooleanType(intT, floatT, boolT);

  result[intT->getName()] = intT;
  result[floatT->getName()] = floatT;
  result[boolT->getName()] = boolT;

  return result;
}

inline std::function<plsm_size_t(execution_engine *)>
getBinExprFunction(const std::function<value *(value *, value *)> &function) {
  return [=](execution_engine *engine) {
    value *v2 = engine->stackPeek();
    engine->stackPop(false);

    value *v1 = engine->stackPeek();
    engine->stackPop(false);

    engine->stackPush(function(v1, v2));

    // std::cout << "deleting v1 and v2" << std::endl;

    // delete v1;
    // delete v2;

    // std::cout << "deleted v1 and v2" << std::endl;

    return 1;
  };
}

#define _REGISTER_BINEXPR_FUNCTION_VALUE(name, type1, type2, result)           \
  type1->register##name(                                                       \
      type2,                                                                   \
      new function(                                                            \
          2, {new value_inst<custom_inst_function>(                            \
                  instruction::code_custom,                                    \
                  new custom_inst_function([](execution_engine *engine) {      \
                    value *v2 = engine->stackPeek();                           \
                    engine->stackPop(false);                                   \
                                                                               \
                    value *v1 = engine->stackPeek();                           \
                    engine->stackPop(false);                                   \
                                                                               \
                    engine->stackPush(result);                                 \
                                                                               \
                    /* delete v1; */                                           \
                    /* delete v2; */                                           \
                                                                               \
                    return 1;                                                  \
                  })),                                                         \
              new instruction(instruction::code_return)}));

void Type::setupFloatType(std::shared_ptr<Type> intType,
                          std::shared_ptr<Type> floatType,
                          std::shared_ptr<Type> boolType) {
  if (floatType->hasRegisteredFunctions())
    return;

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Add, floatType, intType,
      new floating_point(v1->as_floating_point() + v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Add, floatType, floatType,
      new floating_point(v1->as_floating_point() + v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Sub, floatType, intType,
      new floating_point(v1->as_floating_point() - v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Sub, floatType, floatType,
      new floating_point(v1->as_floating_point() - v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Mul, floatType, intType,
      new floating_point(v1->as_floating_point() * v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Mul, floatType, floatType,
      new floating_point(v1->as_floating_point() * v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Mul, floatType, boolType,
      new floating_point(v2->as_boolean() ? v1->as_floating_point() : 0))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Div, floatType, intType,
      new floating_point(v1->as_floating_point() / v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Div, floatType, floatType,
      new floating_point(v1->as_floating_point() / v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Mod, floatType, intType,
      new floating_point(fmod(v1->as_floating_point(), v2->as_integer())))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Mod, floatType, floatType,
      new floating_point(fmod(v1->as_floating_point(), v2->as_floating_point())))
  

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      EQ, floatType, intType,
      new floating_point(v1->as_floating_point() == v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      EQ, floatType, floatType,
      new floating_point(v1->as_floating_point() == v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      NE, floatType, intType,
      new floating_point(v1->as_floating_point() != v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      NE, floatType, floatType,
      new floating_point(v1->as_floating_point() != v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      GT, floatType, intType,
      new floating_point(v1->as_floating_point() > v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      GT, floatType, floatType,
      new floating_point(v1->as_floating_point() > v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      GE, floatType, intType,
      new floating_point(v1->as_floating_point() >= v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      GE, floatType, floatType,
      new floating_point(v1->as_floating_point() >= v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      LT, floatType, intType,
      new floating_point(v1->as_floating_point() < v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      LT, floatType, floatType,
      new floating_point(v1->as_floating_point() < v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      LE, floatType, intType,
      new floating_point(v1->as_floating_point() <= v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      LE, floatType, floatType,
      new floating_point(v1->as_floating_point() <= v2->as_floating_point()))
}

void Type::setupIntegerType(std::shared_ptr<Type> intType,
                            std::shared_ptr<Type> floatType,
                            std::shared_ptr<Type> boolType) {
  if (intType->hasRegisteredFunctions())
    return;


  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Add, intType, intType,
      new integer(v1->as_integer() + v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Add, intType, floatType,
      new floating_point(v1->as_floating_point() + v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Sub, intType, intType,
      new integer(v1->as_integer() - v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Sub, intType, floatType,
      new floating_point(v1->as_integer() - v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Mul, intType, intType,
      new integer(v1->as_integer() * v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Mul, intType, floatType,
      new floating_point(v1->as_integer() * v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Mul, intType, boolType,
      new integer(v2->as_boolean() ? v1->as_integer() : 0))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Div, intType, intType,
      new floating_point(v1->as_integer() / v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Div, intType, floatType,
      new floating_point(v1->as_integer() / v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Mod, intType, intType,
      new integer(v1->as_integer() % v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Mod, intType, floatType,
      new floating_point(fmod(v1->as_integer(), v2->as_floating_point())))
  

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      EQ, intType, intType,
      new floating_point(v1->as_integer() == v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      EQ, intType, floatType,
      new floating_point(v1->as_integer() == v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      NE, intType, intType,
      new floating_point(v1->as_integer() != v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      NE, intType, floatType,
      new floating_point(v1->as_integer() != v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      GT, intType, intType,
      new floating_point(v1->as_integer() > v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      GT, intType, floatType,
      new floating_point(v1->as_integer() > v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      GE, intType, intType,
      new floating_point(v1->as_integer() >= v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      GE, intType, floatType,
      new floating_point(v1->as_integer() >= v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      LT, intType, intType,
      new floating_point(v1->as_integer() < v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      LT, intType, floatType,
      new floating_point(v1->as_integer() < v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      LE, intType, intType,
      new floating_point(v1->as_integer() <= v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      LE, intType, floatType,
      new floating_point(v1->as_integer() <= v2->as_floating_point()))
}

void Type::setupBooleanType(std::shared_ptr<Type> intType,
                            std::shared_ptr<Type> floatType,
                            std::shared_ptr<Type> boolType) {
  if (boolType->hasRegisteredFunctions())
    return;

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Mul, boolType, intType,
      new integer(v1->as_boolean() ? v2->as_integer() : 0))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Mul, boolType, floatType,
      new floating_point(v1->as_boolean() ? v2->as_floating_point() : 0))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      Mul, boolType, boolType,
      new boolean(v1->as_boolean() * v2->as_boolean()))
}

} // namespace plsm
