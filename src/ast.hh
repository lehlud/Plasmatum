#pragma once

#include <stdint.h>

#include <string>
#include <vector>

#include <llvm/IR/Value.h>

#include "utils.hh"

class PlsmContext;

class Expr {
public:
  virtual ~Expr() = default;

  virtual Expr *optimize() { return this; }

  virtual bool isCallExpr() { return false; }
  virtual bool isConstantExpr() { return false; }

  virtual std::u32string to_string() = 0;

  virtual llvm::Value *genCode(PlsmContext &context) = 0;
};

class Stmt {
public:
  virtual ~Stmt() = default;

  virtual Stmt *optimize() { return this; }

  virtual bool isReturning() { return false; }

  // virtual std::u32string to_string() = 0;

  virtual llvm::Value *genCode(PlsmContext &context) = 0;
};

class NullExpr : public Expr {
public:
  bool isConstantExpr() override { return true; }

  std::u32string to_string() override { return U"None"; }

  llvm::Value *genCode(PlsmContext &context) override;
};

class IntExpr : public Expr {
public:
  const uint64_t value;

  IntExpr(uint64_t value) : value(value) {}

  bool isConstantExpr() override { return true; }

  std::u32string to_string() override { return to_u32(std::to_string(value)); }

  llvm::Value *genCode(PlsmContext &context) override;
};

class FloatExpr : public Expr {
public:
  const double_t value;

  FloatExpr(double_t value) : value(value) {}

  bool isConstantExpr() override { return true; }

  std::u32string to_string() override { return to_u32(std::to_string(value)); }

  llvm::Value *genCode(PlsmContext &context) override;
};

class StringExpr : public Expr {
private:
  std::u32string value;

public:
  StringExpr(const std::u32string &value) : value(value) {}

  bool isConstantExpr() override { return true; }

  std::u32string to_string() override { return U"'" + value + U"'"; }

  llvm::Value *genCode(PlsmContext &context) override;
};

class VarExpr : public Expr {
private:
  std::string id;

public:
  VarExpr(const std::string &id) : id(id) {}

  std::u32string to_string() override { return U"var: '" + to_u32(id) + U"'"; }

  llvm::Value *genCode(PlsmContext &context) override;
};

class BinExpr : public Expr {
protected:
  Expr *left, *right;

public:
  BinExpr(Expr *left, Expr *right) : left(left), right(right) {}

  ~BinExpr() {
    delete left;
    delete right;
  }

  std::u32string to_string() override {
    return U"binexpr: { left: " + left->to_string() + U", right: " +
           right->to_string() + U" }";
  }

  bool isConstantExpr() override {
    return left->isConstantExpr() && right->isConstantExpr();
  }
};

class AddBinExpr : public BinExpr {
public:
  AddBinExpr(Expr *left, Expr *right) : BinExpr(left, right) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class SubBinExpr : public BinExpr {
public:
  SubBinExpr(Expr *left, Expr *right) : BinExpr(left, right) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class MulBinExpr : public BinExpr {
public:
  MulBinExpr(Expr *left, Expr *right) : BinExpr(left, right) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class DivBinExpr : public BinExpr {
public:
  DivBinExpr(Expr *left, Expr *right) : BinExpr(left, right) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class ModBinExpr : public BinExpr {
public:
  ModBinExpr(Expr *left, Expr *right) : BinExpr(left, right) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class EqBinExpr : public BinExpr {
public:
  EqBinExpr(Expr *left, Expr *right) : BinExpr(left, right) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class NEBinExpr : public BinExpr {
public:
  NEBinExpr(Expr *left, Expr *right) : BinExpr(left, right) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class GTBinExpr : public BinExpr {
public:
  GTBinExpr(Expr *left, Expr *right) : BinExpr(left, right) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class LTBinExpr : public BinExpr {
public:
  LTBinExpr(Expr *left, Expr *right) : BinExpr(left, right) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class GEBinExpr : public BinExpr {
public:
  GEBinExpr(Expr *left, Expr *right) : BinExpr(left, right) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class LEBinExpr : public BinExpr {
public:
  LEBinExpr(Expr *left, Expr *right) : BinExpr(left, right) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class IfExpr : public Expr {
private:
  Expr *cond, *trueExpr, *falseExpr;

public:
  IfExpr(Expr *cond, Expr *trueExpr, Expr *falseExpr)
      : cond(cond), trueExpr(trueExpr), falseExpr(falseExpr) {}

  ~IfExpr() {
    delete cond;
    delete trueExpr;
    delete falseExpr;
  }

  std::u32string to_string() override {
    return U"ifexpr: { cond: " + cond->to_string() + U", true: " +
           trueExpr->to_string() + U", false: " + falseExpr->to_string() +
           U" }";
  }

  llvm::Value *genCode(PlsmContext &context) override;
};

class CallExpr : public Expr {
private:
  std::string callee;
  std::vector<Expr *> args;

public:
  CallExpr(const std::string &callee, const std::vector<Expr *> &args)
      : callee(callee), args(args) {}

  ~CallExpr() {
    for (auto &expr : args) {
      delete expr;
    }
  }

  std::u32string to_string() override {
    return U"callexpr: '" + to_u32(callee) + U"'";
  }

  bool isCallExpr() override { return true; }

  llvm::Value *genCode(PlsmContext &context) override;
};

class ExprStmt : public Stmt {
private:
  Expr *expr;

public:
  ExprStmt(Expr *expr) : expr(expr) {}

  ~ExprStmt() { delete expr; }

  Stmt *optimize() override;

  llvm::Value *genCode(PlsmContext &context) override;
};

class ReturnStmt : public Stmt {
private:
  Expr *value;

public:
  ReturnStmt(Expr *value) : value(value) {}

  ~ReturnStmt() { delete value; }

  bool isReturning() override { return true; }

  llvm::Value *genCode(PlsmContext &context) override;
};

class FunctionStmt : public Stmt {
private:
  std::string id;
  std::vector<Stmt *> body;
  std::vector<std::string> args;

public:
  FunctionStmt(const std::string &id, const std::vector<Stmt *> &body,
               const std::vector<std::string> &args)
      : id(id), body(body), args(args) {
    optimize();
  }

  ~FunctionStmt() {
    for (auto &stmt : body) {
      delete stmt;
    }
  }

  Stmt *optimize() override;

  bool isReturning() override {
    for (auto &stmt : body) {
      if (stmt->isReturning())
        return true;
    }
    return false;
  }

  llvm::Value *genCode(PlsmContext &context) override;
};
