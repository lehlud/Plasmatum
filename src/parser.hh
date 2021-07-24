#pragma once

#include <string>

#include "ast.hh"

class Parser {
private:
  size_t index = 0;
  std::u32string text;

public:
  Parser(const std::u32string &text) : text(text) {}

  void skipSpaces();
  void forward();

  std::string parseIdentifier();

  Expr *parseExpr();
  Expr *parseOptionalBinExpr(Expr *expr, uint8_t prec = 0);

  Stmt *parseStmt();

};
