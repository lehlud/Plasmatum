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
    return result;
  else if ((result = parseString()))
    return result;
  else if ((result = parseVarExpr()))
    return result;

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

Expr *Parser::parseDefine() {
  skipSpaces();

  return nullptr;
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

Stmt *Parser::parseStmt() { return nullptr; }
