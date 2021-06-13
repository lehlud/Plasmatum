#include <iostream>
#include <memory>

#include "ast.h"
#include "error.h"
#include "utils.h"

using namespace Plasmatum;

std::map<AST::BinExpr::Type, uint8_t> AST::BinOpPrecedence;

void setBinOpPrecedence() {
  AST::BinOpPrecedence = {
      // logical operators
      {AST::BinExpr::Type::LOR, 10},
      {AST::BinExpr::Type::LAND, 10},
      {AST::BinExpr::Type::EQ, 10},
      {AST::BinExpr::Type::NE, 10},
      {AST::BinExpr::Type::GT, 10},
      {AST::BinExpr::Type::LT, 10},
      {AST::BinExpr::Type::GE, 10},
      {AST::BinExpr::Type::LE, 10},
      // bitwise operators
      {AST::BinExpr::Type::OR, 20},
      {AST::BinExpr::Type::AND, 20},
      // arithmetic operators
      {AST::BinExpr::Type::ADD, 30},
      {AST::BinExpr::Type::SUB, 30},
      {AST::BinExpr::Type::MUL, 31},
      {AST::BinExpr::Type::DIV, 31},
      {AST::BinExpr::Type::MOD, 31},
      {AST::BinExpr::Type::POW, 32},
  };
}

int main(int argc, char **argv) {
  if (argc > 1) {
    std::string tmp = Utils::readFile(argv[1]);
    setBinOpPrecedence();

    Lexer lexer(tmp);

    Parser parser(lexer);

    parser.parse();
  } else {
    std::puts("Please specify a name of a file!");
    return 1;
  }
  return 0;
}
