#include "engine.hh"

#include "instruction.hh"
#include "type.hh"
#include "value.hh"

#include <iostream>
#include <memory>

namespace plsm {

void execution_engine::stackPushGlobal(const std::string &id) {
  stack.push_back(globals.count(id) ? globals[id] : new undefined());
}

void execution_engine::call(plsm_size_t argc) {
  value *stack_top = stackPeek();
  stackPop(false);

  if (!stack_top->is_function()) {
    while (argc > 0) {
      stackPop();
      argc -= 1;
    }
    stackPush(new undefined());
    return;
  }
  
  function *callee = (function *)stack_top;

  plsm_size_t calleeArgc = callee->get_argc();
  if (calleeArgc > 0) {
    bool callArgcBigger = argc > calleeArgc;
    plsm_size_t iterationMin = callArgcBigger ? argc - calleeArgc : 0;

    for (plsm_size_t i = argc; i > iterationMin; i--) {
      argumentPush(stackPeek(i - 1));
    }

    if (callArgcBigger) {
      for (plsm_size_t i = 0; i < calleeArgc - argc; i++) {
        argumentPush(new undefined());
      }
    }
  }

  for (plsm_size_t i = 0; i < argc; i++) {
    stackPop();
  }

  callee->call(this);

  for (plsm_size_t i = 0; i < calleeArgc; i++) {
    argumentPop();
  }
}

int execution_engine::execute(const std::vector<std::string> &args) {
  while ((ip = get_instruction(index))) {
    /*std::cout << "engine: executing " << ip->code << std::endl;*/
    index += ip->execute(this);
  }

  return args.size();
}

}
