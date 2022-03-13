#pragma once

#include <string>
#include <vector>

#include <llvm/IR/Value.h>

class Expr;
class Context;

class Stmt {
public:
    virtual ~Stmt() = default;

    virtual void print() = 0;
    virtual llvm::Value *codegen(Context *context) = 0;
};

class ExprStmt : public Stmt {
private:
    Expr *expr;

public:
    ExprStmt(Expr *expr) : expr(expr) {}
    ~ExprStmt();

    void print() override;
    llvm::Value *codegen(Context *context) override;
};

class BlockStmt : public Stmt {
private:
    std::vector<Stmt *> stmts;

public:
    BlockStmt(const std::vector<Stmt *> &stmts) : stmts(stmts) {}
    ~BlockStmt();

    void print() override;
    llvm::Value *codegen(Context *context) override;
};

class DefineStmt : public Stmt {
private:
    std::string id;
    Expr *expr;
    
public:
    DefineStmt(const std::string &id, Expr *expr) : id(id), expr(expr) {}
    ~DefineStmt();

    void print() override;
    llvm::Value *codegen(Context *context) override;
};
