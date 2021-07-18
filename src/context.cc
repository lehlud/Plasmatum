#include "context.hh"
#include "defines.hh"
#include "lib.hh"

#include <iostream>
#include <llvm/IR/Verifier.h>

#include <llvm/Support/TargetRegistry.h>

PlsmContext::PlsmContext()
    : dataLayout(llvm::EngineBuilder().selectTarget()->createDataLayout()),
      context(), module("", context), builder(context),
      i32Type(llvm::Type::getInt32Ty(context)),
      i64Type(llvm::Type::getInt64Ty(context)), intType(i64Type),
      charType(i32Type), floatingPointType(llvm::Type::getDoubleTy(context)),
      typeType(llvm::Type::getInt8Ty(context)),
      pointerType(llvm::Type::getInt8PtrTy(context)),
      plsmType(llvm::StructType::create(context, {typeType, pointerType},
                                        "plsm_val")),
      functionType(llvm::FunctionType::get(
          plsmType, {intType, plsmType->getPointerTo()}, false)),
      mainFunction(nullptr),
      freeFunction(llvm::Function::Create(
          llvm::FunctionType::get(llvm::Type::getVoidTy(context), {pointerType},
                                  false),
          llvm::Function::ExternalLinkage, "free", module)),
      mallocFunction(llvm::Function::Create(
          llvm::FunctionType::get(pointerType, {i64Type}, false),
          llvm::Function::ExternalLinkage, "malloc", module)),
      memcpyFunction(llvm::Function::Create(
          llvm::FunctionType::get(plsmType, {plsmType}, false),
          llvm::Function::ExternalLinkage, "__plsm_memcpy", module)),
      plsmFreeFunction(llvm::Function::Create(
          llvm::FunctionType::get(llvm::Type::getVoidTy(context), {plsmType},
                                  false),
          llvm::Function::ExternalLinkage, "__plsm_free", module)),
      getArgFunction(llvm::Function::Create(
          llvm::FunctionType::get(
              plsmType, {intType, plsmType->getPointerTo(), i64Type}, false),
          llvm::Function::ExternalLinkage, "__plsm_getarg", module)),
      logicalFunction(llvm::Function::Create(
          llvm::FunctionType::get(i64Type, {plsmType}, false),
          llvm::Function::ExternalLinkage, "__plsm_logical", module)),
      binExprFunctionType(
          llvm::FunctionType::get(plsmType, {plsmType, plsmType}, false)),
      addFunction(llvm::Function::Create(binExprFunctionType,
                                         llvm::Function::ExternalLinkage,
                                         "__plsm_add", module)),
      subFunction(llvm::Function::Create(binExprFunctionType,
                                         llvm::Function::ExternalLinkage,
                                         "__plsm_sub", module)),
      mulFunction(llvm::Function::Create(binExprFunctionType,
                                         llvm::Function::ExternalLinkage,
                                         "__plsm_mul", module)),
      divFunction(llvm::Function::Create(binExprFunctionType,
                                         llvm::Function::ExternalLinkage,
                                         "__plsm_div", module)),
      modFunction(llvm::Function::Create(binExprFunctionType,
                                         llvm::Function::ExternalLinkage,
                                         "__plsm_mod", module)),
      eqFunction(llvm::Function::Create(binExprFunctionType,
                                        llvm::Function::ExternalLinkage,
                                        "__plsm_eq", module)),
      neFunction(llvm::Function::Create(binExprFunctionType,
                                        llvm::Function::ExternalLinkage,
                                        "__plsm_ne", module)),
      gtFunction(llvm::Function::Create(binExprFunctionType,
                                        llvm::Function::ExternalLinkage,
                                        "__plsm_gt", module)),
      ltFunction(llvm::Function::Create(binExprFunctionType,
                                        llvm::Function::ExternalLinkage,
                                        "__plsm_lt", module)),
      geFunction(llvm::Function::Create(binExprFunctionType,
                                        llvm::Function::ExternalLinkage,
                                        "__plsm_ge", module)),
      leFunction(llvm::Function::Create(binExprFunctionType,
                                        llvm::Function::ExternalLinkage,
                                        "__plsm_le", module)),
      functions(), variableScopes() {
  module.setDataLayout(dataLayout);
  auto targetTriple = llvm::EngineBuilder().selectTarget()->getTargetTriple();
  module.setTargetTriple(targetTriple.str());

  std::vector<std::string> builtIns = {
      "print",
      "println",
  };
  for (auto &function : builtIns) {
    auto tmp =
        llvm::Function::Create(functionType, llvm::Function::ExternalLinkage,
                               "__plsm_" + function, module);
    functions[function] = tmp;
  }
}

