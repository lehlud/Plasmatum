#include "value.hh"

#include "engine.hh"
#include "instruction.hh"

#include <iostream>

namespace plsm {

plsm_float_t Value::asFloat() { return ((FloatValue *)this)->getValue(); }
plsm_int_t Value::asInteger() { return ((IntegerValue *)this)->getValue(); }
plsm_bool_t Value::asBoolean() { return ((BooleanValue *)this)->getValue(); }

void FunctionValue::call(Engine *engine) {
  plsm_size_t *indexPointer = new plsm_size_t;
  *indexPointer = 0;

  std::shared_ptr<Instruction> ip = getInstruction(*indexPointer);

  plsm_size_t *prevJumpIndexPointer = engine->getJumpIndexPointer();
  engine->setJumpIndexPointer(indexPointer);

  while ((ip = getInstruction(*indexPointer)) && !ip->isReturn()) {
    // std::cout << ip->toString() << "(" << *indexPointer << ")" << std::endl;
    *indexPointer += ip->execute(engine);
  }

  delete indexPointer;
  engine->setJumpIndexPointer(prevJumpIndexPointer);
}

} // namespace plsm
