#pragma once

#include <map>
#include <string>

#include <llvm/ADT/Hashing.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Passes/PassBuilder.h>

#include "error.h"

#define TYPE_INT 1
#define TYPE_FLOAT 1
#define TYPE_ARRAY 1
#define TYPE_FUNC 1
#define TYPE_STRUCT 1

namespace Plasmatum {
namespace AST {
class LambdaExpr;
}
namespace Compiler {

class Value {
public:
  virtual ~Value() = default;
  virtual bool isLLVMValue() { return false; }
  virtual bool isFunctionValue() { return false; }
};
class LLVMValue : public Value {
public:
  llvm::Type *type;
  llvm::Value *value;

  LLVMValue(llvm::Type *type, llvm::Value *value) : type(type), value(value) {}

  bool isLLVMValue() override { return true; }
};
class FunctionValue : public Value {
public:
  ::Plasmatum::AST::LambdaExpr *lambda;

  FunctionValue(::Plasmatum::AST::LambdaExpr *lambda) : lambda(lambda) {}

  bool isFunctionValue() override { return true; }
};

typedef std::map<std::string, Value *> scope;

class Context {
private:
  std::vector<scope> scopes;

  llvm::ModuleAnalysisManager mam;
  llvm::CGSCCAnalysisManager gam;
  llvm::FunctionAnalysisManager fam;
  llvm::LoopAnalysisManager lam;
  llvm::FunctionPassManager fpm;

public:
  llvm::Module *mod;
  llvm::IRBuilder<> *builder;

  bool needsRef = false;
  std::vector<llvm::Type *> *argTypes = nullptr;

  Context(llvm::Module *mod, llvm::IRBuilder<> *builder)
      : mod(mod), builder(builder) {
    llvm::PassBuilder passBuilder;

    passBuilder.registerModuleAnalyses(mam);
    passBuilder.registerCGSCCAnalyses(gam);
    passBuilder.registerFunctionAnalyses(fam);
    passBuilder.registerLoopAnalyses(lam);

    passBuilder.crossRegisterProxies(lam, fam, gam, mam);

    fpm = passBuilder.buildFunctionSimplificationPipeline(
        llvm::PassBuilder::OptimizationLevel::O3,
        llvm::ThinOrFullLTOPhase::None);
  }

  void runFPM(llvm::Function *f);

  void clear() {
    mam.clear();
    gam.clear();
    fam.clear();
    lam.clear();
  }

  void setVar(const std::string &id, Value *value) {
    for (size_t i = scopes.size() - 1; i >= 0; i--) {
      auto scope = scopes[i];
      if (scope.count(id)) {
        scope[id] = value;
      }
    }

    scopes.back()[id] = value;
  }

  Value *getVar(const std::string &id) {
    for (size_t i = scopes.size() - 1; i >= 0; i--) {
      auto scope = scopes[i];
      if (scope.count(id)) {
        return scope[id];
      }
    }

    return nullptr;
  }
};

} // namespace Compiler
} // namespace Plasmatum
