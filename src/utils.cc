#include "utils.hh"

#include <codecvt>
#include <locale>

char32_t charAt(const std::u32string &string, size_t index) {
  if (index >= string.size())
    return 0;
  else
    return string[index];
}

bool isIdChar(char32_t c) { return (c >= 'A' && c <= 'z') || c == '_'; }
bool isBinOp(char32_t c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '<';
}
bool isSpecial(char32_t c) {
  return isBinOp(c) || c == '(' || c == ')' || c == '{' || c == '}' ||
         c == '[' || c == ']';
}

bool isSeparator(char32_t c) {
  return isSpecial(c) || c == ' ' || std::iswspace(c) || c <= 0;
}

std::string to_str(const std::u32string &str) {
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
  return conv.to_bytes(str);
}

std::u32string to_u32(const std::string &str) {
  return std::u32string(str.begin(), str.end());
}
