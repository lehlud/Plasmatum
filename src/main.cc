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

  Parser parser(U"'asdfjkl;' 4 4.666 asdf");
  Expr *expr = nullptr;
  while ((expr = parser.parseExpr()))
    std::cout << to_str(expr->to_string()) << std::endl;

  // int32_t plsm_str[] = {72, 101, 108, 108, 111, 32, 87, 111, 114, 108, 100,
  // 33, 0}; plsm_val args[] = {(plsm_val){TYPE_STRING, (int8_t *)plsm_str}};
  // println(1, args);

  initLLVM();

  std::vector<Stmt *> fibBody = {new ReturnStmt(new IfExpr(
      new LTBinExpr(new VarExpr("x"), new IntExpr(2)), new VarExpr("x"),
      new AddBinExpr(new CallExpr("fib", {new SubBinExpr(new VarExpr("x"),
                                                         new IntExpr(1))}),
                     new CallExpr("fib", {new SubBinExpr(new VarExpr("x"),
                                                         new IntExpr(2))}))))};
  auto testFunc = new FunctionStmt("fib", fibBody, {"x"});

  std::vector<Stmt *> body = {
      new ExprStmt(
          new CallExpr("println", {new StringExpr(U"This is unicode! Σίγμα")})),
      new ReturnStmt(new CallExpr(
          "println", {new CallExpr("fib", {new VarExpr("argc")})})),
  };
  auto f = new FunctionStmt("main", body, {"argc"});

  PlsmContext context;

  testFunc->genCode(context);
  auto func = (llvm::Function *)f->genCode(context);

  auto mainFunc = context.getMain();

  context.optimize();
  // context.printLLVMIR();
  std::cout << "------------------------------------------------" << std::endl;

  std::string errString;
  llvm::raw_string_ostream str(errString);
  if (llvm::verifyFunction(*func, &str)) {
    std::cout << "error: " << errString << std::endl;
  }

  auto &engine = context.getExecutionEngine();

  auto address = engine.getFunctionAddress("main");
  auto finalFunc = (int8_t(*)(int))address;
  finalFunc(30);

  return 0;
}
