#include "ast.hh"

namespace plsm {

std::u32string CallExpr::to_string() {
  std::u32string argsStr = U"{ ";
  if (args.size()) {
    for (size_t i = 0; i < args.size() - 1; i++) {
      argsStr += args[i]->to_string() + U", ";
    }
    argsStr += args.back()->to_string() + U" }";
  } else {
    argsStr += U"}";
  }

  return U"CallExpr: { callee: '" + to_u32(callee) + U"', args: " + argsStr +
         U" }";
}

std::u32string FunctionStmt::to_string() {
  std::u32string argsStr = U"{ ";
  if (args.size()) {
    for (size_t i = 0; i < args.size() - 1; i++) {
      argsStr += U"'" + to_u32(args[i]) + U"', ";
    }
    argsStr += U"'" + to_u32(args.back()) + U"' }";
  } else {
    argsStr += U"}";
  }

  std::u32string bodyStr = U"{ ";
  if (body.size()) {
    for (size_t i = 0; i < body.size() - 1; i++) {
      bodyStr += body[i]->to_string() + U", ";
    }
    bodyStr += body.back()->to_string() + U" }";
  } else {
    bodyStr += U"}";
  }

  return U"FunctionStmt: { args: " + argsStr + U", body: " + bodyStr + U" }";
}

Stmt *ExprStmt::optimize() {
  if (!(expr->isConstantExpr()))
    return this;
  else
    return nullptr;
}

Stmt *FunctionStmt::optimize() {
  std::vector<Stmt *> newBody;

  bool returned = false;
  for (size_t i = 0; i < body.size(); i++) {
    if (returned) {
      delete body[i];
    } else {
      if (body[i]->isReturning())
        returned = true;

      Stmt *optimized = body[i]->optimize();
      if (optimized)
        newBody.push_back(optimized);
    }
  }

  body.clear();
  body = newBody;

  if (!returned)
    body.push_back(new ReturnStmt(new NullExpr()));

  return this;
}

}