void PlsmContext::initNewScope() {
  auto scope = std::pair<std::map<std::string, llvm::Value *>,
                         std::vector<llvm::Value *>>();
  variableScopes.push_back(scope);
  disposalDepth += 1;
}

void PlsmContext::disposeLastScope() {
  if (!variableScopes.size())
    return;

  auto scope = variableScopes.back();
  scope.first.clear();
  scope.second.clear();

  variableScopes.pop_back();
  disposalDepth -= 1;
}

void PlsmContext::freeLastScope() { freeScope(variableScopes.size() - 1); }

void PlsmContext::freeScope(size_t index) {
  if (index >= variableScopes.size())
    return;

  auto scope = variableScopes[index];
  for (auto &value : scope.second) {
    createPlsmFree(value);
  }
}

void PlsmContext::freeMarkedScopes() {
  size_t i = variableScopes.size();
  while (i > variableScopes.size() - disposalDepth) {
    i -= 1;
    freeScope(i);
  }
}

void PlsmContext::setVariable(const std::string &id, llvm::Value *value) {
  if (!variableScopes.size())
    return;

  for (size_t i = variableScopes.size(); i > 0; i--) {
    auto scope = variableScopes[i - 1];
    if (scope.first.count(id)) {
      scope.first[id] = value;
      return;
    }
  }

  variableScopes.back().first[id] = value;
}

llvm::Value *PlsmContext::getVariable(const std::string &id) {
  for (size_t i = variableScopes.size(); i > 0; i--) {
    auto scope = variableScopes[i - 1];
    if (scope.first.count(id))
      return scope.first[id];
  }

  return nullptr;
}

llvm::Constant *PlsmContext::getI64(int64_t value) {
  return llvm::ConstantInt::get(i64Type, value);
}

llvm::Constant *PlsmContext::getInt(int64_t value) {
  return llvm::ConstantInt::get(intType, value);
}

llvm::Constant *PlsmContext::getFloat(double value) {
  return llvm::ConstantFP::get(floatingPointType, value);
}

int64_t PlsmContext::getTypeSize(llvm::Type *type) {
  return dataLayout.getTypeAllocSize(type);
}

void PlsmContext::addToPlsmPointers(llvm::Value *value) {
  variableScopes.back().second.push_back(value);
}

llvm::Value *PlsmContext::getPlsmValue(int8_t type, llvm::Value *valuePointer) {
  auto result = (llvm::Value *)llvm::UndefValue::get(plsmType);

  auto typeValue = llvm::ConstantInt::get(typeType, type);
  result = builder.CreateInsertValue(result, typeValue, {0});

  valuePointer = builder.CreatePointerCast(valuePointer, pointerType);
  result = builder.CreateInsertValue(result, valuePointer, {1});

  addToPlsmPointers(result);

  return result;
}

llvm::Value *PlsmContext::getPlsmNull() {
  return llvm::ConstantAggregateZero::get(plsmType);
}

llvm::Value *PlsmContext::getPlsmInt(int64_t value) {
  auto ptr = createMalloc(intType);

  builder.CreateStore(getInt(value), ptr);

  return getPlsmValue(TYPE_INT, ptr);
}

