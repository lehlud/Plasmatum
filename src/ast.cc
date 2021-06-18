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

  if (var->isFunctionValue())
    return ((Compiler::FunctionValue *)var)->lambda->genCode(context);

  auto tmpVar = (Compiler::LLVMValue *)var;

  return context.builder->CreateLoad(tmpVar->type, tmpVar->value);
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

llvm::Value *BlockExpr::genCode(Compiler::Context &context) {
  for (auto &expr : body) {
    expr->genCode(context);
  }

  return result->genCode(context);
}

llvm::Value *ForExpr::genCode(Compiler::Context &context) { return nullptr; }

llvm::Value *WhileExpr::genCode(Compiler::Context &context) { return nullptr; }

llvm::Value *AssignExpr::genCode(Compiler::Context &context) { return nullptr; }

llvm::CallInst *CallExpr::genCode(Compiler::Context &context) {
  LambdaExpr *function = nullptr;

  if (Utils::instanceof <IdExpr>(callee)) {
    auto id = ((IdExpr *)callee)->getId();
    auto var = context.getVar(id);
    if (!var) {
      Error::compiler("undefined reference to '" + id + "'");
    } else if (var->isFunctionValue()) {
      auto funVal = ((Compiler::FunctionValue *)var);
      auto function = funVal->lambda;
    } else if (var->isLLVMValue()) {
      auto tmpVar = ((Compiler::LLVMValue *) var);
      if (tmpVar->type->isFunctionTy()) {
        auto f = (llvm::Function *) tmpVar->value;
        
        if (f->arg_size() != args.size()) {
          Error::compiler("invalid argument size");
        }

        std::vector<llvm::Value *> callArgs;
        for (int i = 0; i < args.size(); i++) {
          auto expr = args[i]->genCode(context);
          expr = Utils::tryCast(context, expr, f->getArg(i)->getType());
          callArgs.push_back(expr);
        }

        return context.builder->CreateCall(f, callArgs);
      }
    }
  } else if (Utils::instanceof <LambdaExpr>(callee)) {
    function = (LambdaExpr *) callee;
  }

  if (!function) {
    Error::compiler("invalid call instruction");
  } else if (function->args.size() != args.size()) {
    Error::compiler("invalid argument size");
  }

  auto prevArgTypes = context.argTypes;
  auto argTypes = new std::vector<llvm::Type *>();
  std::vector<llvm::Value *> callArgs;

  for (auto &arg : args) {
    auto expr = arg->genCode(context);
    argTypes->push_back(expr->getType());
    callArgs.push_back(expr);
  }

  context.argTypes = argTypes;

  auto f = function->genCode(context);

  context.argTypes = prevArgTypes;

  return context.builder->CreateCall(f, callArgs);
}

llvm::Function *LambdaExpr::genCode(Compiler::Context &context) {
  if (!context.argTypes) {
    Error::compiler("invalid types for function arguments");
  }

  return nullptr;
}

llvm::Function *Function::genCode(Compiler::Context &context) {
  if (context.getVar(id)) {
    // TODO: implement error message
  }

  context.setVar(id, new Compiler::FunctionValue(base));

  return base->genCode(context);
}
