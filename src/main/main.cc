#include "ast.hh"
#include "engine.hh"
#include "instruction.hh"
#include "parser.hh"
#include "type.hh"
#include "value.hh"

#include <iostream>
#include <vector>

using namespace plsm;

function *printFunc() {
  instruction *tmpInst = new value_inst<custom_inst_function>(
      instruction::code_custom,
      new custom_inst_function([](execution_engine *engine) {
        std::cout << engine->argument_peek()->to_string() << std::endl;
        engine->stack_push(new undefined());
        return 1;
      }));
  return new function(1, {tmpInst, new instruction(instruction::code_return)});
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

  std::vector<instruction *> insts = LLParser::fromFile(argv[1])->parse();

  auto types = Type::getStandardTypes();

  execution_engine *engine = new execution_engine(insts, {});
  engine->define_global("print", new function_pointer(printFunc()));

  return engine->execute({});
}
