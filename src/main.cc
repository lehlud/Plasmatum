#include "ast.hh"
#include "parser.hh"
#include "engine.hh"
#include "type.hh"
#include "instruction.hh"
#include "value.hh"

#include <iostream>
#include <vector>

FunctionValue *printFunc() {
  Instruction *tmpInst = new CustomInstruction([](Engine *engine) {
    std::cout << engine->stackPop()->toString() << std::endl;
    engine->stackPush(new UndefinedValue());
    return 1;
  });
  return new FunctionValue(1, {tmpInst, new ReturnInstruction()});
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

  std::u32string text = readFile(argv[1]);

  Parser parser(text);

  std::vector<Stmt *> stmts;
  Stmt *stmt = nullptr;
  while ((stmt = parser.parseStmt())) {
    stmts.push_back(stmt);
    // std::cout << to_str(stmt->to_string()) << std::endl;
  }

  auto types = Type::getStandardTypes();

  Engine *engine = new Engine({types["Int"], types["Float"]}, {});

  engine->stackPush(new FloatValue(0.1));
  engine->stackPush(new FloatValue(0.2));

  (new AddInstruction())->execute(engine);

  engine->stackPush(printFunc());

  engine->call(1);

  return (long)engine;
}
