#include "ast.hh"
#include "utils.hh"

#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>

extern llvm::Module Module;
extern llvm::LLVMContext Context;
extern llvm::IRBuilder<> Builder;

llvm::Value *IntExpr::genCode() {
  extern llvm::Type *IntType;
  return llvm::ConstantInt::get(IntType, value);
}

llvm::Value *FloatExpr::genCode() {
  extern llvm::Type *FloatType;
  return llvm::ConstantFP::get(FloatType, value);
}

llvm::Value *AddBinExpr::genCode() { return nullptr; }

llvm::Value *SubBinExpr::genCode() { return nullptr; }

llvm::Value *MulBinExpr::genCode() { return nullptr; }

llvm::Value *DivBinExpr::genCode() { return nullptr; }

llvm::Value *LambdaExpr::genCode() {
  extern llvm::Type *PlsmType;
  auto functionType = llvm::FunctionType::get(PlsmType, false);
  auto function = llvm::Function::Create(
      functionType, llvm::Function::ExternalLinkage, "", Module);

  auto previousBasicBlock = Builder.GetInsertBlock();
  auto functionBasicBlock = llvm::BasicBlock::Create(Context, "", function);

  Builder.SetInsertPoint(functionBasicBlock);

  for (auto &statement : body) {
    statement->genCode();
  }

  Builder.CreateRet(NullValue());

  Builder.SetInsertPoint(previousBasicBlock);

  return function;
}

llvm::Value *DefineGlobalStmt::genCode() { return nullptr; }

llvm::Value *DefineFunctionStmt::genCode() { return nullptr; }
