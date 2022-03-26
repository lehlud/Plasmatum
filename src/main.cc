#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Context.h"
#include "AST/Stmt.h"
#include "AST/Expr.h"
#include "AST/Type.h"

#include <llvm/Support/TargetSelect.h>

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

void initLLVM() {
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllTargetInfos();

    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
}

int main(int argc, char **argv) {
    if (argc > 1) {
        std::string tmp = readFile(argv[1]).c_str();

        yy_scan_string(tmp.c_str());
        yyparse();

        if (errors) return errors;

        initLLVM();

        Context context;

        for (auto &stmt : program) {
            stmt->print();
            std::cout << std::endl;

            stmt->codegen(&context);
        }

        // auto function = new FunctionExpr({std::make_pair("x", new TypeRef("Num"))}, new NumExpr(42));
        // function->codegen(&context);

        context.module.print(llvm::errs(), 0);

        return 0;
    } else {
        std::cerr << "Please specify a name of a file!" << std::endl;
        return 1;
    }
}
