#include "engine.hh"

#include "instruction.hh"
#include "type.hh"
#include "value.hh"

namespace plsm {

void Engine::stackPushGlobal(const std::string &id) {
  stack.push_back(globals.count(id) ? globals[id] : UndefinedValue::get());
}

void Engine::call(size_t argc) {
  std::shared_ptr<Value> value = stackPop();
  if (!value->isFunction()) {
    while (argc > 0) {
      stackPop();
      argc -= 1;
    }
    stackPush(UndefinedValue::get());
  }

  std::shared_ptr<FunctionValue> callee = std::static_pointer_cast<FunctionValue>(value);

  while (argc > callee->getArgc()) {
    stackPop();
    argc -= 1;
  }

  while (argc < callee->getArgc()) {
    stackPush(UndefinedValue::get());
    argc += 1;
  }

  callee->call(this);
}

int Engine::execute(const std::vector<std::string> &args) {
  while ((ip = getInstruction(index))) {
    index += ip->execute(this);
  }

  return args.size();
}

}
