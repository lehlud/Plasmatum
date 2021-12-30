#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

#include "context.hh"

#define _VIRTUAL_LLVM_TYPE virtual llvm::Type *llvmType(Context &) = 0;
#define _OVERRIDE_LLVM_TYPE llvm::Type *llvmType(Context &) override;
class Type {
public:
  virtual ~Type() = default;
  _VIRTUAL_LLVM_TYPE
};

class IntegerType : public Type {
public:
  _OVERRIDE_LLVM_TYPE
};

class FractionType : public Type {
public:
  _OVERRIDE_LLVM_TYPE
};

class NumberType : public Type {
public:
  _OVERRIDE_LLVM_TYPE
};

class StructType : public Type {
public:
  std::string name;
  std::vector<Type *> fields;
  StructType(std::string name, std::vector<Type *> fields);
  ~StructType() override {
    for (auto field : fields) {
      delete field;
    }
  }
  _OVERRIDE_LLVM_TYPE
};

#define _VIRTUAL_GEN_CODE virtual llvm::Value *genCode(Context &) = 0;
#define _OVERRIDE_GEN_CODE llvm::Value *genCode(Context &) override;
class Expr {
public:
  virtual ~Expr() = default;
  _VIRTUAL_GEN_CODE

  virtual bool isFloat() { return false; }
  virtual bool isInteger() { return false; }
  virtual bool isVarRef() { return false; }
  virtual bool isBinExpr() { return false; }
  virtual bool isString() { return false; }
  virtual bool isFunction() { return false; }
  virtual bool isFunctionCall() { return false; }
  virtual bool isAssignment() { return false; }
};

class Float : public Expr {
public:
  __float128 value;
  Float(__float128 value) : value(value) {}
  _OVERRIDE_GEN_CODE
};

class Integer : public Expr {
public:
  __int128_t value;
  Integer(__int128_t value) : value(value) {}
  _OVERRIDE_GEN_CODE
  bool isInteger() override { return true; }
};

class VarRef : public Expr {
public:
  std::string name;
  VarRef(std::string name) : name(name) {}
  _OVERRIDE_GEN_CODE
  bool isVarRef() override { return true; }
};

#define _OP_ADD 0
#define _OP_SUB 1
#define _OP_MUL 2
#define _OP_DIV 3
class BinExpr : public Expr {
public:
  int8_t op;
  Expr *lhs, *rhs;
  BinExpr(int8_t op, Expr *lhs, Expr *rhs) : op(op), lhs(lhs), rhs(rhs) {}
  ~BinExpr() override {
    delete lhs;
    delete rhs;
  }
  _OVERRIDE_GEN_CODE
  bool isBinExpr() override { return true; }
};

class String : public Expr {
public:
  std::u32string value;
  String(std::u32string value) : value(value) {}
  _OVERRIDE_GEN_CODE
};

#define _FARG_TYPE std::pair<std::string, Type *>
class Function : public Expr {
public:
  std::vector<_FARG_TYPE> args;
  Expr *body;
  Function(std::vector<_FARG_TYPE> args, Expr *body) : args(args), body(body) {}
  ~Function() override {
    delete body;
    for (auto arg : args) {
      delete arg.second;
    }
  }
  _OVERRIDE_GEN_CODE
  virtual bool isFunction() override { return true; }
};

class CallExpr : public Expr {
public:
  Expr *callee;
  std::vector<Expr *> args;
  CallExpr(Expr *callee, std::vector<Expr *> args)
      : callee(callee), args(args) {}
  ~CallExpr() override {
    for (auto arg : args) {
      delete arg;
    }
  }
  _OVERRIDE_GEN_CODE
  virtual bool isFunctionCall() override { return true; }
};

class Assignment : public Expr {
public:
  std::string name;
  Expr *value;
  Assignment(std::string name, Expr *value) : name(name), value(value) {}
  ~Assignment() override { delete value; }
  _OVERRIDE_GEN_CODE
  virtual bool isAssignment() override { return true; }
};

class Stmt {
public:
  virtual ~Stmt() = default;
  _VIRTUAL_GEN_CODE
};

class ExprStmt : public Stmt {
public:
  Expr *expr;
  ExprStmt(Expr *expr) : expr(expr) {}
  ~ExprStmt() override { delete expr; }
  _OVERRIDE_GEN_CODE
};
