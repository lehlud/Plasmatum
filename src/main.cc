#include "ast.hh"
#include "parser.hh"
#include "engine.hh"

#include <iostream>
#include <vector>

void printUsage(char *arg0) {
  std::cout << "usage: " << arg0 << " <file>" << std::endl;
}

int main(int argc, char **argv) {
  if (argc <= 0) {
    return 1;
  } else if (argc <= 1) {
    printUsage(argv[0]);
    return 1;
  }

  std::u32string text = readFile(argv[1]);

  Parser parser(text);

  std::vector<Stmt *> stmts;
  Stmt *stmt = nullptr;
  while ((stmt = parser.parseStmt())) {
    stmts.push_back(stmt);
    // std::cout << to_str(stmt->to_string()) << std::endl;
  }

  

  return 0;
}
