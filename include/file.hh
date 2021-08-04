#pragma once

#include <codecvt>
#include <fstream>
#include <locale>
#include <sstream>
#include <string>

#include "string.hh"

namespace plsm {

class file {
private:
  std::string _filename;

public:
  file(const std::string &__filename) : _filename(__filename) {}

  inline string read() {
    std::ifstream __ifstream_(_filename);
    __ifstream_.imbue(
        std::locale(std::locale::classic(), new std::codecvt_utf8<char32_t>));
    std::stringstream __sstream_;
    __sstream_ << __ifstream_.rdbuf();
    return string(__sstream_.str());
  }
};

} // namespace plsm
