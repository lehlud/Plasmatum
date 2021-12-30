#pragma once

#include <llvm/IR/Value.h>

class Context;

#define _NATIVE(name)                                                          \
  llvm::Value *native##name(Context &context, llvm::Value *lhs,                \
                            llvm::Value *rhs)

_NATIVE(Add);
_NATIVE(Sub);
_NATIVE(Mul);
_NATIVE(Div);