llvm::Value *PlsmContext::getPlsmFloat(double value) {
  auto ptr = createMalloc(floatingPointType);

  builder.CreateStore(getFloat(value), ptr);

  return getPlsmValue(TYPE_FLOAT, ptr);
}

llvm::Value *PlsmContext::getPlsmString(const std::u32string &string) {
  auto stringPtr = createMalloc(charType, string.size() + 1);
  stringPtr = builder.CreatePointerCast(stringPtr, charType->getPointerTo());

  for (size_t i = 0; i < string.size() + 1; i++) {
    auto ep = builder.CreateGEP(stringPtr, getI64(i));
    builder.CreateStore(llvm::ConstantInt::get(charType, string[i]), ep);
  }

  auto ptr = createMalloc(charType->getPointerTo());
  builder.CreateStore(stringPtr, ptr);

  return getPlsmValue(TYPE_STRING, ptr);
}

llvm::Value *PlsmContext::getTypeFromPlsm(llvm::Value *plsmValue) {
  auto alloca = createMalloc(plsmType);
  builder.CreateStore(plsmValue, alloca);

  auto resultEP = builder.CreateStructGEP(alloca, 0);
  resultEP = builder.CreatePointerCast(resultEP, typeType->getPointerTo());

  auto result = builder.CreateLoad(typeType, resultEP);

  createFree(alloca);

  return result;
}

llvm::Value *PlsmContext::getValueFromPlsm(llvm::Value *value,
                                           llvm::Type *type) {
  auto pointer = builder.CreateExtractValue(value, {1});
  pointer = builder.CreatePointerCast(pointer, type->getPointerTo());
  return builder.CreateLoad(type, pointer);
}

llvm::Value *PlsmContext::getPlsmLogicalValue(llvm::Value *value) {
  return builder.CreateCall(logicalFunction, {value});
}

llvm::Function *PlsmContext::getMain() {
  if (mainFunction)
    return mainFunction;
  else if (!functions.count("main"))
    return nullptr;

  auto retT = llvm::IntegerType::getInt8Ty(context);
  auto ft = llvm::FunctionType::get(retT, {i32Type}, false);
  mainFunction = llvm::Function::Create(ft, llvm::Function::ExternalLinkage,
                                        "main", module);

  auto bb = llvm::BasicBlock::Create(context, "", mainFunction);
  builder.SetInsertPoint(bb);

  initNewScope();

  auto argc = builder.CreateIntCast(mainFunction->getArg(0), intType, false);

  auto alloca = createMalloc(intType);
  builder.CreateStore(argc, alloca);
  auto arg = getPlsmValue(TYPE_INT, alloca);

  auto args = createMalloc(plsmType);
  builder.CreateStore(arg, args);

  auto callee = functions["main"];
  auto call = builder.CreateCall(callee, {getInt(1), args});

  freeMarkedScopes();

  builder.CreateRet(llvm::ConstantInt::get(retT, 42));

  disposeLastScope();

  return mainFunction;
}

llvm::Value *PlsmContext::createFree(llvm::Value *pointer) {
  pointer = builder.CreatePointerCast(pointer, pointerType);
  return builder.CreateCall(freeFunction, {pointer});
}

llvm::Value *PlsmContext::createPlsmFree(llvm::Value *value) {
  return builder.CreateCall(plsmFreeFunction, {value});
}

llvm::Value *PlsmContext::createMalloc(llvm::Type *resultType,
                                       int64_t numElements) {
  auto arg = getI64(getTypeSize(resultType) * numElements);

  auto result = (llvm::Value *)builder.CreateCall(mallocFunction, {arg});

  return builder.CreatePointerCast(result, resultType->getPointerTo());
}

llvm::Value *PlsmContext::createMemCpy(llvm::Value *value,
                                       bool addToPlsmPointers) {
  auto result = builder.CreateCall(memcpyFunction, {value});
  if (addToPlsmPointers)
    this->addToPlsmPointers(result);
  return result;
}

