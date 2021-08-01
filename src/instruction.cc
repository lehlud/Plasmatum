#include "instruction.hh"

#include "type.hh"
#include "value.hh"
#include "engine.hh"

plsm_size_t JumpInstruction::execute(Engine *engine) {
  engine->jump(destination);
  return 0;
}

plsm_size_t LoadConstInstruction::execute(Engine *engine) {
  engine->stackPush(value);
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

plsm_size_t CallInstruction::execute(Engine *engine) {
  engine->call(argc);
  return 1;
}

plsm_size_t FunctionStartInstruction::execute(Engine *engine) {
  std::vector<Instruction *> body;

  plsm_size_t index = engine->getIndex() + 1;

  Instruction *ip;
  while (!(ip = engine->getInstruction(index++))->isFunctionFinish()) {
    body.push_back(ip);
  }

  engine->stackPush(new FunctionValue(argc, body));

  return 1 + body.size();
}

plsm_size_t DefineGlobalInstruction::execute(Engine *engine) {
  engine->defineGlobal(id, engine->stackPop());
  return 1;
}
