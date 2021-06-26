#include "defines.hh"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

llvm::LLVMContext Context;
llvm::Module Module("", Context);
llvm::IRBuilder<> Builder(Context);

llvm::Type *IntType = llvm::IntegerType::get(Context, INT_SIZE);
llvm::Type *CharType = llvm::IntegerType::get(Context, CHAR_SIZE);
llvm::Type *FloatType = (llvm::Type *)llvm::Type::getDoubleTy(Context);
llvm::Type *PlsmType = (llvm::Type *)nullptr;

int isMap = 0, isList = 0, isReference = 0;