llvm::Value *PlsmContext::createBinExpr(llvm::Function *f, llvm::Value *left,
                                        llvm::Value *right) {
  auto result = builder.CreateCall(f, {left, right});
  addToPlsmPointers(result);
  return result;
}

llvm::Value *PlsmContext::createAdd(llvm::Value *left, llvm::Value *right) {
  return createBinExpr(addFunction, left, right);
}

llvm::Value *PlsmContext::createSub(llvm::Value *left, llvm::Value *right) {
  return createBinExpr(subFunction, left, right);
}

llvm::Value *PlsmContext::createMul(llvm::Value *left, llvm::Value *right) {
  return createBinExpr(mulFunction, left, right);
}

llvm::Value *PlsmContext::createDiv(llvm::Value *left, llvm::Value *right) {
  return createBinExpr(divFunction, left, right);
}

llvm::Value *PlsmContext::createMod(llvm::Value *left, llvm::Value *right) {
  return createBinExpr(modFunction, left, right);
}

llvm::Value *PlsmContext::createEq(llvm::Value *left, llvm::Value *right) {
  return createBinExpr(eqFunction, left, right);
}

llvm::Value *PlsmContext::createNE(llvm::Value *left, llvm::Value *right) {
  return createBinExpr(neFunction, left, right);
}

llvm::Value *PlsmContext::createGT(llvm::Value *left, llvm::Value *right) {
  return createBinExpr(gtFunction, left, right);
}

llvm::Value *PlsmContext::createLT(llvm::Value *left, llvm::Value *right) {
  return createBinExpr(ltFunction, left, right);
}

llvm::Value *PlsmContext::createGE(llvm::Value *left, llvm::Value *right) {
  return createBinExpr(geFunction, left, right);
}

llvm::Value *PlsmContext::createLE(llvm::Value *left, llvm::Value *right) {
  return createBinExpr(leFunction, left, right);
}

llvm::Value *PlsmContext::createVariableLoad(const std::string &id) {
  auto result = getVariable(id);
  return builder.CreateLoad(plsmType, result);
}

llvm::Value *PlsmContext::createRet(llvm::Value *value) {
  auto result = builder.CreateCall(memcpyFunction, {value});
  freeMarkedScopes();
  return builder.CreateRet(result);
}

llvm::Value *PlsmContext::createPlsmFunction(const std::string &id,
                                             std::vector<std::string> &args,
                                             std::vector<Stmt *> &body) {
  auto f = llvm::Function::Create(functionType, llvm::Function::PrivateLinkage,
                                  "", module);

  if (id.size() > 0)
    functions[id] = f;

  auto count = f->getArg(0);
  auto callArgs = f->getArg(1);

  auto bb = llvm::BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  initNewScope();

  for (size_t i = 0; i < args.size(); i++) {
    auto arg = builder.CreateCall(getArgFunction, {count, callArgs, getI64(i)});
    arg = builder.CreateCall(memcpyFunction, {arg});
    variableScopes.back().second.push_back(arg);

    auto alloca = builder.CreateAlloca(plsmType);
    builder.CreateStore(arg, alloca);

    setVariable(args[i], alloca);
  }

  for (auto &stmt : body) {
    stmt->genCode(*this);
  }

  disposeLastScope();

  return f;
}

llvm::Value *PlsmContext::createPlsmCall(const std::string &id,
                                         std::vector<Expr *> args) {
  auto f = functions[id];
  if (!f) {
    std::cout << "unable to find function '" << id << "'" << std::endl;
  }

  auto argCount = llvm::ConstantInt::get(intType, args.size());
  std::vector<llvm::Value *> callArgs = {argCount};

  llvm::Value *array = nullptr;

  if (args.size() > 0) {
    auto indexType = llvm::Type::getInt64Ty(context);

    array = createMalloc(plsmType, args.size());

    for (size_t i = 0; i < args.size(); i++) {
      auto idx = llvm::ConstantInt::get(indexType, i);
      auto ptr = builder.CreateGEP(array, idx);

      auto value = args[i]->genCode(*this);

      builder.CreateStore(value, ptr);
    }

    callArgs.push_back(array);
  } else {
    auto tmp = llvm::ConstantPointerNull::get(plsmType->getPointerTo());
    callArgs.push_back(tmp);
  }

  auto result = builder.CreateCall(f, callArgs);

  if (array)
    createFree(array);

  return result;
}

