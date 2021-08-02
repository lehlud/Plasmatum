#include "ast.hh"
#include "engine.hh"
#include "instruction.hh"
#include "parser.hh"
#include "type.hh"
#include "value.hh"

#include <iostream>
#include <vector>

using namespace plsm;

std::shared_ptr<FunctionValue> printFunc() {
  std::shared_ptr<Instruction> tmpInst = CustomInstruction::get([](Engine *engine) {
    std::cout << engine->argumentPeek()->toString() << std::endl;
    engine->stackPush(UndefinedValue::get());
    return 1;
  });
  return FunctionValue::get(1, {tmpInst, ReturnInstruction::get()});
}

void printUsage(char *arg0) {
  std::cout << "usage: " << arg0 << " <file>" << std::endl;
}

int main(int argc, char **argv) {
  if (argc <= 0) {
    return 1;
  } else if (argc <= 1) {
    printUsage(argv[0]);
    return 1;
  }

  std::vector<std::shared_ptr<Instruction>> insts = LLParser::fromFile(argv[1])->parse();

  auto types = Type::getStandardTypes();

  Engine *engine = new Engine({}, insts);
  engine->defineGlobal("print", printFunc());

  return engine->execute({});
}
