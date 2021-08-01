#include "engine.hh"

#include "type.hh"
#include "value.hh"
#include "instruction.hh"

void Engine::call(size_t argc) {
  FunctionValue *callee = (FunctionValue *)stackPop();

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
