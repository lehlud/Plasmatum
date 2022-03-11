#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "AST/Stmt.h"

int errors = 0;
extern int yyparse();
extern int yy_scan_string(const char *str);

std::vector<Stmt *> program;

std::string readFile(std::string name) {
    std::ifstream file;
    file.open(name);
    std::stringstream sstream;
    sstream << file.rdbuf();
    return sstream.str();
}

int main(int argc, char **argv) {
    if (argc > 1) {
        std::string tmp = readFile(argv[1]).c_str();

        yy_scan_string(tmp.c_str());
        yyparse();

        if (errors) return errors;

        for (auto &stmt : program) {
            stmt->print();
            std::cout << std::endl;
        }

        return 0;
    } else {
        std::cout << "Please specify a name of a file!" << std::endl;
        return 1;
    }
}
