#include "utils.hh"

#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>

namespace plsm {

std::u32string readFile(const std::string &filename) {
  std::ifstream ifstream(filename);
  if (ifstream.bad()) {
    std::cout << "unable to read from file '" << filename << "'" << std::endl;
  }
  ifstream.imbue(
      std::locale(std::locale::classic(), new std::codecvt_utf8<char32_t>));
  std::stringstream sstream;
  sstream << ifstream.rdbuf();
  return to_u32(sstream.str());
}

char32_t charAt(const std::u32string &string, size_t index) {
  if (index >= string.size())
    return 0;
  else
    return string[index];
}

bool isBinOp(char32_t c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '<' ||
         c == '>' || c == '!';
}
bool isSpecial(char32_t c) {
  return isBinOp(c) || c == '(' || c == ')' || c == '{' || c == '}' ||
         c == '[' || c == ']';
}

bool isSeparator(char32_t c) {
  return isSpecial(c) || c == ' ' || std::iswspace(c) || c <= 0;
}

bool isStmtSeparator(char32_t c) { return c == ';'; }

uint8_t binOpPrec(char32_t op) {
  switch (op) {
  case '=':
  case '!':
  case '<':
  case '>':
    return 5;
  case '+':
  case '-':
    return 10;
  case '%':
    return 15;
  case '*':
    return 16;
  case '/':
    return 17;
  }

  return 0;
}

std::string to_str(const std::u32string &str) {
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
  return conv.to_bytes(str);
}

std::u32string to_u32(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
  return conv.from_bytes(str);
}

}
