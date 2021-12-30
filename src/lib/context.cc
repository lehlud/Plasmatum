#include "context.hh"

#include <llvm/IR/Verifier.h>
#include <llvm/Passes/PassBuilder.h>

#include <iostream>

#include "ast.hh"
#include "natives.hh"
#include "runtime_functions.hh"

Context::Context()
    : dataLayout(llvm::EngineBuilder().selectTarget()->createDataLayout()),
      llvmContext(), module("", llvmContext), builder(llvmContext) {
  module.setDataLayout(dataLayout);
  auto targetTriple = llvm::EngineBuilder().selectTarget()->getTargetTriple();
  module.setTargetTriple(targetTriple.str());

  initNatives();
  initTypes();
  initFunctions();
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

void Context::initNatives() {
  natives["add"] = &nativeAdd;
  natives["sub"] = &nativeSub;
  natives["mul"] = &nativeMul;
  natives["div"] = &nativeDiv;
}

void Context::initTypes() {
  pointerType = builder.getInt8PtrTy();
  classTypeNameType = builder.getInt8PtrTy();
  dummyFunctionType = llvm::FunctionType::get(builder.getInt8Ty(), false);

  nativeFloatType = llvm::Type::getFP128Ty(llvmContext);
  nativeIntType = builder.getInt128Ty();
  nativeCharType = builder.getInt32Ty();

  builtinIntType = createClassType("Int", {{"value", nativeIntType}});
  builtinFloatType = createClassType("Float", {{"value", nativeFloatType}});
}

constexpr auto numPrint = "__builtin__num_print";
constexpr auto numPrintln = "__builtin__num_println";
constexpr auto strPrint = "__builtin__str_print";
constexpr auto strPrintln = "__builtin__str_println";
void Context::initFunctions() {
  // auto numType = llvm::FunctionType::get(numberType, {numberType}, false);
  // auto strType = llvm::FunctionType::get(numberType, {pointerType}, false);

  // auto p = module.getOrInsertFunction(numPrint, numType);
  // auto pln = module.getOrInsertFunction(numPrintln, numType);
  // builtins[numPrint] = (llvm::Function *)p.getCallee();
  // builtins[numPrintln] = (llvm::Function *)pln.getCallee();

  // p = module.getOrInsertFunction(strPrint, strType);
  // pln = module.getOrInsertFunction(strPrintln, strType);
  // builtins[strPrint] = (llvm::Function *)p.getCallee();
  // builtins[strPrintln] = (llvm::Function *)pln.getCallee();
}

llvm::Type *Context::createClassType(std::string name,
                                     std::map<std::string, llvm::Type *> attrs) {
  assert(namedTypes.count(name) != 0);
  assert(attrs.size() > 0);

  std::vector<llvm::Type *> fields;
  std::map<std::string, unsigned long> indexedAttrs;
  unsigned long i = 0;
  for (auto &attr : attrs) {
    fields.push_back(attr.second);
    indexedAttrs[attr.first] = i++;
  }

  auto namePointer = builder.CreateGlobalStringPtr(name);
  auto classType = llvm::StructType::create(llvmContext, fields, name);
  namedTypes[name] = {namePointer, {classType, indexedAttrs}};
  return classType;
}

void Context::print() { module.print(llvm::errs(), nullptr); }

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
