#include "type.hh"

#include <cmath>
#include <functional>

#include "engine.hh"
#include "instruction.hh"
#include "value.hh"

namespace plsm {

void Type::cast(Engine *engine, Type *type) {
  FunctionValue *castFunction = castFunctions[type];
  if (castFunction) {
    castFunction->call(engine);
  } else {
    engine->stackPopVoid();
    engine->stackPush(new UndefinedValue());
  }
}

void Type::binexpr(Engine *engine,
                   std::map<Type *, FunctionValue *> &functions) {
  Type *type = engine->stackPeek()->type;

  FunctionValue *function = functions[type];
  if (function) {
    function->call(engine);
  } else {
    engine->stackPopVoid(2);
    engine->stackPush(new UndefinedValue());
  }
}

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

inline std::function<plsm_size_t(Engine *)>
getBinExprFunction(const std::function<Value *(Value *, Value *)> &function) {
  return [=](Engine *engine) {
    Value *v2 = engine->stackPop();
    Value *v1 = engine->stackPop();
    engine->stackPush(function(v1, v2));
    return 1;
  };
}

inline CustomInstruction *
getCustomBinExprInst(const std::function<Value *(Value *, Value *)> &function) {
  return new CustomInstruction(getBinExprFunction(function));
}

inline FunctionValue *getBinExprFunctionValue(Instruction *inst) {
  return new FunctionValue(2, {inst, new ReturnInstruction()});
}

void Type::setupFloatType(Type *intType, Type *floatType, Type *boolType) {
  if (floatType->hasRegisteredFunctions())
    return;

  Instruction *tmpInst = nullptr;

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(v1->asFloat() + v2->asFloat());
  });

  floatType->registerAdd(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(v1->asFloat() + v2->asInteger());
  });

  floatType->registerAdd(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(v1->asFloat() - v2->asFloat());
  });

  floatType->registerSub(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(v1->asFloat() - v2->asInteger());
  });

  floatType->registerSub(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(v1->asFloat() * v2->asFloat());
  });

  floatType->registerMul(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(v1->asFloat() * v2->asInteger());
  });

  floatType->registerMul(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(v2->asBoolean() ? v1->asFloat() : 0);
  });

  floatType->registerMul(boolType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(v1->asFloat() / v2->asFloat());
  });

  floatType->registerDiv(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(v1->asFloat() / v2->asInteger());
  });

  floatType->registerDiv(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(fmod(v1->asFloat(), v2->asFloat()));
  });

  floatType->registerMod(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(fmod(v1->asFloat(), v2->asInteger()));
  });

  floatType->registerMod(intType, getBinExprFunctionValue(tmpInst));
}

void Type::setupIntegerType(Type *intType, Type *floatType, Type *boolType) {
  if (intType->hasRegisteredFunctions())
    return;

  Instruction *tmpInst = nullptr;

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new IntegerValue(v1->asInteger() + v2->asInteger());
  });

  intType->registerAdd(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new IntegerValue(v1->asInteger() + v2->asFloat());
  });

  intType->registerAdd(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new IntegerValue(v1->asInteger() - v2->asInteger());
  });

  intType->registerSub(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(v1->asInteger() - v2->asFloat());
  });

  intType->registerSub(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new IntegerValue(v1->asInteger() * v2->asInteger());
  });

  intType->registerMul(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(v1->asInteger() * v2->asFloat());
  });

  intType->registerMul(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new IntegerValue(v2->asBoolean() ? v1->asInteger() : 0);
  });

  intType->registerMul(boolType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(v1->asInteger() / v2->asInteger());
  });

  intType->registerDiv(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(v1->asInteger() / v2->asFloat());
  });

  intType->registerDiv(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new IntegerValue(v1->asInteger() % v2->asInteger());
  });

  intType->registerMod(floatType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(fmod(v1->asInteger(), v2->asFloat()));
  });

  intType->registerMod(floatType, getBinExprFunctionValue(tmpInst));
}

void Type::setupBooleanType(Type *intType, Type *floatType, Type *boolType) {
  if (boolType->hasRegisteredFunctions())
    return;

  Instruction *tmpInst = nullptr;

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new IntegerValue(v1->asBoolean() ? v2->asInteger() : 0);
  });

  boolType->registerMul(intType, getBinExprFunctionValue(tmpInst));

  tmpInst = getCustomBinExprInst([](Value *v1, Value *v2) {
    return (Value *)new FloatValue(v1->asBoolean() ? v2->asFloat() : 0);
  });

  boolType->registerMul(floatType, getBinExprFunctionValue(tmpInst));
}

}
