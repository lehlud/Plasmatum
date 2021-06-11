#pragma once

#include <string>

#include "ast.h"
#include "lexerparser.h"

namespace Plasmatum {
namespace Utils {

template <typename Base, typename T> inline bool instanceof (const T *) {
  return std::is_base_of<Base, T>::value;
}

std::string readFile(std::string name);

bool isDigit(char c);
bool isSpecial(char c);
bool isWhitespace(char c);

AST::BinExpr::Type TTToBET(Lexer::Token::Type type);

} // namespace Utils
} // namespace Plasmatum
