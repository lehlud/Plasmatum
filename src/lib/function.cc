#include "function.hh"

#include "engine.hh"
#include "instruction.hh"

namespace plsm {

function::~function() {
  for (auto &inst : instructions) {
    delete inst;
  }
}

void function::call(execution_engine *engine) {
  plsm_size_t *indexPointer = new plsm_size_t;
  *indexPointer = 0;

  instruction *ip = get_instruction(*indexPointer);

  plsm_size_t *prevJumpIndexPointer = engine->jump_index_ptr();
  engine->set_jump_index_ptr(indexPointer);

  while ((ip = get_instruction(*indexPointer))->code !=
         instruction::code_return) {
    // std::cout << ip->to_string() << "(" << *indexPointer << ")" <<
    // std::endl; std::cout << "function: executing " << ip->to_string() <<
    // std::endl;
    *indexPointer += ip->execute(engine);
  }

  delete indexPointer;
  engine->set_jump_index_ptr(prevJumpIndexPointer);
}

}
