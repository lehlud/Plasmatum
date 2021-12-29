#include "ast.hh"

#include <llvm/IR/Verifier.h>

#include <iostream>

llvm::Type *NumberType::llvmType(Context &context) {
  return context.getNumberType();
}

llvm::Type *StructType::llvmType(Context &context) {
  std::vector<llvm::Type *> fieldTypes;
  for (auto field : fields) {
    fieldTypes.push_back(field->llvmType(context));
  }
  return llvm::StructType::create(fieldTypes);
}

llvm::Value *Number::genCode(Context &context) {
  return llvm::ConstantFP::get(context.getNumberType(), value);
}

llvm::Value *VarRef::genCode(Context &context) {
  if (context.namedValues.count(name) != 0) {
    auto var = context.namedValues[name];
    return context.builder.CreateLoad(var.first, var.second);
  } else if (context.builtins.count(name) != 0) {
    return context.builtins[name];
  }
  assert(0 && "Variable not defined");
}

llvm::Value *BinExpr::genCode(Context &context) {
  auto lhsV = lhs->genCode(context);
  auto rhsV = rhs->genCode(context);
  switch (op) {
  case _OP_ADD:
    return context.builder.CreateFAdd(lhsV, rhsV);
  case _OP_SUB:
    return context.builder.CreateFSub(lhsV, rhsV);
  case _OP_MUL:
    return context.builder.CreateFMul(lhsV, rhsV);
  case _OP_DIV:
    return context.builder.CreateFDiv(lhsV, rhsV);
  default:
    assert(0 && "Unknown binary operator");
  }
}

llvm::Value *String::genCode(Context &context) {
  auto charType = context.getCharType();
  std::vector<llvm::Constant *> chars;
  for (auto c : value) {
    chars.push_back(llvm::ConstantInt::get(charType, c));
  }
  auto arrayType = llvm::ArrayType::get(charType, chars.size());
  auto array = llvm::ConstantArray::get(arrayType, chars);

  auto i64Type = context.builder.getInt64Ty();
  auto zero = llvm::ConstantInt::get(i64Type, 0);
  return context.builder.CreateGEP(i64Type, array, {zero, zero});
}

llvm::Value *Function::genCode(Context &context) {
  auto previousNamed = context.namedValues;
  context.namedValues = _VAR_SCOPE_TYPE();

  auto previousBB = context.builder.GetInsertBlock();

  auto dummy = llvm::Function::Create(context.getDummyFunctionType(),
                                      llvm::Function::ExternalLinkage, "",
                                      context.module);
  auto firstBB = llvm::BasicBlock::Create(context.llvmContext, "", dummy);
  auto secondBB = llvm::BasicBlock::Create(context.llvmContext, "", dummy);
  auto thirdBB = llvm::BasicBlock::Create(context.llvmContext, "", dummy);
  context.builder.SetInsertPoint(firstBB);

  std::vector<llvm::Type *> paramTypes;
  std::vector<llvm::Value *> paramValues;
  for (auto arg : args) {
    auto type = arg.second->llvmType(context);
    paramTypes.push_back(type);
    auto alloca = context.builder.CreateAlloca(type);
    paramValues.push_back(alloca);
    context.namedValues[arg.first] = {type, alloca};
  }

  context.builder.CreateBr(secondBB);
  context.builder.SetInsertPoint(thirdBB);

  auto result = body->genCode(context);
  context.builder.CreateRet(result);

  auto function = llvm::Function::Create(
      llvm::FunctionType::get(result->getType(), paramTypes, false),
      llvm::Function::ExternalLinkage, "", context.module);

  context.builder.SetInsertPoint(secondBB);

  for (size_t i = 0; i < function->arg_size(); i++) {
    auto arg = function->getArg(i);
    context.builder.CreateStore(arg, paramValues[i]);
  }

  context.builder.CreateBr(thirdBB);

  function->getBasicBlockList().push_back(firstBB);
  function->getBasicBlockList().push_back(secondBB);
  function->getBasicBlockList().push_back(thirdBB);
  // secondBB->moveAfter(firstBB);
  // thirdBB->moveAfter(secondBB);

  dummy->removeFromParent();

  context.builder.SetInsertPoint(previousBB);
  context.namedValues = previousNamed;

  assert(!llvm::verifyFunction(*function));

  return function;
}

llvm::Value *CallExpr::genCode(Context &context) {
  auto calleeV = callee->genCode(context);
  auto type = calleeV->getType();
  assert(type->isPointerTy() && type->getPointerElementType()->isFunctionTy());

  auto functionType = (llvm::FunctionType *)type->getPointerElementType();

  auto function = llvm::FunctionCallee(functionType, calleeV);
  auto functionArgs = functionType->params();
  assert(functionArgs.size() == args.size());

  std::vector<llvm::Value *> argValues;
  for (size_t i = 0; i < args.size(); i++) {
    auto arg = args[i];
    auto argValue = arg->genCode(context);
    argValues.push_back(argValue);
  }

  return context.builder.CreateCall(function, argValues);
}

llvm::Value *Assignment::genCode(Context &context) {
  auto v = value->genCode(context);
  auto type = v->getType();
  llvm::Value *alloca = nullptr;
  if (context.namedValues.count(name) != 0) {
    auto pair = context.namedValues[name];
    assert(pair.first == type);
  } else {
    alloca = context.builder.CreateAlloca(type);
    context.namedValues[name] = {type, alloca};
  }
  context.builder.CreateStore(v, alloca);
  return v;
}

llvm::Value *ExprStmt::genCode(Context &context) {
  return expr->genCode(context);
}
