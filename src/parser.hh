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
  Expr *parseIfExpr();
  Expr *parseOptionalBinExpr(Expr *expr);

  Stmt *parseStmt();
  Stmt *parseDefine();
  Stmt *parseReturn();
  Stmt *parseExprStmt();

};
