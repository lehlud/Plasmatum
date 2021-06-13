#include "ast.h"

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

    if (var.first->isFunctionTy()) return var.second;

    return context.builder->CreateLoad(var.first, var.second);
}

llvm::Value *BinExpr::genCode(Compiler::Context &context) {
    return nullptr;
}

llvm::Value *IfExpr::genCode(Compiler::Context &context) {
    return nullptr;
}

llvm::Value *BlockExpr::genCode(Compiler::Context &context) {
    return nullptr;
}

llvm::Value *ForExpr::genCode(Compiler::Context &context) {
    return nullptr;
}

llvm::Value *WhileExpr::genCode(Compiler::Context &context) {
    return nullptr;
}

llvm::Value *AssignExpr::genCode(Compiler::Context &context) {
    return nullptr;
}

llvm::Value *CallExpr::genCode(Compiler::Context &context) {
    return nullptr;
}

llvm::Value *LambdaExpr::genCode(Compiler::Context &context) {
    return nullptr;
}

llvm::Value *Function::genCode(Compiler::Context &context) {
    return nullptr;
}
