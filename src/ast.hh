#pragma once

#include <stdint.h>

#include <string>
#include <vector>

#include <llvm/IR/Value.h>

class Expr {
public:
  virtual llvm::Value *genCode() = 0;
};

class Stmt {
public:
  virtual llvm::Value *genCode() = 0;
};

class IntExpr : public Expr {
public:
  const uint64_t value;

  IntExpr(uint64_t value) : value(value) {}

  llvm::Value *genCode() override;
};

class FloatExpr : public Expr {
public:
  const double_t value;

  FloatExpr(double_t value) : value(value) {}

  llvm::Value *genCode() override;
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

  llvm::Value *genCode() override;
};

class SubBinExpr : public BinExpr {
public:
  SubBinExpr(Expr *left, Expr *right) : BinExpr(left, right) {}

  llvm::Value *genCode() override;
};

class MulBinExpr : public BinExpr {
public:
  MulBinExpr(Expr *left, Expr *right) : BinExpr(left, right) {}

  llvm::Value *genCode() override;
};

class DivBinExpr : public BinExpr {
public:
  DivBinExpr(Expr *left, Expr *right) : BinExpr(left, right) {}

  llvm::Value *genCode() override;
};

class Function {
public:
  std::vector<std::string> args;
  Expr *body;

  Function(const std::vector<std::string> &args, Expr *body)
      : args(args), body(body) {}

  llvm::Value *genCode();
};

class FunctionStmt : public Stmt {
private:
  std::string id;
  Function *function;

public:
  FunctionStmt(const std::string &id, Function *function)
      : id(id), function(function) {}

  llvm::Value *genCode() override;
};
