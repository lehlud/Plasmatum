#include "type.hh"

#include <cmath>
#include <functional>
#include <iostream>

#include "engine.hh"
#include "instruction.hh"
#include "value.hh"

namespace plsm {

void Type::cast(Engine *engine, std::shared_ptr<Type> type) {
  std::shared_ptr<FunctionValue> castFunction = castFunctions[type];
  if (castFunction) {
    castFunction->call(engine);
  } else {
    engine->stackPop();
    engine->stackPush(UndefinedValue::get());
  }
}

void Type::binexpr(Engine *engine,
                   std::map<std::shared_ptr<Type>,
                            std::shared_ptr<FunctionValue>> &functions) {
  std::shared_ptr<Type> type = engine->stackPeek()->type;

  std::shared_ptr<FunctionValue> function = functions[type];
  if (function) {
    function->call(engine);
  } else {
    engine->stackPop();
    engine->stackPop();
    engine->stackPush(UndefinedValue::get());
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

inline std::function<plsm_size_t(Engine *)> getBinExprFunction(
    const std::function<std::shared_ptr<Value>(
        std::shared_ptr<Value>, std::shared_ptr<Value>)> &function) {
  return [=](Engine *engine) {
    std::shared_ptr<Value> v2 = engine->stackPeek();
    engine->stackPop();

    std::shared_ptr<Value> v1 = engine->stackPeek();
    engine->stackPop();

    engine->stackPush(function(v1, v2));
    return 1;
  };
}

inline std::shared_ptr<CustomInstruction> getCustomBinExprInst(
    const std::function<std::shared_ptr<Value>(
        std::shared_ptr<Value>, std::shared_ptr<Value>)> &function) {
  return CustomInstruction::get(getBinExprFunction(function));
}

inline std::shared_ptr<FunctionValue>
getBinExprFunctionValue(std::shared_ptr<Instruction> inst) {
  return FunctionValue::get(2, {inst, ReturnInstruction::get()});
}

void Type::setupFloatType(std::shared_ptr<Type> intType,
                          std::shared_ptr<Type> floatType,
                          std::shared_ptr<Type> boolType) {
  if (floatType->hasRegisteredFunctions())
    return;

  std::shared_ptr<Instruction> tmpInst = nullptr;

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(v1->asFloat() + v2->asFloat());
      });

  floatType->registerAdd(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(v1->asFloat() + v2->asInteger());
      });

  floatType->registerAdd(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(v1->asFloat() - v2->asFloat());
      });

  floatType->registerSub(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(v1->asFloat() - v2->asInteger());
      });

  floatType->registerSub(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(v1->asFloat() * v2->asFloat());
      });

  floatType->registerMul(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(v1->asFloat() * v2->asInteger());
      });

  floatType->registerMul(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(v2->asBoolean() ? v1->asFloat() : 0);
      });

  floatType->registerMul(boolType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(v1->asFloat() / v2->asFloat());
      });

  floatType->registerDiv(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(v1->asFloat() / v2->asInteger());
      });

  floatType->registerDiv(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(fmod(v1->asFloat(), v2->asFloat()));
      });

  floatType->registerMod(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(fmod(v1->asFloat(), v2->asInteger()));
      });

  floatType->registerMod(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asFloat() == v2->asFloat());
      });

  floatType->registerEQ(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asFloat() == v2->asInteger());
      });

  floatType->registerEQ(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asFloat() != v2->asFloat());
      });

  floatType->registerNE(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asFloat() != v2->asInteger());
      });

  floatType->registerNE(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asFloat() > v2->asFloat());
      });

  floatType->registerGT(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asFloat() > v2->asInteger());
      });

  floatType->registerGT(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asFloat() >= v2->asFloat());
      });

  floatType->registerGE(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asFloat() >= v2->asInteger());
      });

  floatType->registerGE(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asFloat() < v2->asFloat());
      });

  floatType->registerLT(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asFloat() < v2->asInteger());
      });

  floatType->registerLT(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asFloat() <= v2->asFloat());
      });

  floatType->registerLE(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asFloat() <= v2->asInteger());
      });

  floatType->registerLE(intType, getBinExprFunctionValue(tmpInst));
}

void Type::setupIntegerType(std::shared_ptr<Type> intType,
                            std::shared_ptr<Type> floatType,
                            std::shared_ptr<Type> boolType) {
  if (intType->hasRegisteredFunctions())
    return;

  std::shared_ptr<Instruction> tmpInst = nullptr;

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return IntegerValue::get(v1->asInteger() + v2->asInteger());
      });

  intType->registerAdd(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return IntegerValue::get(v1->asInteger() + v2->asFloat());
      });

  intType->registerAdd(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return IntegerValue::get(v1->asInteger() - v2->asInteger());
      });

  intType->registerSub(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(v1->asInteger() - v2->asFloat());
      });

  intType->registerSub(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return IntegerValue::get(v1->asInteger() * v2->asInteger());
      });

  intType->registerMul(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(v1->asInteger() * v2->asFloat());
      });

  intType->registerMul(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return IntegerValue::get(v2->asBoolean() ? v1->asInteger() : 0);
      });

  intType->registerMul(boolType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(v1->asInteger() / v2->asInteger());
      });

  intType->registerDiv(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(v1->asInteger() / v2->asFloat());
      });

  intType->registerDiv(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return IntegerValue::get(v1->asInteger() % v2->asInteger());
      });

  intType->registerMod(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(fmod(v1->asInteger(), v2->asFloat()));
      });

  intType->registerMod(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asInteger() == v2->asInteger());
      });

  intType->registerEQ(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asInteger() == v2->asFloat());
      });

  intType->registerEQ(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asInteger() != v2->asInteger());
      });

  intType->registerNE(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asInteger() != v2->asFloat());
      });

  intType->registerNE(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asInteger() > v2->asInteger());
      });

  intType->registerGT(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return BooleanValue::get(v1->asInteger() > v2->asFloat());
      });

  intType->registerGT(floatType, getBinExprFunctionValue(tmpInst));
}

void Type::setupBooleanType(std::shared_ptr<Type> intType,
                            std::shared_ptr<Type> floatType,
                            std::shared_ptr<Type> boolType) {
  if (boolType->hasRegisteredFunctions())
    return;

  std::shared_ptr<Instruction> tmpInst = nullptr;

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return IntegerValue::get(v1->asBoolean() ? v2->asInteger() : 0);
      });

  boolType->registerMul(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst(
      [](std::shared_ptr<Value> v1, std::shared_ptr<Value> v2) {
        return FloatValue::get(v1->asBoolean() ? v2->asFloat() : 0);
      });

  boolType->registerMul(floatType, getBinExprFunctionValue(tmpInst));
}

} // namespace plsm
