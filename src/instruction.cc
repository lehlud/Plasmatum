#include "instruction.hh"

#include "type.hh"
#include "value.hh"
#include "engine.hh"

plsm_size_t ReturnInstruction::execute(Engine *) { return 1; }

plsm_size_t JumpInstruction::execute(Engine *engine) {
  engine->jump(destination);
  return 0;
}

plsm_size_t LoadConstInstruction::execute(Engine *engine) {
  engine->stack_push(value);
  return 1;
}

plsm_size_t LoadGlobalInstruction::execute(Engine *engine) {
  engine->stack_push_global(id);
  return 1;
}

plsm_size_t CastInstruction::execute(Engine *engine) {
  type->cast(engine, engine->stack_peek()->type);
  return 1;
}

plsm_size_t CustomInstruction::execute(Engine *engine) {
  return executeFunction(engine);
}

plsm_size_t AddInstruction::execute(Engine *engine) {
  engine->stack_peek(1)->type->add(engine);
  return 1;
}

plsm_size_t SubInstruction::execute(Engine *engine) {
  engine->stack_peek(1)->type->sub(engine);
  return 1;
}

plsm_size_t MulInstruction::execute(Engine *engine) {
  engine->stack_peek(1)->type->mul(engine);
  return 1;
}

plsm_size_t DivInstruction::execute(Engine *engine) {
  engine->stack_peek(1)->type->div(engine);
  return 1;
}

plsm_size_t ModInstruction::execute(Engine *engine) {
  engine->stack_peek(1)->type->mod(engine);
  return 1;
}
