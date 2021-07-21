#include "parser.hh"
#include "utils.hh"

#include <iostream>

void Parser::skipSpaces() {
  while (std::iswspace(charAt(text, index))) {
    index += 1;
  }
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

  Expr *result = nullptr;

  if ((result = parseNumber()))
    return parseOptionalBinExpr(result);
  else if ((result = parseString()))
    return parseOptionalBinExpr(result);
  else if ((result = parseIfExpr()))
    return parseOptionalBinExpr(result);
  else if ((result = parseParenExpr()))
    return parseOptionalBinExpr(result);
  else if ((result = parseVarExpr()))
    return parseOptionalBinExpr(result);

  return nullptr;
}

Expr *Parser::parseCallExpr(const std::string &id) {
  index += 1;

  skipSpaces();

  std::vector<Expr *> args;
  while (charAt(text, index) != ')' && charAt(text, index) > 0) {
    Expr *expr = parseExpr();
    if (!expr) {
      std::cout << "expected expression in function call argument list"
                << std::endl;
      exit(1);
      return nullptr;
    }
    args.push_back(expr);

    skipSpaces();
  }

  index += 1;

  return new CallExpr(id, args);
}

Expr *Parser::parseNumber() {
  size_t origIdx = index;
  std::string tmp;

  if (charAt(text, index) == '0' && charAt(text, index + 1) == 'x') {
    index += 2;

    while (std::isdigit(charAt(text, index))) {
      tmp += charAt(text, index);
      index += 1;
    }

    if (isSeparator(charAt(text, index))) {
      return new IntExpr(std::stoul(tmp, 0, 16));
    } else {
      index = origIdx;
      return nullptr;
    }
  }

  while (std::isdigit(charAt(text, index))) {
    tmp += charAt(text, index);
    index += 1;
  }

  if (charAt(text, index) == '.') {
    if (!std::isdigit(charAt(text, index + 1))) {
      index = origIdx;
      return nullptr;
    }

    tmp += '.';
    index += 1;

    while (std::isdigit(charAt(text, index))) {
      tmp += charAt(text, index);
      index += 1;
    }

    return new FloatExpr(std::stod(tmp));
  } else if (tmp.size()) {
    return new IntExpr(std::stoul(tmp));
  }

  index = origIdx;
  return nullptr;
}

Expr *Parser::parseString() {
  if (charAt(text, index) != '\'')
    return nullptr;

  size_t origIdx = index;

  index += 1;
  std::u32string tmp;

  while (charAt(text, index) != '\'' && charAt(text, index) > 0) {
    tmp += charAt(text, index);
    index += 1;
  }

  if (charAt(text, index) != '\'') {
    std::cout << "unexpected end of file while parsing string" << std::endl;
    exit(1);
    return nullptr;
  }

  index += 1;
  return new StringExpr(tmp);
}

Expr *Parser::parseVarExpr() {
  size_t origIdx = index;
  std::string tmp = parseIdentifier();

  if (!tmp.size()) {
    index = origIdx;
    return nullptr;
  }

  skipSpaces();

  if (charAt(text, index) == '(') {
    return parseCallExpr(tmp);
  }

  return new VarExpr(tmp);
}

Expr *Parser::parseIfExpr() {
  size_t origIdx = index;
  std::string tmp = parseIdentifier();

  if (tmp != "if") {
    index = origIdx;
    return nullptr;
  }

  skipSpaces();

  Expr *cond = parseExpr();
  if (!cond) {
    index = origIdx;
    return nullptr;
  }

  skipSpaces();

  Expr *trueExpr = parseExpr();
  if (!trueExpr) {
    std::cout << "error while parsing if expr" << std::endl;
    exit(1);
    return nullptr;
  }

  skipSpaces();

  tmp = parseIdentifier();
  if (tmp != "else") {
    std::cout << "error while parsing if expr" << std::endl;
    exit(1);
    return nullptr;
  }

  skipSpaces();

  Expr *falseExpr = parseExpr();
  if (!falseExpr) {
    std::cout << "error while parsing if expr" << std::endl;
    exit(1);
    return nullptr;
  }

  return new IfExpr(cond, trueExpr, falseExpr);
}

Expr *Parser::parseParenExpr() {
  if (charAt(text, index) != '(') {
    return nullptr;
  }

  size_t origIdx = index;

  index += 1;

  Expr *expr = parseExpr();

  skipSpaces();

  if (!expr || charAt(text, index) != ')') {
    index = origIdx;
    return nullptr;
  }

  index += 1;

  return expr;
}

Expr *Parser::parseOptionalBinExpr(Expr *expr) {
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

Stmt *Parser::parseStmt() {
  skipSpaces();

  Stmt *result = nullptr;

  if ((result = parseDefine()))
    return result;
  else if ((result = parseReturn()))
    return result;
  else if ((result = parseExprStmt()))
    return result;


  return nullptr;
}

Stmt *Parser::parseDefine() {
  size_t origIdx = index;

  std::string id = parseIdentifier();
  if (id != "define") {
    index = origIdx;
    return nullptr;
  }

  skipSpaces();

  id = parseIdentifier();

  if (!id.size()) {
    index = origIdx;
    return nullptr;
  }

  skipSpaces();

  if (charAt(text, index) != '(') {
    index = origIdx;
    return nullptr;
  }

  index += 1;
  skipSpaces();

  std::vector<std::string> args;
  while (charAt(text, index) != ')' && charAt(text, index) > 0) {
    std::string tmp = parseIdentifier();
    if (!tmp.size()) {
      index = origIdx;
      return nullptr;
    }
    args.push_back(tmp);

    skipSpaces();
  }

  index += 1;
  skipSpaces();

  if (charAt(text, index) != '=') {
    index = origIdx;
    return nullptr;
  }

  index += 1;

  std::vector<Stmt *> body;

  if (charAt(text, index) == '>') {
    index += 1;
    skipSpaces();

    if (charAt(text, index) != '(') {
      index = origIdx;
      return nullptr;
    }

    index += 1;
    skipSpaces();

    while (charAt(text, index) != ')' && charAt(text, index) > 0) {
      Stmt *stmt = parseStmt();

      if (!stmt) {
        index = origIdx;
        return nullptr;
      }

      body.push_back(stmt);
    }

  } else {
    skipSpaces();

    Expr *result = parseExpr();
    if (!result) {
      index = origIdx;
      return nullptr;
    }

    body = {new ReturnStmt(result)};
  }

  return new FunctionStmt(id, body, args);
}

Stmt *Parser::parseReturn() {
  size_t origIdx = index;
  std::string tmp = parseIdentifier();

  if (tmp != "return") {
    index = origIdx;
    return nullptr;
  }

  skipSpaces();

  Expr *result = parseExpr();
  if (!result) {
    index = origIdx;
    return nullptr;
  }
  return new ReturnStmt(result);
}

Stmt *Parser::parseExprStmt() {
  size_t origIdx = index;
  Expr *result = parseExpr();

  if (!result) {
    index = origIdx;
    return nullptr;
  }

  return new ExprStmt(result);
}
