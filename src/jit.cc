#include "jit.h"

using namespace Plasmatum;

void Compiler::Context::runFPM(llvm::Function *f) { fpm.run(*f, fam); }
