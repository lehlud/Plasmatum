#include "context.hh"

#include <llvm/IR/Verifier.h>
#include <llvm/Passes/PassBuilder.h>

#include <iostream>

#include "ast.hh"
#include "runtime_functions.hh"

Context::Context()
    : dataLayout(llvm::EngineBuilder().selectTarget()->createDataLayout()),
      llvmContext(), module("", llvmContext), builder(llvmContext) {
  module.setDataLayout(dataLayout);
  auto targetTriple = llvm::EngineBuilder().selectTarget()->getTargetTriple();
  module.setTargetTriple(targetTriple.str());

  initTypes();
  initFunctions();
}

void Context::store(llvm::Value *value, std::string name) {
  auto alloca = builder.CreateAlloca(value->getType());
  builder.CreateStore(value, alloca);
  namedValues[name] = {value->getType(), alloca};
}

void Context::createMain(std::vector<Stmt *> stmts) {
  auto functionType = llvm::FunctionType::get(builder.getVoidTy(), false);
  auto function = llvm::Function::Create(
      functionType, llvm::Function::ExternalLinkage, "main", module);
  auto functionBB = llvm::BasicBlock::Create(llvmContext, "", function);
  builder.SetInsertPoint(functionBB);

  for (auto stmt : stmts) {
    stmt->genCode(*this);
  }

  builder.CreateRetVoid();
}

void Context::initTypes() {
  numberType = builder.getDoubleTy();
  charType = builder.getInt32Ty();
  pointerType = llvm::PointerType::get(builder.getVoidTy(), 0);
  dummyFunctionType = llvm::FunctionType::get(builder.getInt8Ty(), false);
}

constexpr auto numPrint = "__builtin__num_print";
constexpr auto numPrintln = "__builtin__num_println";
constexpr auto strPrint = "__builtin__str_print";
constexpr auto strPrintln = "__builtin__str_println";
void Context::initFunctions() {
  auto numType = llvm::FunctionType::get(numberType, {numberType}, false);
  auto strType = llvm::FunctionType::get(numberType, {pointerType}, false);
  
  auto p = module.getOrInsertFunction(numPrint, numType);
  auto pln = module.getOrInsertFunction(numPrintln, numType);
  builtins[numPrint] = (llvm::Function *)p.getCallee();
  builtins[numPrintln] = (llvm::Function *)pln.getCallee();
  
  p = module.getOrInsertFunction(strPrint, strType);
  pln = module.getOrInsertFunction(strPrintln, strType);
  builtins[strPrint] = (llvm::Function *)p.getCallee();
  builtins[strPrintln] = (llvm::Function *)pln.getCallee();
}

void Context::optimizeIR() {
  llvm::PassBuilder passBuilder;

  llvm::ModuleAnalysisManager mam;
  llvm::CGSCCAnalysisManager gam;
  llvm::FunctionAnalysisManager fam;
  llvm::LoopAnalysisManager lam;

  passBuilder.registerModuleAnalyses(mam);
  passBuilder.registerCGSCCAnalyses(gam);
  passBuilder.registerFunctionAnalyses(fam);
  passBuilder.registerLoopAnalyses(lam);

  passBuilder.crossRegisterProxies(lam, fam, gam, mam);

  auto fpm = passBuilder.buildFunctionSimplificationPipeline(
      llvm::PassBuilder::OptimizationLevel::O3, llvm::ThinOrFullLTOPhase::None);

  for (auto &f : module.functions()) {
    if (!f.getBasicBlockList().size()) {
      continue;
    }
    fpm.run(f, fam);
  }

  mam.clear();
  gam.clear();
  fam.clear();
  lam.clear();
}

void Context::print() { module.print(llvm::errs(), nullptr); }

llvm::ExecutionEngine &Context::getExecutionEngine() {
  auto &result = *(llvm::EngineBuilder(std::unique_ptr<llvm::Module>(&module))
                       .setEngineKind(llvm::EngineKind::JIT)
                       .setOptLevel(llvm::CodeGenOpt::Aggressive)
                       .create());

  result.DisableSymbolSearching();

  auto numP = module.getFunction(numPrint);
  auto numPLn = module.getFunction(numPrintln);
  auto strP = module.getFunction(strPrint);
  auto strPln = module.getFunction(strPrintln);

  result.addGlobalMapping(numP, (void *)&num_print);
  result.addGlobalMapping(numPLn, (void *)&num_println);
  result.addGlobalMapping(strP, (void *)&str_print);
  result.addGlobalMapping(strPln, (void *)&str_println);

  result.finalizeObject();
  return result;
}