#pragma once

#include <string>

#include "ast.h"

namespace Plasmatum {

enum TokenType {
  INT,   // integer literal
  FLOAT, // float literal
  ID,    // identifier
  PO,    // '('
  PC,    // ')'
  QM,    // '?'
  COL,   // ':'
  ASS,   // '='
  ADD,   // '+'
  SUB,   // '-'
  MUL,   // '*'
  DIV,   // '/'
  MOD,   // '%'
  ARR,   // '->'
};

typedef struct Token_t {
  TokenType type;
  std::string val;
} Token;

class Lexer {
private:
  size_t idx = 0;
  std::string text;

public:
  Lexer(const std::string &text) : text(text) {}
  Token next();
};

class Parser {
private:
  Lexer lexer;
  Token token;

public:
  Parser(const Lexer &lexer) : lexer(lexer) {}

  void next() { token = lexer.next(); }

  AST::Expr *parseExpr(bool topLevel = false);
};

} // namespace Plasmatum
