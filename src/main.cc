#include "ast.hh"
#include "context.hh"
#include "lib.hh"

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

  // int32_t plsm_str[] = {72, 101, 108, 108, 111, 32, 87, 111, 114, 108, 100, 33, 0};
  // plsm_val args[] = {(plsm_val){TYPE_STRING, (int8_t *)plsm_str}};
  // println(1, args);

  initLLVM();

  auto arg = new IfExpr(new IntExpr(0), new StringExpr(U"Hallo Welt!"), new IntExpr(42));
  auto call = new CallExpr("println", {arg});
  std::vector<Stmt *> body = {new ReturnStmt(call)};
  auto f = new FunctionStmt("main", body, {});

  PlsmContext context;

  auto func = (llvm::Function *)f->genCode(context);

  auto mainFunc = context.getMain();

  context.printLLVMIR();
  std::cout << "---------------" << std::endl;

  std::string errString;
  llvm::raw_string_ostream str(errString);
  if (llvm::verifyFunction(*func, &str)) {
    std::cout << "error: " << errString << std::endl;
  }

  auto &engine = context.getExecutionEngine();

  engine.runFunctionAsMain(mainFunc, {}, nullptr);

  return 0;
}
