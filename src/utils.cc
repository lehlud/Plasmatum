#include <fstream>
#include <sstream>

#include "utils.h"
#include "error.h"

using namespace Plasmatum;

std::string Utils::readFile(std::string name) {
  std::ifstream fs(name);

  if (fs.bad()) Error::def("cannot read from file '" + name + "'");

  std::stringstream ss;
  ss << fs.rdbuf();
  return ss.str();
}

bool Utils::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Utils::isSpecial(char c) {
    return c == '('
        || c == ')'
        || c == '['
        || c == ']'
        || c == '{'
        || c == '}'
        || c == '*'
        || c == '?'
        || c == ':'
        || c == '+'
        || c == '-'
        || c == '*'
        || c == '/'
        || c == '%'
        || c == '<'
        || c == '>'
        || c == '=';
}

bool Utils::isWhitespace(char c) {
    return c <= ' ';
}

AST::BinExpr::Type Utils::TTToBET(Lexer::Token::Type type) {
    switch (type) {
        case Lexer::Token::Type::ADD: return AST::BinExpr::ADD;
        case Lexer::Token::Type::SUB: return AST::BinExpr::SUB;
        case Lexer::Token::Type::MUL: return AST::BinExpr::MUL;
        case Lexer::Token::Type::DIV: return AST::BinExpr::DIV;
        case Lexer::Token::Type::MOD: return AST::BinExpr::MOD;
        case Lexer::Token::Type::POW: return AST::BinExpr::POW;
        default: Error::def("unable to convert token to binary operator");
    }
    return AST::BinExpr::ERR;
}

llvm::Value *Utils::plsmVal(llvm::Value *v) {
    return nullptr;
}

llvm::Value *Utils::tryCast(llvm::Value *v, llvm::Type *t) {
    return nullptr;
}
