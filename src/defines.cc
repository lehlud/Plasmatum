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
llvm::Type *FloatType = llvm::Type::getDoubleTy(Context);

llvm::Type *TypeType = llvm::Type::getInt8Ty(Context);
llvm::Type *ValueType = llvm::Type::getInt8PtrTy(Context);
llvm::Type *PlsmType = llvm::StructType::get(Context, {TypeType, ValueType});

int isMap = 0, isList = 0, isReference = 0;

llvm::Function *AddFunction;
llvm::Function *SubFunction;
llvm::Function *MulFunction;
llvm::Function *DivFunction;

