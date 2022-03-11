#pragma once

#include "Stmt.h"

#include <string>
#include <vector>

class Expr {
public:
    virtual ~Expr() = default;
    virtual void print() = 0;
};

class IdExpr : public Expr {
private:
    std::string id;

public:
    IdExpr(const std::string &id) : id(id) {}

    void print();
};

class NumExpr : public Expr {
private:
    double value;

public:
    NumExpr(double value) : value(value) {}

    void print();
};

class StringExpr : public Expr {
private:
    std::string value;

public:
    StringExpr(const std::string &value) : value(value) {}

    static StringExpr *fromWithQuotes(const std::string &value) {
        return new StringExpr(value.substr(1, value.size() - 2));
    }

    void print();
};

class BinExpr : public Expr {
public:
    enum Op { ADD, SUB, MUL, DIV, MOD };

private:
    Op op;
    Expr *left;
    Expr *right;

public:
    BinExpr(Op op, Expr *left, Expr *right) : op(op), left(left), right(right) {}
    ~BinExpr();

    void print();
};

class BlockExpr : public Expr {
private:
    std::vector<Stmt *> stmts;
    Expr *result;

public:
    BlockExpr(const std::vector<Stmt *> &stmts, Expr *result) 
        : stmts(stmts), result(result) {}
    ~BlockExpr();

    void print();
};

class FunctionExpr : public Expr {
private:
    std::vector<std::string> args;
    Expr *result;

public:
    FunctionExpr(const std::vector<std::string> &args, Expr *result)
        : args(args), result(result) {}
    ~FunctionExpr();

    void print();
};