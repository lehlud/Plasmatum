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

  if (c == -1)
    return Token(TT::EOF, "EOF");

  Lexer::Token *t = nullptr;

  switch (c) {
  case '(':
    index += 1;
    return Token(TT::PO, "(");
  case ')':
    index += 1;
    return Token(TT::PC, ")");
  case '[':
    index += 1;
    return Token(TT::BO, "[");
  case ']':
    index += 1;
    return Token(TT::BC, "]");
  case '{':
    index += 1;
    return Token(TT::CO, "{");
  case '}':
    index += 1;
    return Token(TT::CC, "}");
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

    if (c != '.')
      return Token(TT::INT, str);
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

  while (!Utils::isSpecial(c) && !Utils::isWhitespace(c) && c != -1) {
    str += c;
    c = getc(++index);
  }

  if (str.size() > 0)
    return Token(TT::ID, str);

  Error::lexer("unexpected lexer input ('" + std::string(1, c) + "')");

  return Token(TT::EOF, "EOF");
}

AST::Expr *Parser::parseExpr(bool topLevel) {
  AST::Expr *result = nullptr;
  if (topLevel) {
    if (token == TT::ID) {
      if (token == "def") {
        next();

        if (token != TT::ID)
          Error::parserExpected("identifier", token.val);

        auto id = token.val;

        next();

        auto base = parseLambda();

        result = new AST::Function(base, id);
      }
    }

    if (!result)
      Error::parser("expected top level expression");

    return result;
  } else {
    if (token == TT::ID) {
      if (token == "lambda") {
        next();
        result = parseLambda();
      } else {
        result = new AST::IdExpr(token.val);
        next();
      }
    } else if (token == TT::PO) {
      next();

      result = parseExpr();

      if (token != TT::PC)
        Error::parserExpected("')'", token.val);

      next();
    } else if (token == TT::INT) {
      result = new AST::IntExpr(std::stol(token.val));
      next();
    } else if (token == TT::FLOAT) {
      result = new AST::FPExpr(std::stod(token.val));
      next();
    }
  }

  if (!result)
    Error::parserExpected("expression", token.val);

  result = parseOptBinExpr(result);

  if (token == TT::QM) {
    next();

    auto exprTrue = parseExpr();

    if (token != TT::COL)
      Error::parserExpected("':'", token.val);

    next();

    auto exprFalse = parseExpr();

    return new AST::IfExpr(result, exprTrue, exprFalse);
  }

  return result;
}

AST::Expr *Parser::parseOptBinExpr(AST::Expr *left) {
  if (token.isBinOperator()) {
    auto op = Utils::TTToBET(token.type);

    next();

    auto right = parseExpr();

    if (token.isBinOperator()) {
      auto tmpOp = Utils::TTToBET(token.type);

      if (AST::BinOpPrecedence[tmpOp] <= AST::BinOpPrecedence[op]) {
        return parseOptBinExpr(new AST::BinExpr(op, left, right));
      }

      right = parseOptBinExpr(right);
    }

    return new AST::BinExpr(op, left, right);
  } else {
    return left;
  }
}

AST::LambdaExpr *Parser::parseLambda() {
  if (token != TT::PO)
    Error::parserExpected("'('", token.val);

  next();

  std::vector<std::string> args;
  while (token != TT::PC) {
    if (token != TT::ID)
      Error::parserExpected("identifer", token.val);

    args.push_back(token.val);

    next();
  }

  next();

  if (token != TT::ARR)
    Error::parserExpected("'->'", token.val);

  next();

  auto body = parseExpr(false);

  return new AST::LambdaExpr(args, body);
}
