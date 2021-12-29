#pragma once

#include <iostream>
#include <string>

namespace std {
string to_string(u32string str);
ostream &operator<<(ostream &os, const u32string &s);
} // namespace std

extern "C" double num_print(double value);
extern "C" double num_println(double value);

extern "C" double str_print(const char32_t *value);
extern "C" double str_println(const char32_t *value);
