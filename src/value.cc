#include "value.hh"

#include "engine.hh"
#include "instruction.hh"

void FunctionValue::call(Engine *engine) {
  plsm_size_t index = 0;
  Instruction *ip = nullptr;
  while (!(ip = getInstruction(index))->isReturn()) {
    index += ip->execute(engine);
  }

  Value *result = engine->stack_pop();

  for (plsm_size_t i = 0; i < argc; i++) {
    engine->stack_pop();
  }

  engine->stack_push(result);
}
