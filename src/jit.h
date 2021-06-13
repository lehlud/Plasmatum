#pragma once

#include <map>
#include <string>

#include <llvm/ADT/Hashing.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Passes/PassBuilder.h>

#include "error.h"

#define TYPE_INT    1
#define TYPE_FLOAT  1
#define TYPE_ARRAY  1
#define TYPE_FUNC   1
#define TYPE_STRUCT 1


namespace Plasmatum {
namespace Compiler {

class Context {
private:
  std::map<std::string, std::pair<llvm::Type *, llvm::Value *>> vars;

  llvm::ModuleAnalysisManager mam;
  llvm::CGSCCAnalysisManager gam;
  llvm::FunctionAnalysisManager fam;
  llvm::LoopAnalysisManager lam;
  llvm::FunctionPassManager fpm;

public:
  llvm::Module *mod;
  llvm::IRBuilder<> *builder;

  bool needsRef = false;

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

  void setVar(const std::string &id,
              const std::pair<llvm::Type *, llvm::Value *> &value) {
    if (vars.count(id))
      Error::compiler("'" + id + "' already defined");
    vars[id] = value;
  }

  std::pair<llvm::Type *, llvm::Value *> getVar(const std::string &id) {
    if (vars.count(id)) {
      return vars[id];
    } else {
      auto f = mod->getFunction(id);
      if (f) {
        return {f->getType(), f};
      }
    }

    Error::compiler("undefined reference to '" + id + "'");

    return {nullptr, nullptr};
  }
};

} // namespace Compiler
} // namespace Plasmatum
