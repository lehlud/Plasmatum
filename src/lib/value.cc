#include "value.hh"

#include "engine.hh"
#include "instruction.hh"

namespace plsm {

plsm_float_t Value::asFloat() { return ((FloatValue *)this)->getValue(); }
plsm_int_t Value::asInteger() { return ((IntegerValue *)this)->getValue(); }
plsm_bool_t Value::asBoolean() { return ((BooleanValue *)this)->getValue(); }

void FunctionValue::call(Engine *engine) {
  plsm_size_t index = 0;
  Instruction *ip = nullptr;
  while ((ip = getInstruction(index)) && !ip->isReturn()) {
    index += ip->execute(engine);
  }

  // Value *result = engine->stackPop();

  // for (plsm_size_t i = 0; i < argc; i++) {
  //   engine->stackPop();
  // }

  // engine->stackPush(result);
}

}
