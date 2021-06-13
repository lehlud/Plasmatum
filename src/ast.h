#pragma once

#include <map>
#include <string>
#include <vector>

#include <llvm/IR/Value.h>

#include "jit.h"

namespace Plasmatum {
namespace AST {

class Expr {
public:
  virtual ~Expr() = default;
  virtual llvm::Value *genCode(Compiler::Context &context) = 0;
};

class IntExpr : public Expr {
private:
  long val;

public:
  IntExpr(long val) : val(val) {}
  llvm::Value *genCode(Compiler::Context &context) override;
};

class FPExpr : public Expr {
private:
  double val;

public:
  FPExpr(double val) : val(val) {}
  llvm::Value *genCode(Compiler::Context &context) override;
};

class IdExpr : public Expr {
private:
  std::string id;

public:
  IdExpr(const std::string& id) : id(id) {}
  std::string getId() { return id; }
  llvm::Value *genCode(Compiler::Context &context) override;
};

class BinExpr : public Expr {
public:
  enum Type {
    ERR, // error type
    ADD, // '+'
    SUB, // '-'
    MUL, // '*'
    DIV, // '/'
    MOD, // '%'
    POW, // '**'
    // bitwise operators
    OR,  // '|'
    AND, // '&'
    // logical operators
    LOR,  // 'or'
    LAND, // 'and'
    EQ,   // '=='
    NEQ,  // '!='
    GT,   // '>'
    LT,   // '<'
    GTEQ, // '>='
    LTEQ, // '<='
  };

private:
  Type type;
  Expr *left, *right;

public:
  BinExpr(Type type, Expr *left, Expr *right)
      : type(type), left(left), right(right) {}
  llvm::Value *genCode(Compiler::Context &context) override;
};

extern std::map<BinExpr::Type, uint8_t> BinOpPrecedence;

class IfExpr : public Expr {
private:
  Expr *cond, *exprTrue, *exprFalse;

public:
  IfExpr(Expr *cond, Expr *exprTrue, Expr *exprFalse)
      : cond(cond), exprTrue(exprTrue), exprFalse(exprFalse) {}
  llvm::Value *genCode(Compiler::Context &context) override;
};

class BlockExpr : public Expr {
private:
  Expr *result;
  std::vector<Expr *> body;

public:
  BlockExpr(std::vector<Expr *> body, Expr *result)
      : body(body), result(result) {}
  llvm::Value *genCode(Compiler::Context &context) override;
};

class ForExpr : public Expr {
private:
  Expr *min, *max;
  std::string counterId;
  Expr *body;

public:
  ForExpr(Expr *min, Expr *max, IdExpr *counter, Expr *body)
      : min(min), max(max), counterId(counter->getId()), body(body) {}
  llvm::Value *genCode(Compiler::Context &context) override;
};

class WhileExpr : public Expr {
private:
  Expr *cond;
  Expr *ifBody, *elseBody;

public:
  WhileExpr(Expr *cond, Expr *ifBody, Expr *elseBody)
      : cond(cond), ifBody(ifBody), elseBody(elseBody ? elseBody : 0) {}
  llvm::Value *genCode(Compiler::Context &context) override;
};

class AssignExpr : public Expr {
private:
  std::string id;
  Expr *val;

public:
  AssignExpr(IdExpr *varExpr, Expr *val) : id(varExpr->getId()), val(val) {}
  llvm::Value *genCode(Compiler::Context &context) override;
};

class CallExpr : public Expr {
private:
  std::string id;
  std::vector<Expr *> *args;

public:
  CallExpr(IdExpr *varExpr, std::vector<Expr *> *args)
      : id(varExpr->getId()), args(args) {}
  llvm::Value *genCode(Compiler::Context &context) override;
};

class LambdaExpr : public Expr {
private:
  Expr *body;
  std::vector<std::string> args;

public:
  LambdaExpr(const std::vector<std::string> &args, Expr *body)
      : args(args), body(body) {}
  llvm::Value *genCode(Compiler::Context &context) override;
};

class Function : public Expr {
private:
  LambdaExpr *base;
  std::string id;

public:
  Function(LambdaExpr *base, const std::string &id) : base(base), id(id) {}
  llvm::Value *genCode(Compiler::Context &context) override;
};

} // namespace AST
} // namespace Plasmatum
