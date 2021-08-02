#include "engine.hh"

#include "instruction.hh"
#include "type.hh"
#include "value.hh"

#include <iostream>

namespace plsm {

void Engine::stackPushGlobal(const std::string &id) {
  stack.push_back(globals.count(id) ? globals[id] : UndefinedValue::get());
}

void Engine::call(plsm_size_t argc) {
  std::shared_ptr<Value> value = stackPeek();
  stackPop();

  if (!value->isFunction()) {
    while (argc > 0) {
      stackPop();
      argc -= 1;
    }
    stackPush(UndefinedValue::get());
    return;
  }

  std::shared_ptr<FunctionValue> function = std::static_pointer_cast<FunctionValue>(value);

  plsm_size_t functionArgc = function->getArgc();
  if (functionArgc > 0) {
    bool callArgcBigger = argc > functionArgc;
    plsm_size_t iterationMin = callArgcBigger ? argc - functionArgc : 0;

    for (plsm_size_t i = argc; i > iterationMin; i--) {
      argumentPush(stackPeek(i - 1));
    }

    if (callArgcBigger) {
      for (plsm_size_t i = 0; i < functionArgc - argc; i++) {
        argumentPush(UndefinedValue::get());
      }
    }
  }
  
  for (plsm_size_t i = 0; i < argc; i++) {
    stackPop();
  }

  function->call(this);

  for (plsm_size_t i = 0; i < functionArgc; i++) {
    argumentPop();
  }
}

int Engine::execute(const std::vector<std::string> &args) {
  while ((ip = getInstruction(index))) {
    index += ip->execute(this);
  }

  return args.size();
}

}
