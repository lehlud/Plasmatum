#pragma once

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

#include <string>

llvm::Value *NullValue();

llvm::Value *createIntCast(llvm::Value *v);
llvm::Value *createFloatCast(llvm::Value *v);

typedef llvm::Value *(*BinExprCreator)(llvm::Value *left, llvm::Value *right);

llvm::Value *createBinExpr(llvm::Value *left, llvm::Value *right,
                           BinExprCreator intBEC, BinExprCreator floatBEC,
                           const std::string &name = "binary expression");

llvm::Value *createAdd(llvm::Value *left, llvm::Value *right);
llvm::Value *createSub(llvm::Value *left, llvm::Value *right);
llvm::Value *createMul(llvm::Value *left, llvm::Value *right);
llvm::Value *createDiv(llvm::Value *left, llvm::Value *right);
llvm::Value *createMod(llvm::Value *left, llvm::Value *right);
