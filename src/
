#pragma once

#include <string>

namespace Plasmatum {
namespace Utils {

template <typename Base, typename T> inline bool instanceof (const T *) {
  return std::is_base_of<Base, T>::value;
}

std::string readFile(std::string name);

} // namespace Utils
} // namespace Plasmatum
