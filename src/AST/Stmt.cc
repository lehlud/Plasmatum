#include "Stmt.h"

#include "Expr.h"
#include "../Context.h"

#include <iostream>

ExprStmt::~ExprStmt() {
    delete expr;
}

BlockStmt::~BlockStmt() {
    for (auto stmt : stmts) delete stmt;
}

DefineStmt::~DefineStmt() {
    delete expr;
}

void ExprStmt::print() {
    std::cout << "ExprStmt: ";
    expr->print();
}

void BlockStmt::print() {
    std::cout << "BlockStmt: {" << std::endl;
    for (auto stmt : stmts) {
        std::cout << "  ";
        stmt->print();
        std::cout << "," << std::endl;
    }
    std::cout << "}";
}

void DefineStmt::print() {
    std::cout << "DefineStmt: '" << id << "\' as ";
    expr->print();
}

llvm::Value *ExprStmt::codegen(Context *context) {
    return expr->codegen(context);
}

llvm::Value *BlockStmt::codegen(Context *context) {
    context->initNewValueScope();
    for (auto stmt : stmts) {
        stmt->codegen(context);
    }
    context->disposeLastValueScope();

    return nullptr;
}

llvm::Value *DefineStmt::codegen(Context *context) {
    auto value = expr->codegen(context);
    auto global = new llvm::GlobalVariable(value->getType(), false, llvm::GlobalValue::InternalLinkage);

    context->builder.CreateStore(value, global);

    auto storedValue = new StoredValue(value->getType(), global);
    context->setValue(id, storedValue);

    return nullptr;
}
