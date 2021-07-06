#include "defines.hh"
#include "ast.hh"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

#include <map>
#include <vector>

llvm::LLVMContext Context;
llvm::Module Module("", Context);
llvm::IRBuilder<> Builder(Context);

llvm::Type *IntType = llvm::IntegerType::get(Context, INT_SIZE);
llvm::Type *FloatType = llvm::Type::getDoubleTy(Context);

typedef std::map<std::string, llvm::Value *> var_scope_t;
std::vector<var_scope_t> VarScopes = {var_scope_t()};

std::map<std::string, std::pair<Function *, std::vector<llvm::Function *>>>
    Functions;
