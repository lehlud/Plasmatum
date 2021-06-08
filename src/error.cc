#include "error.h"

#include <iostream>

using namespace Plasmatum;

std::string Error::typeToString(Type type) {
  switch (type) {
  case Type::DEFAULT:
    return "error";
  case Type::LEXER:
    return "lexer error";
  case Type::PARSER:
    return "parser error";
  case Type::COMPILER:
    return "compiler error";
  }
}

void Error::error(Type type, const std::string &msg) {
  std::cout << "\x1B[91m\x1B[1m" << typeToString(type) << ":\x1B[0m " << msg
            << std::endl;
  exit(1);
}

void Error::def(const std::string &msg) { error(Type::DEFAULT, msg); }

void Error::lexer(const std::string &msg) { error(Type::LEXER, msg); }

void Error::parser(const std::string &msg) { error(Type::PARSER, msg); }

void Error::compiler(const std::string &msg) { error(Type::COMPILER, msg); }

void Error::parserExpected(const std::string &expected,
                           const std::string &got) {
  error(Type::PARSER, "expected " + expected + ", got '" + got + "'");
}
