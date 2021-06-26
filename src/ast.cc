#include "ast.hh"

#include <llvm/IR/Type.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>

llvm::Value *IntExpr::genCode() {
    extern llvm::Type *IntType;
    return llvm::ConstantInt::get(IntType, value);
}

llvm::Value *FloatExpr::genCode() {
    extern llvm::Type *FloatType;
    return llvm::ConstantFP::get(FloatType, value);
}

llvm::Value *AddBinExpr::genCode() {
    return 0;
}

llvm::Value *SubBinExpr::genCode() {
    return 0;
}

llvm::Value *MulBinExpr::genCode() {
    return 0;
}

llvm::Value *DivBinExpr::genCode() {
    return 0;
}
