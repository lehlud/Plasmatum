#pragma once

#include <llvm/ExecutionEngine/ExecutionEngine.h>

class Engine {
public:
  llvm::ExecutionEngine executionEngine;
  Engine(llvm::DataLayout dataLayout, llvm::Module *module)
      : executionEngine(dataLayout, module) {}

}
;
