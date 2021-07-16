#include "ast.hh"
#include "context.hh"

llvm::Value *IntExpr::genCode(PlsmContext &context) {
  return context.getPlsmInt(value);
}

llvm::Value *FloatExpr::genCode(PlsmContext &context) {
  return context.getPlsmFloat(value);
}

llvm::Value *StringExpr::genCode(PlsmContext &context) {
  return context.getPlsmString(value);
}

llvm::Value *AddBinExpr::genCode(PlsmContext &context) {
  auto leftV = left->genCode(context);
  auto rightV = right->genCode(context);

  return nullptr;
}

llvm::Value *SubBinExpr::genCode(PlsmContext &context) {
  auto leftV = left->genCode(context);
  auto rightV = right->genCode(context);

  return nullptr;
}

llvm::Value *MulBinExpr::genCode(PlsmContext &context) {
  auto leftV = left->genCode(context);
  auto rightV = right->genCode(context);

  return nullptr;
}

llvm::Value *DivBinExpr::genCode(PlsmContext &context) {
  auto leftV = left->genCode(context);
  auto rightV = right->genCode(context);

  return nullptr;
}

llvm::Value *IfExpr::genCode(PlsmContext &context) {
  return context.createPlsmIf(cond, trueExpr, falseExpr);
}

llvm::Value *CallExpr::genCode(PlsmContext &context) {
  return context.createPlsmCall(callee, args);
}

llvm::Value *ExprStmt::genCode(PlsmContext &context) {
  return expr->genCode(context);
}

llvm::Value *ReturnStmt::genCode(PlsmContext &context) {
  return context.createRet(value->genCode(context));
}

llvm::Value *FunctionStmt::genCode(PlsmContext &context) {
  return context.createPlsmFunction(id, body);
}
