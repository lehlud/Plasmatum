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

llvm::Value *AddBinExpr::genCode() {
  auto leftV = left->genCode();
  auto rightV = right->genCode();

  return createAdd(leftV, rightV);
}

llvm::Value *SubBinExpr::genCode() {
  auto leftV = left->genCode();
  auto rightV = right->genCode();

  return createSub(leftV, rightV);
}

llvm::Value *MulBinExpr::genCode() {
  auto leftV = left->genCode();
  auto rightV = right->genCode();

  return createMul(leftV, rightV);
}

llvm::Value *DivBinExpr::genCode() {
  auto leftV = left->genCode();
  auto rightV = right->genCode();

  return createDiv(leftV, rightV);
}

llvm::Value *FunctionExpr::genCode() {
  // asdf
  return nullptr;
}
