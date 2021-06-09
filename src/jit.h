#include <map>
#include <string>

#include <llvm/ADT/Hashing.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Passes/PassBuilder.h>

namespace Plasmatum {
namespace Compiler {

class Context {
private:
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
};

} // namespace Compiler
} // namespace Plasmatum