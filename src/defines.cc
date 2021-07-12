#include "defines.hh"
#include "ast.hh"
#include "lib.hh"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

#include <map>
#include <memory>
#include <vector>

llvm::LLVMContext Context;
llvm::Module Module("", Context);
llvm::IRBuilder<> Builder(Context);

llvm::Type *IntType = llvm::IntegerType::get(Context, INT_SIZE);
llvm::Type *FloatType = llvm::Type::getDoubleTy(Context);

llvm::Type *TypeType = llvm::Type::getInt8Ty(Context);
llvm::PointerType *PointerType = llvm::Type::getInt8PtrTy(Context);
llvm::StructType *PlsmType =
    llvm::StructType::get(Context, {TypeType, PointerType});

llvm::FunctionType *FunctionType = llvm::FunctionType::get(
    PlsmType, {IntType, PlsmType->getPointerTo()}, false);

typedef std::map<std::string, llvm::Value *> var_scope_t;
std::vector<var_scope_t> VarScopes;

std::map<std::string, llvm::Function *> Functions;

std::string objName;
int8_t valueType = -1;

llvm::ExecutionEngine &getExecutionEngine() {
  auto &result = *(llvm::EngineBuilder(std::unique_ptr<llvm::Module>(&Module))
                       .setEngineKind(llvm::EngineKind::JIT)
                       .setOptLevel(llvm::CodeGenOpt::Level::Aggressive)
                       .setRelocationModel(llvm::Reloc::Model::PIC_)
                       .create());

  result.DisableSymbolSearching();

  result.addGlobalMapping(Functions["print"], (void *)&print);
  result.addGlobalMapping(Functions["println"], (void *)&println);

  result.finalizeObject();
  return result;
}

void initFunctions() {
  std::vector<std::string> funcs = {"print", "println"};
  for (auto &element : funcs) {
    auto func = llvm::Function::Create(
        FunctionType, llvm::Function::ExternalLinkage, element, Module);
    Functions[element] = func;
  }
}

void resetScopes() { VarScopes.clear(); }

void resetFunctions() { Functions.clear(); }

void resetFlags() {
  objName.clear();
  valueType = -1;
}
