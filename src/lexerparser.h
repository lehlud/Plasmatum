#pragma once

#include <string>

#include "ast.h"

namespace Plasmatum {

class Lexer {
private:
  size_t index = 0;
  std::string text;

public:
  typedef struct Token_t {
    enum Type {
#undef EOF
      EOF,   // end of file
      INT,   // integer literal
      FLOAT, // float literal
      ID,    // identifier
      PO,    // '('
      PC,    // ')'
      QM,    // '?'
      COL,   // ':'
      ASS,   // '='
      ARR,   // '->'
      EQ,    // '=='
      NE,    // '!='
      LT,    // '<'
      GT,    // '>'
      LE,    // '<='
      GE,    // '>='
      ADD,   // '+'
      SUB,   // '-'
      MUL,   // '*'
      DIV,   // '/'
      MOD,   // '%'
      POW,   // '**'
      ADDA,  // '+='
      SUBA,  // '-='
      MULA,  // '*='
      DIVA,  // '/='
      MODA,  // '%='
      POWA,  // '**='
    };

    Type type;
    std::string val;

    Token_t(Type type, const std::string &val) : type(type), val(val) {}

    bool operator!=(Type t) { return type != t; }
    bool operator==(Type t) { return type == t; }
    bool operator!=(const std::string &s) { return val.compare(s); }
    bool operator==(const std::string &s) { return !val.compare(s); }

    bool isBinOperator() { return type >= Type::ADD && type <= Type::POW; }
  } Token;

  Lexer(const std::string &text) : text(text) {}

  char getc(size_t index) { return index >= text.size() ? -1 : text[index]; }

  Token next();
};

class Parser {
private:
  Lexer lexer;
  Lexer::Token token;

public:
  Parser(const Lexer &lexer) : lexer(lexer), token(next()) {}

  Lexer::Token next() {
    token = lexer.next();
    return token;
  }

  AST::Expr *parseExpr(bool topLevel = false);
  AST::LambdaExpr *parseLambda();
};

} // namespace Plasmatum
