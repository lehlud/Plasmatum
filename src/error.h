#pragma once

#include <string>

namespace Plasmatum {
namespace Error {

enum Type {
  DEFAULT,
  LEXER,
  PARSER,
  COMPILER,
};

std::string typeToString(Type type);

void error(Type type, const std::string &msg);
void def(const std::string &msg);
void lexer(const std::string &msg);
void parser(const std::string &msg);
void compiler(const std::string &msg);

void parserExpected(const std::string &expected, const std::string &got);

} // namespace Error
} // namespace Plasmatum
