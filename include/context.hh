#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>

#include <vector>

class Stmt;

#define _VAR_VALUE_TYPE std::pair<llvm::Type *, llvm::Value *>
#define _VAR_SCOPE_TYPE std::map<std::string, _VAR_VALUE_TYPE>
class Context {
public:
  llvm::DataLayout dataLayout;
  llvm::LLVMContext llvmContext;
  llvm::Module module;
  llvm::IRBuilder<> builder;

  std::map<std::string, llvm::Type *> namedTypes;
  std::map<std::string, llvm::Function *> builtins;
  _VAR_SCOPE_TYPE namedValues;

  Context();

  auto getNumberType() { return numberType; }
  auto getCharType() { return charType; }
  auto getPointerType() { return pointerType; }
  auto getDummyFunctionType() { return dummyFunctionType; }

  void store(llvm::Value *value, std::string name);

  void optimizeIR();
  void print();
  void createMain(std::vector<Stmt *> stmts);

  llvm::ExecutionEngine &getExecutionEngine();

private:
  void initTypes();
  void initFunctions();

  llvm::Type *numberType;
  llvm::IntegerType *charType;
  llvm::PointerType *pointerType;
  llvm::FunctionType *dummyFunctionType;
};
