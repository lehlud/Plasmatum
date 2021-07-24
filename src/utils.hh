#pragma once

#include <string>

std::u32string readFile(const std::string &filename);

char32_t charAt(const std::u32string &string, size_t index);

bool isEOL(char32_t c);
bool isIdChar(char32_t c);
bool isBinOp(char32_t c);
bool isSpecial(char32_t c);
bool isSeparator(char32_t c);
bool isStmtSeparator(char32_t c);

std::string to_str(const std::u32string &str);
std::u32string to_u32(const std::string &str);
