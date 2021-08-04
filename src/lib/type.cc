#include "type.hh"

#include <cmath>
#include <functional>
#include <iostream>

#include "engine.hh"
#include "instruction.hh"
#include "value.hh"

namespace plsm {

void Type::cast(execution_engine *engine, Type *type) {
  function *cast_function = cast_functions[type];
  if (cast_function) {
    cast_function->call(engine);
  } else {
    engine->stack_pop();
    engine->stack_push(new undefined());
  }
}

#define _DEF_TYPE_BINEXPR(name, default)                                       \
  void Type::name(execution_engine *engine) {                                  \
    Type *type = engine->stack_peek()->type;                                   \
                                                                               \
    if (name##_functions.count(type)) {                                        \
      name##_functions[type]->call(engine);                                    \
    } else {                                                                   \
      engine->stack_pop();                                                     \
      engine->stack_pop();                                                     \
      engine->stack_push(default);                                             \
    }                                                                          \
  }

_DEF_TYPE_BINEXPR(add, new undefined())
_DEF_TYPE_BINEXPR(sub, new undefined())
_DEF_TYPE_BINEXPR(mul, new undefined())
_DEF_TYPE_BINEXPR(div, new undefined())
_DEF_TYPE_BINEXPR(mod, new undefined())

_DEF_TYPE_BINEXPR(eq, new boolean(false))
_DEF_TYPE_BINEXPR(ne, new boolean(false))
_DEF_TYPE_BINEXPR(gt, new boolean(false))
_DEF_TYPE_BINEXPR(ge, new boolean(false))
_DEF_TYPE_BINEXPR(lt, new boolean(false))
_DEF_TYPE_BINEXPR(le, new boolean(false))

std::map<std::string, Type *> Type::getStandardTypes() {
  std::map<std::string, Type *> result;

  Type *intT = getIntegerType();
  Type *floatT = getFloatType();
  Type *boolT = getBooleanType();

  setupFloatType(intT, floatT, boolT);
  setupIntegerType(intT, floatT, boolT);
  setupBooleanType(intT, floatT, boolT);

  result[intT->getName()] = intT;
  result[floatT->getName()] = floatT;
  result[boolT->getName()] = boolT;

  return result;
}

#define _REGISTER_BINEXPR_FUNCTION_VALUE(name, type1, type2, result)           \
  type1->register_##name(                                                      \
      type2,                                                                   \
      new function(                                                            \
          2, {new value_inst<custom_inst_function>(                            \
                  instruction::code_custom,                                    \
                  new custom_inst_function([](execution_engine *engine) {      \
                    value *v2 = engine->stack_peek();                          \
                    engine->stack_pop_no_delete();                             \
                                                                               \
                    value *v1 = engine->stack_peek();                          \
                    engine->stack_pop_no_delete();                             \
                                                                               \
                    engine->stack_push(result);                                \
                                                                               \
                    delete v1;                                                 \
                    delete v2;                                                 \
                                                                               \
                    return 1;                                                  \
                  })),                                                         \
              new instruction(instruction::code_return)}));

void Type::setupFloatType(Type *intType, Type *floatType, Type *boolType) {
  if (floatType->hasRegisteredFunctions())
    return;

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      add, floatType, intType,
      new floating_point(v1->as_floating_point() + v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      add, floatType, floatType,
      new floating_point(v1->as_floating_point() + v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      sub, floatType, intType,
      new floating_point(v1->as_floating_point() - v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      sub, floatType, floatType,
      new floating_point(v1->as_floating_point() - v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      mul, floatType, intType,
      new floating_point(v1->as_floating_point() * v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      mul, floatType, floatType,
      new floating_point(v1->as_floating_point() * v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      mul, floatType, boolType,
      new floating_point(v2->as_boolean() ? v1->as_floating_point() : 0))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      div, floatType, intType,
      new floating_point(v1->as_floating_point() / v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      div, floatType, floatType,
      new floating_point(v1->as_floating_point() / v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      mod, floatType, intType,
      new floating_point(fmod(v1->as_floating_point(), v2->as_integer())))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      mod, floatType, floatType,
      new floating_point(
          fmod(v1->as_floating_point(), v2->as_floating_point())))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      eq, floatType, intType,
      new floating_point(v1->as_floating_point() == v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      eq, floatType, floatType,
      new floating_point(v1->as_floating_point() == v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      ne, floatType, intType,
      new floating_point(v1->as_floating_point() != v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      ne, floatType, floatType,
      new floating_point(v1->as_floating_point() != v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      gt, floatType, intType,
      new floating_point(v1->as_floating_point() > v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      gt, floatType, floatType,
      new floating_point(v1->as_floating_point() > v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      ge, floatType, intType,
      new floating_point(v1->as_floating_point() >= v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      ge, floatType, floatType,
      new floating_point(v1->as_floating_point() >= v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      lt, floatType, intType,
      new floating_point(v1->as_floating_point() < v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      lt, floatType, floatType,
      new floating_point(v1->as_floating_point() < v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      le, floatType, intType,
      new floating_point(v1->as_floating_point() <= v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      le, floatType, floatType,
      new floating_point(v1->as_floating_point() <= v2->as_floating_point()))
}

void Type::setupIntegerType(Type *intType, Type *floatType, Type *boolType) {
  if (intType->hasRegisteredFunctions())
    return;

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      add, intType, intType, new integer(v1->as_integer() + v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      add, intType, floatType,
      new floating_point(v1->as_floating_point() + v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      sub, intType, intType, new integer(v1->as_integer() - v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      sub, intType, floatType,
      new floating_point(v1->as_integer() - v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      mul, intType, intType, new integer(v1->as_integer() * v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      mul, intType, floatType,
      new floating_point(v1->as_integer() * v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      mul, intType, boolType,
      new integer(v2->as_boolean() ? v1->as_integer() : 0))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      div, intType, intType,
      new floating_point(v1->as_integer() / v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      div, intType, floatType,
      new floating_point(v1->as_integer() / v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      mod, intType, intType, new integer(v1->as_integer() % v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      mod, intType, floatType,
      new floating_point(fmod(v1->as_integer(), v2->as_floating_point())))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      eq, intType, intType,
      new floating_point(v1->as_integer() == v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      eq, intType, floatType,
      new floating_point(v1->as_integer() == v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      ne, intType, intType,
      new floating_point(v1->as_integer() != v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      ne, intType, floatType,
      new floating_point(v1->as_integer() != v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      gt, intType, intType,
      new floating_point(v1->as_integer() > v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      gt, intType, floatType,
      new floating_point(v1->as_integer() > v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      ge, intType, intType,
      new floating_point(v1->as_integer() >= v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      ge, intType, floatType,
      new floating_point(v1->as_integer() >= v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      lt, intType, intType,
      new floating_point(v1->as_integer() < v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      lt, intType, floatType,
      new floating_point(v1->as_integer() < v2->as_floating_point()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      le, intType, intType,
      new floating_point(v1->as_integer() <= v2->as_integer()))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      le, intType, floatType,
      new floating_point(v1->as_integer() <= v2->as_floating_point()))
}

void Type::setupBooleanType(Type *intType, Type *floatType, Type *boolType) {
  if (boolType->hasRegisteredFunctions())
    return;

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      mul, boolType, intType,
      new integer(v1->as_boolean() ? v2->as_integer() : 0))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      mul, boolType, floatType,
      new floating_point(v1->as_boolean() ? v2->as_floating_point() : 0))

  _REGISTER_BINEXPR_FUNCTION_VALUE(
      mul, boolType, boolType, new boolean(v1->as_boolean() * v2->as_boolean()))
}

} // namespace plsm
