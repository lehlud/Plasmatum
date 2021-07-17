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

llvm::Value *VarExpr::genCode(PlsmContext &context) {
  return context.createVariableLoad(id);
}

llvm::Value *AddBinExpr::genCode(PlsmContext &context) {
  return context.createAdd(left->genCode(context), right->genCode(context));
}

llvm::Value *SubBinExpr::genCode(PlsmContext &context) {
  return context.createSub(left->genCode(context), right->genCode(context));
}

llvm::Value *MulBinExpr::genCode(PlsmContext &context) {
  return context.createMul(left->genCode(context), right->genCode(context));
}

llvm::Value *DivBinExpr::genCode(PlsmContext &context) {
  return context.createDiv(left->genCode(context), right->genCode(context));
}

llvm::Value *ModBinExpr::genCode(PlsmContext &context) {
  return context.createMod(left->genCode(context), right->genCode(context));
}

llvm::Value *EqBinExpr::genCode(PlsmContext &context) {
  return context.createEq(left->genCode(context), right->genCode(context));
}

llvm::Value *NEBinExpr::genCode(PlsmContext &context) {
  return context.createNE(left->genCode(context), right->genCode(context));
}

llvm::Value *GTBinExpr::genCode(PlsmContext &context) {
  return context.createGT(left->genCode(context), right->genCode(context));
}

llvm::Value *LTBinExpr::genCode(PlsmContext &context) {
  return context.createLT(left->genCode(context), right->genCode(context));
}

llvm::Value *GEBinExpr::genCode(PlsmContext &context) {
  return context.createGE(left->genCode(context), right->genCode(context));
}

llvm::Value *LEBinExpr::genCode(PlsmContext &context) {
  return context.createLE(left->genCode(context), right->genCode(context));
}

llvm::Value *IfExpr::genCode(PlsmContext &context) {
  return context.createPlsmConditional(cond, trueExpr, falseExpr);
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
  return context.createPlsmFunction(id, args, body);
}
