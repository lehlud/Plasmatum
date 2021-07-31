#include "type.hh"

#include "value.hh"
#include "engine.hh"

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

  return result;
}
