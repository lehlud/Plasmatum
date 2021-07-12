#include "ast.hh"
#include "lib.hh"
#include "utils.hh"

#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/ADT/APInt.h>

#include <llvm/Support/TargetSelect.h>

#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>

#include <vector>
#include <iostream>

extern llvm::Module Module;

void initLLVM() {
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmPrinters();
}

void initializeModule() {
  auto targetMachine = llvm::EngineBuilder().selectTarget();
  auto dataLayout = targetMachine->createDataLayout();

  Module.setDataLayout(dataLayout);

  initFunctions();
}

extern "C" int64_t foo() {
  std::cout << "Hello World!" << std::endl;
  return 1;
}

int main(int argc, char **argv) {
  if (argc <= 1) {
    // error here
  }

  initLLVM();
  initializeModule();

  auto call = new CallExpr("println", {new IntExpr(42)});
  std::vector<Stmt *> body = {new ExprStmt(call)};//{new ReturnStmt(call)};
  auto f = new FunctionStmt("main", body, {});

  auto func = (llvm::Function *) f->genCode();

  std::string errString;
  llvm::raw_string_ostream str(errString);
  if (llvm::verifyFunction(*func, &str)) {
    std::cout << "error: " << errString << std::endl;
  }

  auto mainFunc = createMain();

  auto &engine = getExecutionEngine();

  Module.print(llvm::errs(), 0);

  engine.runFunctionAsMain(mainFunc, {}, nullptr);

  return 0;
}
