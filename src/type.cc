#include "type.hh"

#include <cmath>

#include "engine.hh"
#include "instruction.hh"
#include "value.hh"

void Type::cast(Engine *engine, Type *type) {
  FunctionValue *castFunction = castFunctions[type];
  if (castFunction) {
    castFunction->call(engine);
  } else {
    engine->stackPopVoid();
    engine->stackPush(new UndefinedValue());
  }
}

void Type::binexpr(Engine *engine, std::map<Type *, FunctionValue *> &functions) {
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

void Type::setupFloatType(Type *intType, Type *floatType, Type *boolType) {
  if (floatType->hasRegisteredFunctions())
    return;
}

void Type::setupIntegerType(Type *intType, Type *floatType, Type *boolType) {
  if (intType->hasRegisteredFunctions())
    return;

  Instruction *tmpInst = nullptr;

  // * Integer Add Instructions

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stackPop();
    Value *left = engine->stackPop();

    Value *result = new IntegerValue(((IntegerValue *)left)->getValue() +
                                     ((IntegerValue *)right)->getValue());
    engine->stackPush(result);
    return 1;
  });

  intType->registerAdd(
      intType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stackPop();
    Value *left = engine->stackPop();

    Value *result = new FloatValue(((IntegerValue *)left)->getValue() +
                                   ((FloatValue *)right)->getValue());
    engine->stackPush(result);
    return 1;
  });

  intType->registerAdd(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stackPop();
    Value *left = engine->stackPop();

    Value *result = new IntegerValue(((IntegerValue *)left)->getValue() -
                                     ((IntegerValue *)right)->getValue());
    engine->stackPush(result);
    return 1;
  });

  intType->registerSub(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stackPop();
    Value *left = engine->stackPop();

    Value *result = new FloatValue(((IntegerValue *)left)->getValue() -
                                   ((FloatValue *)right)->getValue());
    engine->stackPush(result);
    return 1;
  });

  intType->registerSub(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stackPop();
    Value *left = engine->stackPop();

    Value *result = new IntegerValue(((IntegerValue *)left)->getValue() *
                                     ((IntegerValue *)right)->getValue());
    engine->stackPush(result);
    return 1;
  });

  intType->registerMul(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stackPop();
    Value *left = engine->stackPop();

    Value *result = new FloatValue(((IntegerValue *)left)->getValue() *
                                   ((FloatValue *)right)->getValue());
    engine->stackPush(result);
    return 1;
  });

  intType->registerMul(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stackPop();
    Value *left = engine->stackPop();

    Value *result = new FloatValue(((IntegerValue *)left)->getValue() /
                                   ((IntegerValue *)right)->getValue());
    engine->stackPush(result);
    return 1;
  });

  intType->registerDiv(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stackPop();
    Value *left = engine->stackPop();

    Value *result = new FloatValue(((IntegerValue *)left)->getValue() /
                                   ((FloatValue *)right)->getValue());
    engine->stackPush(result);
    return 1;
  });

  intType->registerDiv(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stackPop();
    Value *left = engine->stackPop();

    Value *result = new IntegerValue(((IntegerValue *)left)->getValue() %
                                     ((IntegerValue *)right)->getValue());
    engine->stackPush(result);
    return 1;
  });

  intType->registerMod(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stackPop();
    Value *left = engine->stackPop();

    plsm_float_t result = fmod(((IntegerValue *)left)->getValue(),
                               ((FloatValue *)right)->getValue());
    engine->stackPush(new FloatValue(result));
    return 1;
  });

  intType->registerMod(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));
}

void Type::setupBooleanType(Type *intType, Type *floatType, Type *boolType) {
  if (boolType->hasRegisteredFunctions())
    return;
}
