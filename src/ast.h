#pragma once

#include <map>
#include <string>
#include <vector>

namespace Plasmatum {
namespace AST {

class Expr {
public:
  virtual ~Expr() = default;
};

class IntExpr : public Expr {
private:
  long val;

public:
  IntExpr(long val) : val(val) {}
};

class FPExpr : public Expr {
private:
  double val;

public:
  FPExpr(double val) : val(val) {}
};

class IdExpr : public Expr {
private:
  std::string id;

public:
  IdExpr(std::string id) : id(id) {}
  std::string getId() { return id; }
};

class BinExpr : public Expr {
public:
  enum Type {
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
};

class CallExpr : public Expr {
private:
  std::string id;
  std::vector<Expr *> *args;

public:
  CallExpr(IdExpr *varExpr, std::vector<Expr *> *args)
      : id(varExpr->getId()), args(args) {}
};

class IfExpr : public Expr {
private:
  Expr *cond, *exprTrue, *exprFalse;

public:
  IfExpr(Expr *cond, Expr *exprTrue, Expr *exprFalse)
      : cond(cond), exprTrue(exprTrue), exprFalse(exprFalse) {}
};

class BlockExpr : public Expr {
private:
  Expr *result;
  std::vector<Expr *> body;

public:
  BlockExpr(std::vector<Expr *> body, Expr *result)
      : body(body), result(result) {}
};

class ForExpr : public Expr {
private:
  Expr *min, *max;
  std::string counterId;
  Expr *body;

public:
  ForExpr(Expr *min, Expr *max, IdExpr *counter, Expr *body)
      : min(min), max(max), counterId(counter->getId()), body(body) {}
};

class WhileExpr : public Expr {
private:
  Expr *cond;
  Expr *ifBody, *elseBody;

public:
  WhileExpr(Expr *cond, Expr *ifBody, Expr *elseBody)
      : cond(cond), ifBody(ifBody), elseBody(elseBody ? elseBody : 0) {}
};

class FDefExpr : public Expr {
private:
  std::string id;
  std::vector<std::string> *args;
  Expr *body;

public:
  FDefExpr(IdExpr *varExpr, std::vector<IdExpr *> *args, Expr *body)
      : id(varExpr->getId()), body(body) {
    this->args = new std::vector<std::string>();
    for (unsigned long i = 0; i < args->size(); i++)
      this->args->push_back(args->at(i)->getId());
  }
};

class SDefExpr : public Expr {
private:
  std::string id;
  std::vector<std::string> *attrs;

public:
  SDefExpr(IdExpr *varExpr, std::vector<IdExpr *> *attrs)
      : id(varExpr->getId()) {
    this->attrs = new std::vector<std::string>();
    for (unsigned long i = 0; i < attrs->size(); i++)
      this->attrs->push_back(attrs->at(i)->getId());
  }
};

class DeclExpr : public Expr {
private:
  std::string id;
  Expr *val;

public:
  DeclExpr(IdExpr *varExpr, Expr *val) : id(varExpr->getId()), val(val) {}
};

class AssignExpr : public Expr {
private:
  std::string id;
  Expr *val;

public:
  AssignExpr(IdExpr *varExpr, Expr *val) : id(varExpr->getId()), val(val) {}
};

class UndefExpr : public Expr {
private:
  std::string id;

public:
  UndefExpr(IdExpr *varExpr) : id(varExpr->getId()) {}
};

class Lambda {
private:
  Expr *body;
  std::vector<const std::string&> args;

public:
  Lambda(const std::vector<const std::string&> &args, Expr *body)
      : args(args), body(body) {}
};

class Function {
private:
  Lambda *base;
  std::string id;

public:
  Function(Lambda *base, const std::string &id) : base(base), id(id) {}
};

} // namespace AST
} // namespace Plasmatum
