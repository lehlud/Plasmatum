#pragma once

#include <string>

#include "ast.hh"

class Parser {
private:
  size_t index = 0;
  std::u32string text;

public:
  Parser(const std::u32string &text) : text(text) {}

  Expr *parseExpr();
  Expr *parseNumber();
  Expr *parseString();
  Expr *parseIdentifier();

  Stmt *parseStmt();

};
