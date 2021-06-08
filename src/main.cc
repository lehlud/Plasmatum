#include <iostream>
#include <memory>

#include "ast.h"
#include "error.h"
#include "utils.h"

using namespace Plasmatum;

int main(int argc, char **argv) {
  if (argc > 1) {
    std::string tmp = Utils::readFile(argv[1]);
  } else
    std::cout << "Please specify a name of a file!" << std::endl;
  return 0;
}
