#include "engine.hh"

#include "instruction.hh"
#include "type.hh"
#include "value.hh"

namespace plsm {

void Engine::stackPushGlobal(const std::string &id) {
  stack.push_back(globals.count(id) ? globals[id] : new UndefinedValue());
}

void Engine::call(size_t argc) {
  Value *value = stackPop();
  if (!value->isFunction()) {
    while (argc > 0) {
      stackPop();
      argc -= 1;
    }
    stackPush(new UndefinedValue());
  }

  FunctionValue *callee = (FunctionValue *)value;

  while (argc > callee->getArgc()) {
    stackPop();
    argc -= 1;
  }

  while (argc < callee->getArgc()) {
    stackPush(new UndefinedValue());
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
