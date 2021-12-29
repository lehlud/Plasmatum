#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

class Context {
public:
  llvm::Module module;
  llvm::IRBuilder<> builder;
  llvm::LLVMContext llvmContext;

  std::map<std::string, llvm::Type *> namedTypes;
  std::map<std::string, llvm::Value *> namedValues;

  Context() : module("", llvmContext), builder(llvmContext) { initTypes(); }

  llvm::Type *getIntegerType() { return integerType; }
  llvm::Type *getFractionType() { return fractionType; }
  llvm::Type *getGenericPointerType() { return genericPointerType; }

private:
  void initTypes() {
    integerType = builder.getInt64Ty();
    fractionType = llvm::StructType::create({integerType, integerType});
    genericPointerType = llvm::PointerType::get(builder.getVoidTy(), 0);
  }

  llvm::Type *integerType;
  llvm::Type *fractionType;
  llvm::Type *genericPointerType;
};
