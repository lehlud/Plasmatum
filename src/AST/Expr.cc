#include "Expr.h"

#include "Stmt.h"

#include <iostream>

void IdExpr::print() {
    std::cout << "IdExpr: \'" << id << "\'";
}

void NumExpr::print() {
    std::cout << "NumExpr: " << value;
}

void StringExpr::print() {
    std::cout << "StringExpr: \'" << value << "\'";
}

BinExpr::~BinExpr() {
    delete left;
    delete right;
}

void BinExpr::print() {
    std::cout << "BinExpr: { op: " << op << ", left: ";
    left->print();
    std::cout << ", right: ";
    right->print();
    std::cout << " }";
}

BlockExpr::~BlockExpr() {
    for (auto stmt : stmts) delete stmt;
    delete result;
}

void BlockExpr::print() {
    std::cout << "BlockExpr: ";
}

FunctionExpr::~FunctionExpr() {
    delete result;
}

void FunctionExpr::print() {
    std::cout << "FunctionExpr: { ";
    std::cout << "args: [";
    for (auto arg : args) {
        std::cout << "{ id: '" << arg.first << "', type: ";
        if (arg.second.length() > 0) std::cout << "'" << arg.second << "'";
        else std::cout << "none";
        std::cout << " }";
        
        if (arg != args.back()) std::cout << ", ";
    }
    std::cout << "], ";
    std::cout << "result: ";
    result->print();
    std::cout << " }";
}


