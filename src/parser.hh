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

  Expr *parseExpr();


  std::string parseIdentifier();

  Expr *parseCallExpr(const std::string &id);
  Expr *parseVarExpr();
  Expr *parseNumber();
  Expr *parseString();
  Expr *parseDefine();

  Stmt *parseStmt();

};
