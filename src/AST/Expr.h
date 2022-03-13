#pragma once

#include <map>
#include <string>
#include <vector>

#include <llvm/IR/Value.h>

class Stmt;
class TypeRef;

class Context;

class Expr {
public:
    virtual ~Expr() = default;
    
    virtual void print() = 0;
    virtual llvm::Value *codegen(Context *context) = 0;
};

class IdExpr : public Expr {
private:
    std::string id;

public:
    IdExpr(const std::string &id) : id(id) {}

    void print() override;
    llvm::Value *codegen(Context *context) override;
};

class NumExpr : public Expr {
private:
    double value;

public:
    NumExpr(double value) : value(value) {}

    void print() override;
    llvm::Value *codegen(Context *context) override;
};

class StringExpr : public Expr {
private:
    std::string value;

public:
    StringExpr(const std::string &value) : value(value) {}

    static StringExpr *fromWithQuotes(const std::string &value) {
        return new StringExpr(value.substr(1, value.size() - 2));
    }

    void print() override;
    llvm::Value *codegen(Context *context) override;
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

    std::string opName() {
        switch (op) {
        case ADD:
            return "add";
        case SUB:
            return "sub";
        case MUL:
            return "mul";
        case DIV:
            return "div";
        case MOD:
            return "mod";
        }

        std::exit(1);
    }

    void print() override;
    llvm::Value *codegen(Context *context) override;
};

class CallExpr : public Expr {
private:
    Expr *callee;
    std::vector<Expr *> args;

public:
    CallExpr(Expr *callee, const std::vector<Expr *> &args) : callee(callee), args(args) {}
    ~CallExpr();

    void print() override;
    llvm::Value *codegen(Context *context) override;
};

class BlockExpr : public Expr {
private:
    std::vector<Stmt *> stmts;
    Expr *result;

public:
    BlockExpr(const std::vector<Stmt *> &stmts, Expr *result) 
        : stmts(stmts), result(result) {}
    ~BlockExpr();

    void print() override;
    llvm::Value *codegen(Context *context) override;
};

class FunctionExpr : public Expr {
private:
    std::vector<std::pair<std::string, TypeRef *>> args;
    Expr *result;

public:
    FunctionExpr(const std::vector<std::pair<std::string, TypeRef *>> &args, Expr *result)
        : args(args), result(result) {}
    ~FunctionExpr();

    void print() override;
    llvm::Value *codegen(Context *context) override;
};