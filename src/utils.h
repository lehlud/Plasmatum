#pragma once

#include <string>

#include "ast.h"
#include "jit.h"
#include "lexerparser.h"

namespace Plasmatum {
namespace Utils {

template <typename Base, typename T> inline bool instanceof (const T *) {
  return std::is_base_of<Base, T>::value;
}

template <typename Base1, typename Base2, typename T> inline bool instanceof (const T *) {
  return std::is_base_of<Base1, T>::value || std::is_base_of<Base2, T>::value;
}

std::string readFile(std::string name);

bool isDigit(char c);
bool isSpecial(char c);
bool isWhitespace(char c);

AST::BinExpr::Type TTToBET(Lexer::Token::Type type);

#define TYPE_INT 1
#define TYPE_FLOAT 2
#define TYPE_LIST 3

llvm::Value *plsmVal(Compiler::Context &context, llvm::Value *v);

llvm::Value *tryCast(Compiler::Context &context, llvm::Value *v, llvm::Type *t);

llvm::Value *tryLogicalVal(Compiler::Context &context, llvm::Value *v);

} // namespace Utils
} // namespace Plasmatum
