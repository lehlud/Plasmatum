#include "parser.hh"
#include "utils.hh"

#include <iostream>

void Parser::skipSpaces() {
  while (std::iswspace(charAt(text, index))) {
    index += 1;
  }
}

void Parser::forward() {
  index += 1;
  skipSpaces();
}

std::string Parser::parseIdentifier() {
  if (!isIdChar(charAt(text, index)))
    return "";

  std::string tmp;

  while (isIdChar(charAt(text, index))) {
    tmp += charAt(text, index);
    index += 1;
  }

  if (!isSeparator(charAt(text, index))) {
    std::cout << "invalid character within identifier" << std::endl;
    exit(1);
    return nullptr;
  }

  return tmp;
}

Expr *Parser::parseExpr() {
  skipSpaces();

  std::string tmp;
  if (charAt(text, index) == '(') {
    index += 1;

    Expr *expr = parseExpr();

    skipSpaces();

    if (charAt(text, index) != ')') {
      std::cout << "error: expected closing parenthesis" << std::endl;
      std::exit(1);
    }

    index += 1;

    return expr;
  } else if (charAt(text, index) == '\'') {
    index += 1;

    std::u32string tmpStr;

    while (charAt(text, index) != '\'' && charAt(text, index) > 0) {
      tmpStr += charAt(text, index);
      index += 1;
    }

    if (charAt(text, index) <= 0) {
      std::cout << "error: unexpected end of file" << std::endl;
    }

    index += 1;

    return parseOptionalBinExpr(new StringExpr(tmpStr));
  } else if (charAt(text, index) == '0' && charAt(text, index) == 'x') {
    index += 2;

    while (std::isxdigit(charAt(text, index))) {
      tmp += charAt(text, index);
      index += 1;
    }

    return parseOptionalBinExpr(new IntExpr(std::stoul(tmp, nullptr, 16)));
  } else if (std::isdigit(charAt(text, index))) {
    tmp += charAt(text, index);
    index += 1;

    while (std::isdigit(charAt(text, index))) {
      tmp += charAt(text, index);
      index += 1;
    }
  }

  if (charAt(text, index) == '.') {
    if (!tmp.size()) {
      tmp += '0';
    }

    tmp += charAt(text, index);
    index += 1;

    if (!std::isdigit(charAt(text, index))) {
      std::cout << "error: expected digit after '.'" << std::endl;
      std::exit(1);
    }

    while (std::isdigit(charAt(text, index))) {
      tmp += charAt(text, index);
      index += 1;
    }

    return parseOptionalBinExpr(new FloatExpr(std::stod(tmp)));
  } else if (tmp.size()) {
    return parseOptionalBinExpr(new IntExpr(std::stoul(tmp)));
  }

  if ((tmp = parseIdentifier()).size()) {
    skipSpaces();

    if (tmp == "if") {
      Expr *condition = parseExpr();
      if (!condition) {
        std::cout << "error: expected condition after 'if'" << std::endl;
        std::exit(1);
      }

      Expr *trueExpr = parseExpr();
      if (!trueExpr) {
        std::cout << "error: expected 'true-value' in conditional expression"
                  << std::endl;
        std::exit(1);
      }

      if (parseIdentifier() != "else") {
        std::cout << "error: expected 'else' in conditional expression"
                  << std::endl;
        std::exit(1);
      }

      Expr *falseExpr = parseExpr();
      if (!falseExpr) {
        std::cout << "error: expected 'false-value' in conditional expression"
                  << std::endl;
        std::exit(1);
      }

      return parseOptionalBinExpr(new IfExpr(condition, trueExpr, falseExpr));
    } else if (charAt(text, index) == '(') {
      forward();

      std::vector<Expr *> args;
      while (charAt(text, index) != ')' && charAt(text, index) > 0) {
        Expr *expr = parseExpr();

        if (!expr) {
          std::cout << "error: expected expression in argument list"
                    << std::endl;
          std::exit(1);
        }

        args.push_back(expr);

        skipSpaces();
      }

      if (charAt(text, index) <= 0) {
        std::cout << "error: unexpected end of file" << std::endl;
        std::exit(1);
      }

      forward();

      return parseOptionalBinExpr(new CallExpr(tmp, args));
    } else {
      return parseOptionalBinExpr(new VarExpr(tmp));
    }
  }

  return nullptr;
}

