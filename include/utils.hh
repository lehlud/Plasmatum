#pragma once

#include <string>

namespace plsm {



std::u32string readFile(const std::string &filename);

std::string to_str(const std::u32string &str);
std::u32string to_u32(const std::string &str);

}
