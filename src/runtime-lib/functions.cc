#include "runtime_functions.hh"

#include <codecvt>
#include <iostream>
#include <locale>

std::string std::to_string(std::u32string str) {
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cv;
  return cv.to_bytes(str);
}

std::ostream &operator<<(std::ostream &os, const std::u32string &s) {
  return (os << std::to_string(s));
}

double num_print(double value) {
  std::cout << value;
  return 0;
}

double num_println(double value) {
  std::cout << value << std::endl;
  return 0;
}

double str_print(const char32_t *value) {
  std::cout << value;
  return 0;
}

double str_println(const char32_t *value) {
  std::cout << value << std::endl;
  return 0;
}
