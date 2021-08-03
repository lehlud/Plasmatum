#include "value.hh"

#include "engine.hh"
#include "instruction.hh"

#include <iostream>

namespace plsm {

plsm_float_t value::as_floating_point() { return ((floating_point *)this)->getValue(); }
plsm_int_t value::as_integer() { return ((integer *)this)->getValue(); }
plsm_bool_t value::as_boolean() { return ((boolean *)this)->getValue(); }

undefined *undefined::copy() {
  return new undefined();
}

integer *integer::copy() {
  return new integer(int_v);
}

floating_point *floating_point::copy() {
  return new floating_point(float_v);
}

boolean *boolean::copy() {
  return new boolean(bool_v);
}

function::~function() {
  for (auto &inst : instructions) {
    delete inst;
  }
}

function *function::copy() {
  std::vector<instruction *> copied;
  for (auto &inst : instructions) {
    copied.push_back(inst->copy());
  }
  return new function(argc, copied);
}

void function::call(execution_engine *engine) {
  plsm_size_t *indexPointer = new plsm_size_t;
  *indexPointer = 0;

  instruction *ip = get_instruction(*indexPointer);

  plsm_size_t *prevJumpIndexPointer = engine->getJumpIndexPointer();
  engine->setJumpIndexPointer(indexPointer);

  while ((ip = get_instruction(*indexPointer))->code != instruction::code_return) {
    // std::cout << ip->to_string() << "(" << *indexPointer << ")" << std::endl;
    // std::cout << "function: executing " << ip->to_string() << std::endl;
    *indexPointer += ip->execute(engine);
  }

  delete indexPointer;
  engine->setJumpIndexPointer(prevJumpIndexPointer);
}

} // namespace plsm
