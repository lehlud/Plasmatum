#include "parser.hh"
#include "utils.hh"

#include <iostream>

Expr *Parser::parseExpr() {
  Expr *result = nullptr;

  while (std::iswspace(charAt(text, index))) {
    index += 1;
  }

  if ((result = parseNumber()))
    return result;
  else if ((result = parseString()))
    return result;
  else if ((result = parseIdentifier()))
    return result;

  return nullptr;
}

Expr *Parser::parseNumber() {
  size_t tmpIdx = index;

  std::string tmp;
  if (charAt(text, tmpIdx) == '0' && charAt(text, tmpIdx + 1) == 'x') {
    while (std::isdigit(charAt(text, tmpIdx))) {
      tmp += charAt(text, tmpIdx);
      tmpIdx += 1;
    }

    if (isSeparator(charAt(text, tmpIdx))) {
      index = tmpIdx + 1;
      return new IntExpr(std::stoul(tmp, 0, 16));
    } else {
      return nullptr;
    }
  }

  while (std::isdigit(charAt(text, tmpIdx))) {
    tmp += charAt(text, tmpIdx);
    tmpIdx += 1;
  }

  if (charAt(text, tmpIdx) == '.') {
    if (!std::isdigit(charAt(text, tmpIdx + 1))) {
      return nullptr;
    }

    tmp += '.';
    tmpIdx += 1;

    while (std::isdigit(charAt(text, tmpIdx))) {
      tmp += charAt(text, tmpIdx);
      tmpIdx += 1;
    }

    index = tmpIdx;
    return new FloatExpr(std::stod(tmp));
  } else if (tmp.size()) {
    index = tmpIdx;
    return new IntExpr(std::stoul(tmp));
  }

  return nullptr;
}

Expr *Parser::parseString() {
  if (charAt(text, index) != '\'')
    return nullptr;

  size_t tmpIdx = index + 1;
  std::u32string tmp;

  while (charAt(text, tmpIdx) != '\'' && charAt(text, tmpIdx) > 0) {
    tmp += charAt(text, tmpIdx);
    tmpIdx += 1;
  }

  if (charAt(text, tmpIdx) != '\'') {
    std::cout << "unexpected end of file while parsing string" << std::endl;
    exit(1);
    return nullptr;
  }

  index = tmpIdx + 2;
  return new StringExpr(tmp);
}

Expr *Parser::parseIdentifier() {
  if (!isIdChar(charAt(text, index)))
    return nullptr;

  size_t tmpIdx = index;
  std::string tmp;

  while (isIdChar(charAt(text, tmpIdx))) {
    tmp += charAt(text, tmpIdx);
    tmpIdx += 1;
  }

  if (!isSeparator(charAt(text, tmpIdx))) {
    std::cout << "invalid character within identifier" << std::endl;
    exit(1);
    return nullptr;
  }

  index = tmpIdx;
  return new VarExpr(tmp);
}

Stmt *Parser::parseStmt() { return nullptr; }
