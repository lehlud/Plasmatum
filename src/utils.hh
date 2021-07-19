#pragma once

#include <string>

char32_t charAt(const std::u32string &string, size_t index);

bool isIdChar(char32_t c);
bool isBinOp(char32_t c);
bool isSpecial(char32_t c);
bool isSeparator(char32_t c);

std::string to_str(const std::u32string &str);
std::u32string to_u32(const std::string &str);
