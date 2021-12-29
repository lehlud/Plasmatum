#include "compiler.hh"

#include <llvm/Support/TargetSelect.h>

#include "context.hh"

#include <iostream>

void compileAndExecute(std::vector<Stmt *> stmts) {
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  Context context;
  context.createMain(stmts);

  context.optimizeIR();
  // context.print();

  execute(context);
}

void execute(Context &context) {
  auto &engine = context.getExecutionEngine();
  auto address = engine.getFunctionAddress("main");
  auto main = reinterpret_cast<void (*)()>(address);
  main();
}
