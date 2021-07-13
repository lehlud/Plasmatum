#pragma once

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

#include <string>

int64_t typeSize(llvm::Type *type);

llvm::Function *createMain();
llvm::Function *createAddFunction();


llvm::Value *plsmValue(int8_t type, llvm::Value *value);
llvm::Constant *plsmConstValue(int8_t type, llvm::Constant *value);

llvm::Constant *nullValue(int8_t type = 0);

llvm::Value *plsmMalloc(int64_t size, llvm::Type *PtrType);
