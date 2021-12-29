#include "ast.hh"
#include "compiler.hh"

#include <iostream>
#include <vector>

int main() {
  std::vector<Stmt *> stmts;
  auto f = new Function({{"x", new NumberType()}, {"y", new NumberType()}},
                        new BinExpr(_OP_MUL, new VarRef("x"), new VarRef("y")));
  auto a = new Assignment("f", f);
  stmts.push_back(new ExprStmt(a));
  auto b = new CallExpr(new VarRef("f"), {new Number(10.5), new Number(4)});
  auto c = new CallExpr(new VarRef("__builtin__num_print"), {b});
  stmts.push_back(new ExprStmt(c));

  compile(stmts);

  return 0;
}
