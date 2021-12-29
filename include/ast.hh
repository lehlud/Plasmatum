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

class StructType : public Type {
public:
  std::string name;
  std::vector<Type *> fields;
  StructType(std::string name, std::vector<Type *> fields);
  ~StructType() override;
  _OVERRIDE_LLVM_TYPE
};

#define _VIRTUAL_GEN_CODE virtual llvm::Value *genCode(Context &) = 0;
#define _OVERRIDE_GEN_CODE llvm::Value *genCode(Context &) override;
class Expr {
public:
  virtual ~Expr() = default;
  _VIRTUAL_GEN_CODE
};

class NumExpr : public Expr {
public:
  virtual ~NumExpr() = default;
};

class Integer : public NumExpr {
public:
  int64_t value;
  Integer(int64_t value) : value(value) {}
  _OVERRIDE_GEN_CODE
};

class Fraction : public NumExpr {
public:
  NumExpr *num, *denom;
  Fraction(NumExpr *num, NumExpr *denom) : num(num), denom(denom) {}
  ~Fraction() override;
  _OVERRIDE_GEN_CODE
};

#define _FARG_TYPE std::pair<std::string, Type *>
class Function : public Expr {
public:
  std::string name;
  std::vector<_FARG_TYPE> args;
  Function(std::string name, std::vector<_FARG_TYPE> args)
      : name(name), args(args) {}
  ~Function() override;
  _OVERRIDE_GEN_CODE
};

class FunctionCall : public Expr {
public:
  std::string calleeName;
  std::vector<Expr *> args;
  FunctionCall(std::string calleeName, std::vector<Expr *> args)
      : calleeName(calleeName), args(args) {}
  ~FunctionCall() override;
  _OVERRIDE_GEN_CODE
};

class Assignment : public Expr {
public:
  std::string name;
  Expr *value;
  Assignment(std::string name, Expr *value) : name(name), value(value) {}
  ~Assignment() override;
  _OVERRIDE_GEN_CODE
};

class Stmt {
public:
  virtual ~Stmt() {}
  _VIRTUAL_GEN_CODE
};
