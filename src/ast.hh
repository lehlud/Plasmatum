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
private:
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

class LambdaExpr : public Expr {
public:
  const std::vector<std::string> args;
  const std::vector<Stmt *> body;

  llvm::Value *genCode() override;
};

class DefineStmt : public Stmt {};

class DefineGlobalStmt : public DefineStmt {
public:
  const std::string id;

  DefineGlobalStmt(const std::string &id) : id(id) {}

  llvm::Value *genCode() override;
};

class DefineFunctionStmt : public DefineStmt {
public:
  const std::string id;
  const LambdaExpr base;

  llvm::Value *genCode() override;
};
