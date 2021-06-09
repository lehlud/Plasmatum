#include "lexerparser.h"
#include "error.h"
#include "utils.h"

using namespace Plasmatum;

typedef Lexer::Token::Type TT;

inline Lexer::Token *ttAltEq(size_t &index, char c, char c1, char cmp, TT t1,
                             TT t2) {
  if (c == cmp) {
    if (c1 == '=') {
      index += 2;
      return new Lexer::Token(t2, std::string({cmp, '='}));
    }
    index += 1;
    return new Lexer::Token(t1, std::string(1, cmp));
  }
  return nullptr;
}

Lexer::Token Lexer::next() {
  char c, c1;

  while (Utils::isWhitespace((c = getc(index))))
    index += 1;

  Lexer::Token *t = nullptr;

  switch (c) {
  case '(':
    index += 1;
    return Token(TT::PO, "(");
  case ')':
    index += 1;
    return Token(TT::PC, ")");
  case '?':
    index += 1;
    return Token(TT::QM, "?");
  case ':':
    index += 1;
    return Token(TT::COL, ":");
  }

  if (c == '-' && (c1 = getc(index + 1)) == '>') {
    index += 2;
    return Token(TT::ARR, "->");
  } else if ((t = ttAltEq(index, c, c1, '<', TT::LT, TT::LE))) {
    return *t;
  } else if ((t = ttAltEq(index, c, c1, '>', TT::GT, TT::GE))) {
    return *t;
  } else if ((t = ttAltEq(index, c, c1, '=', TT::ASS, TT::EQ))) {
    return *t;
  } else if ((t = ttAltEq(index, c, c1, '+', TT::ADD, TT::ADDA))) {
    return *t;
  } else if ((t = ttAltEq(index, c, c1, '-', TT::SUB, TT::SUBA))) {
    return *t;
  } else if ((t = ttAltEq(index, c, c1, '*', TT::MUL, TT::MULA))) {
    return *t;
  } else if ((t = ttAltEq(index, c, c1, '/', TT::DIV, TT::DIVA))) {
    return *t;
  } else if ((t = ttAltEq(index, c, c1, '%', TT::MOD, TT::MODA))) {
    return *t;
  }

  std::string str;

  if (Utils::isDigit(c)) {
    str += c;
    c = getc(++index);

    while (Utils::isDigit(c)) {
      str += c;
      c = getc(++index);
    }

    if (c != '.') return Token(TT::INT, str);
  }

  if (c == '.' && Utils::isDigit((c1 = getc(index + 1)))) {
    str += c;
    str += c1;

    index += 2;

    while (Utils::isDigit(c = getc(index))) {
      str += c;
    }

    return Token(TT::FLOAT, str);
  } else if (c == '.') {
    Error::lexer("expected digit after '.', got '" + std::string(1, c) + "'");
  }

  while (!Utils::isSpecial(c) && c != -1) {
    str += c;
    c = getc(++index);
  }

  if (str.size() > 0) return Token(TT::ID, str);

  Error::lexer("unexpected lexer input ('" + std::string(1, c) + "')");

  return Token(TT::EOF, "EOF");
}

AST::Expr *Parser::parseExpr(bool topLevel) {
  if (topLevel) {
    if (token.type == TT::ID) {

      bool def = false;
      bool lambda = false;

      std::string defId;

      if (token.val == "def") {
        next();

        if (token.type != TT::ID)
          Error::parserExpected("identifier", token.val);

        defId = token.val;

        next();

        def = true;
      } else if (token.val == "lambda") {
        next();
      }

      if (def || lambda) {
        if (token.type != TT::PO)
          Error::parserExpected("'('", token.val);

        next();

        std::vector<std::string> args;
        while (token.type != TT::PC) {
          if (token.type != TT::ID)
            Error::parserExpected("identifer", token.val);

          args.push_back(token.val);

          next();
        }

        auto body = parseExpr(false);

        next();

        auto base = new AST::LambdaExpr(args, body);

        if (lambda)
          return base;
        else
          return new AST::Function(base, defId);
      }
    }
    return nullptr;
  } else {
    return nullptr;
  }
}
