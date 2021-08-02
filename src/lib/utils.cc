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

std::string to_str(const std::u32string &str) {
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
  return conv.to_bytes(str);
}

std::u32string to_u32(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
  return conv.from_bytes(str);
}

}
