#include "instruction.hh"

#include <string>

#include "type.hh"
#include "value.hh"
#include "engine.hh"

namespace plsm {

plsm_size_t JumpInstruction::execute(Engine *engine) {
  engine->jump(destination);
  return 0;
}

plsm_size_t JumpCondInstruction::execute(Engine *engine) {
  plsm_size_t result = 1;
  if (engine->stackPeek()->isTruthy()) {
    engine->jump(destination);
    result = 0;
  }

  engine->stackPop();
  return result;
}

plsm_size_t LoadConstInstruction::execute(Engine *engine) {
  engine->stackPush(value);
  return 1;
}

plsm_size_t LoadArgInstruction::execute(Engine *engine) {
  engine->stackPush(engine->argumentPeek(back));
  return 1;
}

plsm_size_t LoadGlobalInstruction::execute(Engine *engine) {
  engine->stackPushGlobal(id);
  return 1;
}

plsm_size_t CastInstruction::execute(Engine *engine) {
  type->cast(engine, engine->stackPeek()->type);
  return 1;
}

plsm_size_t CustomInstruction::execute(Engine *engine) {
  return executeFunction(engine);
}

plsm_size_t AddInstruction::execute(Engine *engine) {
  engine->stackPeek(1)->type->add(engine);
  return 1;
}

plsm_size_t SubInstruction::execute(Engine *engine) {
  engine->stackPeek(1)->type->sub(engine);
  return 1;
}

plsm_size_t MulInstruction::execute(Engine *engine) {
  engine->stackPeek(1)->type->mul(engine);
  return 1;
}

plsm_size_t DivInstruction::execute(Engine *engine) {
  engine->stackPeek(1)->type->div(engine);
  return 1;
}

plsm_size_t ModInstruction::execute(Engine *engine) {
  engine->stackPeek(1)->type->mod(engine);
  return 1;
}

plsm_size_t EQInstruction::execute(Engine *engine) {
  engine->stackPeek(1)->type->eq(engine);
  return 1;
}

plsm_size_t NEInstruction::execute(Engine *engine) {
  engine->stackPeek(1)->type->ne(engine);
  return 1;
}

plsm_size_t GTInstruction::execute(Engine *engine) {
  engine->stackPeek(1)->type->gt(engine);
  return 1;
}

plsm_size_t GEInstruction::execute(Engine *engine) {
  engine->stackPeek(1)->type->ge(engine);
  return 1;
}

plsm_size_t LTInstruction::execute(Engine *engine) {
  engine->stackPeek(1)->type->lt(engine);
  return 1;
}

plsm_size_t LEInstruction::execute(Engine *engine) {
  engine->stackPeek(1)->type->le(engine);
  return 1;
}

plsm_size_t CallInstruction::execute(Engine *engine) {
  engine->call(argc);
  return 1;
}

plsm_size_t FunctionStartInstruction::execute(Engine *engine) {
  std::vector<std::shared_ptr<Instruction>> body;

  plsm_size_t index = engine->getIndex() + 1;

  std::shared_ptr<Instruction> ip;
  while (!(ip = engine->getInstruction(index++))->isFunctionFinish()) {
    body.push_back(ip);
  }

  engine->stackPush(FunctionValue::get(argc, body));

  return 1 + body.size();
}

plsm_size_t DefineGlobalInstruction::execute(Engine *engine) {
  engine->defineGlobal(id, engine->stackPeek());
  engine->stackPop();
  return 1;
}

}
