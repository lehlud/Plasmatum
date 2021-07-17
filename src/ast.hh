#pragma once

#include <stdint.h>

#include <string>
#include <vector>

#include <llvm/IR/Value.h>

class PlsmContext;

class Expr {
public:
  virtual llvm::Value *genCode(PlsmContext &context) = 0;
};

class Stmt {
public:
  virtual llvm::Value *genCode(PlsmContext &context) = 0;

  virtual bool isReturnStmt() { return false; }
};

class IntExpr : public Expr {
public:
  const uint64_t value;

  IntExpr(uint64_t value) : value(value) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class FloatExpr : public Expr {
public:
  const double_t value;

  FloatExpr(double_t value) : value(value) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class StringExpr : public Expr {
private:
  std::u32string value;

public:
  StringExpr(const std::u32string &value) : value(value) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class BinExpr : public Expr {
protected:
  Expr *left, *right;

public:
  BinExpr(Expr *left, Expr *right) : left(left), right(right) {}
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

class IfExpr : public Expr {
private:
  Expr *cond, *trueExpr, *falseExpr;

public:
  IfExpr(Expr *cond, Expr *trueExpr, Expr *falseExpr)
      : cond(cond), trueExpr(trueExpr), falseExpr(falseExpr) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class CallExpr : public Expr {
private:
  std::string callee;
  std::vector<Expr *> args;

public:
  CallExpr(const std::string &callee, const std::vector<Expr *> &args)
      : callee(callee), args(args) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class ExprStmt : public Stmt {
private:
  Expr *expr;

public:
  ExprStmt(Expr *expr) : expr(expr) {}

  llvm::Value *genCode(PlsmContext &context) override;
};

class ReturnStmt : public Stmt {
private:
  Expr *value;

public:
  ReturnStmt(Expr *value) : value(value) {}

  llvm::Value *genCode(PlsmContext &context) override;
  
  bool isReturnStmt() override { return true; }
};

class FunctionStmt : public Stmt {
private:
  std::string id;
  std::vector<Stmt *> body;
  std::vector<std::string> args;

public:
  FunctionStmt(const std::string &id, const std::vector<Stmt *> &body,
               const std::vector<std::string> &args)
      : id(id), body(body), args(args) {}

  llvm::Value *genCode(PlsmContext &context) override;
};
