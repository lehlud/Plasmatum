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
