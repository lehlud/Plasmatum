#include "type.hh"

#include <cmath>

#include "engine.hh"
#include "instruction.hh"
#include "value.hh"

void Type::cast(Engine *engine, Type *type) {
  FunctionValue *castFunction = castFunctions[type];
  castFunction->call(engine);
}

void Type::add(Engine *engine) {
  Type *type = engine->stack_peek()->type;

  FunctionValue *addFunction = addFunctions[type];
  addFunction->call(engine);
}

void Type::sub(Engine *engine) {
  Type *type = engine->stack_peek()->type;

  FunctionValue *subFunction = subFunctions[type];
  subFunction->call(engine);
}

void Type::mul(Engine *engine) {
  Type *type = engine->stack_peek()->type;

  FunctionValue *mulFunction = mulFunctions[type];
  mulFunction->call(engine);
}

void Type::div(Engine *engine) {
  Type *type = engine->stack_peek()->type;

  FunctionValue *divFunction = divFunctions[type];
  divFunction->call(engine);
}

void Type::mod(Engine *engine) {
  Type *type = engine->stack_peek()->type;

  FunctionValue *modFunction = modFunctions[type];
  modFunction->call(engine);
}

std::map<std::string, Type *> Type::getStandardTypes() {
  std::map<std::string, Type *> result;

  Type *intT = getIntegerType();
  Type *floatT = getFloatType();
  Type *boolT = getBooleanType();

  setupFloatType(intT, floatT, boolT);
  setupIntegerType(intT, floatT, boolT);
  setupBooleanType(intT, floatT, boolT);

  std::vector<Instruction *> tmpInstructions;
  intT->registerAdd(intT, new FunctionValue(2, tmpInstructions));

  tmpInstructions.clear();

  tmpInstructions.push_back(new CustomInstruction([](Engine *engine) {
    Value *right = engine->stack_pop();
    Value *left = engine->stack_pop();

    Value *result = new IntegerValue(((IntegerValue *)left)->getValue() +
                                     ((IntegerValue *)right)->getValue());
    engine->stack_push(result);
    return 1;
  }));
  tmpInstructions.push_back(new ReturnInstruction());
  intT->registerAdd(intT, new FunctionValue(2, tmpInstructions));

  return result;
}

void Type::setupFloatType(Type *intType, Type *floatType, Type *boolType) {}

void Type::setupIntegerType(Type *intType, Type *floatType, Type *boolType) {
  Instruction *tmpInst = nullptr;

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stack_pop();
    Value *left = engine->stack_pop();

    Value *result = new IntegerValue(((IntegerValue *)left)->getValue() +
                                     ((IntegerValue *)right)->getValue());
    engine->stack_push(result);
    return 1;
  });

  intType->registerAdd(
      intType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stack_pop();
    Value *left = engine->stack_pop();

    Value *result = new FloatValue(((IntegerValue *)left)->getValue() +
                                   ((FloatValue *)right)->getValue());
    engine->stack_push(result);
    return 1;
  });

  intType->registerAdd(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stack_pop();
    Value *left = engine->stack_pop();

    Value *result = new IntegerValue(((IntegerValue *)left)->getValue() -
                                     ((IntegerValue *)right)->getValue());
    engine->stack_push(result);
    return 1;
  });

  intType->registerSub(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stack_pop();
    Value *left = engine->stack_pop();

    Value *result = new FloatValue(((IntegerValue *)left)->getValue() -
                                   ((FloatValue *)right)->getValue());
    engine->stack_push(result);
    return 1;
  });

  intType->registerSub(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stack_pop();
    Value *left = engine->stack_pop();

    Value *result = new IntegerValue(((IntegerValue *)left)->getValue() *
                                     ((IntegerValue *)right)->getValue());
    engine->stack_push(result);
    return 1;
  });

  intType->registerMul(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stack_pop();
    Value *left = engine->stack_pop();

    Value *result = new FloatValue(((IntegerValue *)left)->getValue() *
                                   ((FloatValue *)right)->getValue());
    engine->stack_push(result);
    return 1;
  });

  intType->registerMul(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stack_pop();
    Value *left = engine->stack_pop();

    Value *result = new FloatValue(((IntegerValue *)left)->getValue() /
                                   ((IntegerValue *)right)->getValue());
    engine->stack_push(result);
    return 1;
  });

  intType->registerDiv(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stack_pop();
    Value *left = engine->stack_pop();

    Value *result = new FloatValue(((IntegerValue *)left)->getValue() /
                                   ((FloatValue *)right)->getValue());
    engine->stack_push(result);
    return 1;
  });

  intType->registerDiv(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stack_pop();
    Value *left = engine->stack_pop();

    Value *result = new IntegerValue(((IntegerValue *)left)->getValue() %
                                     ((IntegerValue *)right)->getValue());
    engine->stack_push(result);
    return 1;
  });

  intType->registerMod(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));

  tmpInst = new CustomInstruction([](Engine *engine) {
    Value *right = engine->stack_pop();
    Value *left = engine->stack_pop();

    plsm_float_t result = fmod(((IntegerValue *)left)->getValue(), ((FloatValue *)right)->getValue());
    engine->stack_push(new FloatValue(result));
    return 1;
  });

  intType->registerMod(
      floatType, new FunctionValue(2, {tmpInst, new ReturnInstruction()}));
}

void Type::setupBooleanType(Type *intType, Type *floatType, Type *boolType) {}
