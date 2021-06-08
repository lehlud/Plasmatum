#pragma once

#include <string>

namespace Plasmatum {
namespace Utils {

template <typename Base, typename T> inline bool instanceof (const T *) {
  return std::is_base_of<Base, T>::value;
}

std::string readFile(std::string name);

bool isDigit(char c);
bool isSpecial(char c);
bool isWhitespace(char c);

} // namespace Utils
} // namespace Plasmatum
