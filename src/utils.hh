#pragma once

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

#include <string>

llvm::Function *createMain();

llvm::Function *createAddFunction();


llvm::Value *plsmValue(int8_t type, llvm::Value *value);
llvm::Constant *plsmConstValue(int8_t type, llvm::Constant *value);

llvm::Constant *nullValue(int8_t type = 0);