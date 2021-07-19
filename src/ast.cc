#include "ast.hh"
#include "context.hh"

std::u32string CallExpr::to_string() {
  std::u32string argsStr = U"{ ";
  if (args.size()) {
    for (size_t i = 0; i < args.size() - 1; i++) {
      argsStr += args[i]->to_string() + U", ";
    }
    argsStr += args.back()->to_string() + U" }";
  } else {
    argsStr += U"}";
  }

  return U"CallExpr: { callee: '" + to_u32(callee) + U"', args: " + argsStr +
         U" }";
}

std::u32string FunctionStmt::to_string() {
  std::u32string argsStr = U"{ ";
  if (args.size()) {
    for (size_t i = 0; i < args.size() - 1; i++) {
      argsStr += U"'" + to_u32(args[i]) + U"', ";
    }
    argsStr += U"'" + to_u32(args.back()) + U"' }";
  } else {
    argsStr += U"}";
  }

  std::u32string bodyStr = U"{ ";
  if (body.size()) {
    for (size_t i = 0; i < body.size() - 1; i++) {
      bodyStr += body[i]->to_string() + U", ";
    }
    bodyStr += body.back()->to_string() + U" }";
  } else {
    bodyStr += U"}";
  }

  return U"FunctionStmt: { args: " + argsStr + U", body: " + bodyStr + U" }";
}

Stmt *ExprStmt::optimize() {
  if (!(expr->isConstantExpr()))
    return this;
  else
    return nullptr;
}

Stmt *FunctionStmt::optimize() {
  std::vector<Stmt *> newBody;

  bool returned = false;
  for (size_t i = 0; i < body.size(); i++) {
    if (returned) {
      delete body[i];
    } else {
      if (body[i]->isReturning())
        returned = true;

      Stmt *optimized = body[i]->optimize();
      if (optimized)
        newBody.push_back(optimized);
    }
  }

  body.clear();
  body = newBody;

  if (!returned)
    body.push_back(new ReturnStmt(new NullExpr()));

  return this;
}

llvm::Value *NullExpr::genCode(PlsmContext &context) {
  return context.getPlsmNull();
}

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
