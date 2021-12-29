#pragma once

#include "ast.hh"

void compileAndExecute(std::vector<Stmt *> stmts);
void execute(Context &context);
