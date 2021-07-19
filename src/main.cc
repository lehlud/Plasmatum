#include "ast.hh"
#include "context.hh"
#include "lib.hh"
#include "parser.hh"

#include <llvm/ADT/APInt.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <llvm/Support/TargetSelect.h>

#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>

#include <iostream>
#include <vector>

void initLLVM() {
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmPrinters();
}

extern "C" int64_t foo() {
  std::cout << "Hello World!" << std::endl;
  return 1;
}

int main(int argc, char **argv) {
  if (argc <= 1) {
    // error here
  }

  Parser parser(U"define fib(x) = if x < 2 x else fib(x - 1) + fib(x - 2)\ndefine main(argc) = "
                U"println(fib(30))");

  std::vector<Stmt *> stmts;
  Stmt *stmt = nullptr;
  while ((stmt = parser.parseStmt())) {
    stmts.push_back(stmt);
    std::cout << to_str(stmt->to_string()) << std::endl;
  }

  // int32_t plsm_str[] = {72, 101, 108, 108, 111, 32, 87, 111, 114, 108, 100,
  // 33, 0}; plsm_val args[] = {(plsm_val){TYPE_STRING, (int8_t *)plsm_str}};
  // println(1, args);

  initLLVM();

  PlsmContext context;
  for (auto &tl_stmt : stmts) {
    tl_stmt->genCode(context);
  }

  auto mainFunc = context.getMain();

  std::cout << "------------------------------------------------" << std::endl;
  context.optimize();
  context.printLLVMIR();
  std::cout << "------------------------------------------------" << std::endl;

  auto &engine = context.getExecutionEngine();

  auto address = engine.getFunctionAddress("main");
  auto finalFunc = (int8_t(*)(int))address;
  finalFunc(20);

  return 0;
}