Stmt *Parser::parseStmt() {
  skipSpaces();

  size_t orig_index = index;

  std::string tmp = parseIdentifier();

  if (tmp.size()) {
    if (tmp == "return") {
      skipSpaces();

      Expr *expr = parseExpr();

      if (!expr) {
        std::cout << "error: expected expression after 'return'" << std::endl;
        std::exit(1);
      }

      return new ReturnStmt(expr);
    } else if (tmp == "define") {
      skipSpaces();

      std::string id = parseIdentifier();
      if (!id.size()) {
        std::cout << "error: expected identifier after 'define'" << std::endl;
        std::exit(1);
      }

      skipSpaces();

      if (charAt(text, index) != '(') {
        std::cout << "error: expected argument list in function definition"
                  << std::endl;
        std::exit(1);
      }

      forward();

      std::vector<std::string> args;
      while (charAt(text, index) != ')' && charAt(text, index) > 0) {
        std::string arg = parseIdentifier();

        if (!arg.size()) {
          std::cout << "error: expected named argument" << std::endl;
          std::exit(1);
        }

        args.push_back(arg);

        skipSpaces();
      }

      if (charAt(text, index) <= 0) {
        std::cout << "error: unexpected end of file" << std::endl;
      }

      forward();

      if (charAt(text, index) != '=') {
        std::cout << "error: expected '=' after function prototype"
                  << std::endl;
        std::exit(1);
      }

      index += 1;

      std::vector<Stmt *> body;
      if (charAt(text, index) == '>') {
        forward();

        if (charAt(text, index) != '(') {
          std::cout << "error: expected '(' as a extended function body opener"
                    << std::endl;
          std::exit(1);
        }

        forward();

        while (charAt(text, index) != ')' && charAt(text, index) > 0) {
          Stmt *stmt = parseStmt();

          if (!stmt) {
            std::cout << "error: expected statement in function body"
                      << std::endl;
            std::exit(1);
          }

          body.push_back(stmt);

          skipSpaces();
        }

        if (charAt(text, index) <= 0) {
          std::cout << "error: unexpected end of file" << std::endl;
        }

        forward();
      } else {
        skipSpaces();

        Expr *expr = parseExpr();

        if (!expr) {
          std::cout << "error: expected expression in simple function body"
                    << std::endl;
          std::exit(1);
        }

        body = {new ReturnStmt(expr)};
      }

      return new FunctionStmt(id, body, args);
    }
  }

  index = orig_index;
  Expr *expr = parseExpr();

  if (!expr) {
    index = orig_index;
  } else {
    return new ExprStmt(expr);
  }

  return nullptr;
}

Expr *Parser::parseOptionalBinExpr(Expr *expr, uint8_t prec) {
  skipSpaces();

  char32_t op;
  if (!isBinOp((op = charAt(text, index)))) {
    return expr;
  }

  size_t origIdx = index;

  index += 1;

  bool isEq = false;
  if (charAt(text, index) == '=') {
    isEq = true;
    index += 1;
  } else if (op == '=' || op == '!') {
    index = origIdx;
    return expr;
  }

  skipSpaces();

  Expr *right = parseExpr();
  if (!right) {
    index = origIdx;
    return expr;
  }

  switch (op) {
  case '+':
    return new AddBinExpr(expr, right);
  case '-':
    return new SubBinExpr(expr, right);
  case '*':
    return new MulBinExpr(expr, right);
  case '/':
    return new DivBinExpr(expr, right);
  case '%':
    return new ModBinExpr(expr, right);
  case '<':
    if (isEq)
      return new LEBinExpr(expr, right);
    else
      return new LTBinExpr(expr, right);
  case '>':
    if (isEq)
      return new GEBinExpr(expr, right);
    else
      return new GTBinExpr(expr, right);
  case '=':
    return new EQBinExpr(expr, right);
  case '!':
    return new NEBinExpr(expr, right);
  }

  index = origIdx;
  return expr;
}
