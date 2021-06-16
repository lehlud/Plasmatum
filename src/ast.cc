#include "ast.h"
#include "utils.h"

using namespace Plasmatum::AST;

llvm::Value *IntExpr::genCode(Compiler::Context &context) {
  auto type = llvm::Type::getInt64Ty(context.mod->getContext());
  return llvm::ConstantInt::get(type, val);
}

llvm::Value *FPExpr::genCode(Compiler::Context &context) {
  auto type = llvm::Type::getDoubleTy(context.mod->getContext());
  return llvm::ConstantFP::get(type, val);
}

llvm::Value *IdExpr::genCode(Compiler::Context &context) {
  auto var = context.getVar(id);

  if (var.first->isFunctionTy())
    return var.second;

  return context.builder->CreateLoad(var.first, var.second);
}

llvm::Value *BinExpr::genCode(Compiler::Context &context) { return nullptr; }

llvm::Value *IfExpr::genCode(Compiler::Context &context) {
  auto condV = cond->genCode(context);
  condV = Utils::tryLogicalVal(context, condV);

  if (!condV) {
    Error::compiler("invalid condition type in conditional expression");
  }

  auto f = context.builder->GetInsertBlock()->getParent();

  auto ifBB = llvm::BasicBlock::Create(context.mod->getContext(), "", f);
  auto elseBB = llvm::BasicBlock::Create(context.mod->getContext(), "", f);
  auto mergeBB = llvm::BasicBlock::Create(context.mod->getContext(), "", f);

  context.builder->CreateCondBr(condV, ifBB, elseBB);

  context.builder->SetInsertPoint(ifBB);
  auto trueV = exprTrue->genCode(context);

  context.builder->CreateBr(mergeBB);
  ifBB = context.builder->GetInsertBlock();

  context.builder->SetInsertPoint(elseBB);
  auto falseV = exprFalse->genCode(context);
  falseV = Utils::tryCast(context, falseV, trueV->getType());

  if (!falseV) {
    Error::compiler("invalid operand types in conditional expression");
  }

  context.builder->CreateBr(mergeBB);
  elseBB = context.builder->GetInsertBlock();

  context.builder->SetInsertPoint(mergeBB);

  auto phiNode = context.builder->CreatePHI(trueV->getType(), 2);

  phiNode->addIncoming(trueV, ifBB);
  phiNode->addIncoming(falseV, elseBB);

  return phiNode;
}

llvm::Value *BlockExpr::genCode(Compiler::Context &context) { return nullptr; }

llvm::Value *ForExpr::genCode(Compiler::Context &context) { return nullptr; }

llvm::Value *WhileExpr::genCode(Compiler::Context &context) { return nullptr; }

llvm::Value *AssignExpr::genCode(Compiler::Context &context) { return nullptr; }

llvm::Value *CallExpr::genCode(Compiler::Context &context) { return nullptr; }

llvm::Value *LambdaExpr::genCode(Compiler::Context &context) { return nullptr; }

llvm::Value *Function::genCode(Compiler::Context &context) { return nullptr; }