llvm::Value *PlsmContext::createPlsmConditional(Expr *condExpr, Expr *trueExpr,
                                                Expr *falseExpr) {
  auto condV = condExpr->genCode(*this);
  condV = getPlsmLogicalValue(condV);

  auto f = builder.GetInsertBlock()->getParent();

  auto trueBB = llvm::BasicBlock::Create(context, "", f);
  auto falseBB = llvm::BasicBlock::Create(context, "", f);
  auto mergeBB = llvm::BasicBlock::Create(context, "", f);

  builder.CreateCondBr(condV, trueBB, falseBB);

  builder.SetInsertPoint(trueBB);
  initNewScope();
  auto trueV = trueExpr->genCode(*this);
  trueV = createMemCpy(trueV, false);
  freeLastScope();
  disposeLastScope();
  builder.CreateBr(mergeBB);

  builder.SetInsertPoint(falseBB);
  initNewScope();
  auto falseV = falseExpr->genCode(*this);
  falseV = createMemCpy(falseV, false);
  freeLastScope();
  disposeLastScope();
  builder.CreateBr(mergeBB);

  builder.SetInsertPoint(mergeBB);

  auto phiNode = builder.CreatePHI(plsmType, 2);
  phiNode->addIncoming(trueV, trueBB);
  phiNode->addIncoming(falseV, falseBB);

  addToPlsmPointers(phiNode);

  return phiNode;
}

llvm::ExecutionEngine &PlsmContext::getExecutionEngine() {
  auto &result = *(llvm::EngineBuilder(std::unique_ptr<llvm::Module>(&module))
                       .setEngineKind(llvm::EngineKind::JIT)
                       .create());

  result.DisableSymbolSearching();

  result.addGlobalMapping(freeFunction, (void *)&free);
  result.addGlobalMapping(mallocFunction, (void *)&malloc);

  result.addGlobalMapping(memcpyFunction, (void *)&__plsm_memcpy);
  result.addGlobalMapping(plsmFreeFunction, (void *)&__plsm_free);

  result.addGlobalMapping(getArgFunction, (void *)&__plsm_getarg);
  result.addGlobalMapping(logicalFunction, (void *)&__plsm_logical);

  result.addGlobalMapping(addFunction, (void *)&__plsm_add);
  result.addGlobalMapping(subFunction, (void *)&__plsm_sub);
  result.addGlobalMapping(mulFunction, (void *)&__plsm_mul);
  result.addGlobalMapping(divFunction, (void *)&__plsm_div);
  result.addGlobalMapping(modFunction, (void *)&__plsm_mod);
  result.addGlobalMapping(eqFunction, (void *)&__plsm_eq);
  result.addGlobalMapping(neFunction, (void *)&__plsm_ne);
  result.addGlobalMapping(gtFunction, (void *)&__plsm_gt);
  result.addGlobalMapping(ltFunction, (void *)&__plsm_lt);
  result.addGlobalMapping(geFunction, (void *)&__plsm_ge);
  result.addGlobalMapping(leFunction, (void *)&__plsm_le);

  auto pf = module.getFunction("__plsm_print");
  auto plf = module.getFunction("__plsm_println");
  result.addGlobalMapping(pf, (void *)&__plsm_print);
  result.addGlobalMapping(plf, (void *)&__plsm_println);

  result.finalizeObject();
  return result;
}

void PlsmContext::printLLVMIR() { module.print(llvm::errs(), nullptr); }

void PlsmContext::optimize() {
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
    if (!f.getBasicBlockList().size())
      continue;
    fpm.run(f, fam);
  }

  mam.clear();
  gam.clear();
  fam.clear();
  lam.clear();
}
