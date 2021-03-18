#include <memory>
#include <iostream>
#include "utils.hh"

extern "C" int yylex();
int yy_scan_string(const char*);
extern int yyparse();

extern int errors;
extern std::vector<StmtAST*> *program;

int main(int argc, char **argv) {
    if (argc > 1) {
        std::string tmp = readFile(argv[1]).c_str();
        yy_scan_string(tmp.c_str());
        yyparse();
        if (errors) exit(1);
        return execProgram(std::move(program));
    } else std::cout << "Please specify a name of a file!\n" << std::endl;
    return 0;
}
