#include "natives.hh"

#include "context.hh"

_NATIVE(Add) {
  return context.builder.CreateAdd(lhs, rhs);
}

_NATIVE(Sub) {
  return context.builder.CreateSub(lhs, rhs);
}

_NATIVE(Mul) {
  return context.builder.CreateMul(lhs, rhs);
}

_NATIVE(Div) {
  return context.builder.CreateSDiv(lhs, rhs);
}
