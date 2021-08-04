#include "engine.hh"

#include "instruction.hh"
#include "type.hh"
#include "value.hh"

#include <iostream>
#include <memory>

namespace plsm {

execution_engine::~execution_engine() {
  for (auto &function : functions) {
    delete function;
  }
}

void execution_engine::stack_push_global(const std::string &id) {
  _stack.push_back(_globals.count(id) ? _globals[id]->copy() : new undefined());
}

void execution_engine::call(plsm_size_t argc) {
  value *stack_top = stack_peek();
  stack_pop_no_delete();

  if (!stack_top->is_function()) {
    while (argc > 0) {
      stack_pop();
      argc -= 1;
    }
    stack_push(new undefined());
    delete stack_top;
    return;
  }
  
  function *callee = ((function_pointer *)stack_top)->get_function();

  plsm_size_t calleeArgc = callee->get_argc();
  if (calleeArgc > 0) {
    bool callArgcBigger = argc > calleeArgc;
    plsm_size_t iterationMin = callArgcBigger ? argc - calleeArgc : 0;

    for (plsm_size_t i = argc; i > iterationMin; i--) {
      argument_push(stack_peek(i - 1));
    }

    if (callArgcBigger) {
      for (plsm_size_t i = 0; i < calleeArgc - argc; i++) {
        argument_push(new undefined());
      }
    }
  }

  for (plsm_size_t i = 0; i < argc; i++) {
    stack_pop();
  }

  callee->call(this);

  for (plsm_size_t i = 0; i < calleeArgc; i++) {
    argument_pop();
  }

  delete stack_top;
}

int execution_engine::execute(const std::vector<std::string> &args) {
  while ((_ip = get_instruction(_index))) {
    /*std::cout << "engine: executing " << ip->code << std::endl;*/
    _index += _ip->execute(this);
  }

  return args.size();
}

}
