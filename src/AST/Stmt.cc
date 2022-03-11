#include "Stmt.h"

#include "Expr.h"

#include <iostream>

ExprStmt::~ExprStmt() {
    delete expr;
}

void ExprStmt::print() {
    std::cout << "ExprStmt: ";
    expr->print();
}

BlockStmt::~BlockStmt() {
    for (auto stmt : stmts) delete stmt;
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

DefineStmt::~DefineStmt() {
    delete expr;
}

void DefineStmt::print() {
    std::cout << "DefineStmt: '" << id << "\' as ";
    expr->print();
}