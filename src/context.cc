#include "context.hh"
#include "defines.hh"
#include "lib.hh"

#include <iostream>
#include <llvm/IR/Verifier.h>

PlsmContext::PlsmContext()
    : dataLayout(llvm::EngineBuilder().selectTarget()->createDataLayout()),
      context(), module("", context), builder(context),
      i1Type(llvm::Type::getInt1Ty(context)),
      i32Type(llvm::Type::getInt32Ty(context)),
      i64Type(llvm::Type::getInt64Ty(context)), intType(i64Type),
      charType(i32Type), floatingPointType(llvm::Type::getDoubleTy(context)),
      typeType(llvm::Type::getInt8Ty(context)),
      pointerType(llvm::Type::getInt8PtrTy(context)),
      plsmType(llvm::StructType::create(context, {typeType, pointerType},
                                        "plsm_val")),
      functionType(llvm::FunctionType::get(
          plsmType, {intType, plsmType->getPointerTo()}, false)),
      logicalFunctionType(llvm::FunctionType::get(i1Type, {plsmType}, false)),
      binExprFunctionType(
          llvm::FunctionType::get(plsmType, {plsmType, plsmType}, false)),
      binExprMatrixType(llvm::ArrayType::get(
          llvm::ArrayType::get(binExprFunctionType->getPointerTo(), 3), 3)),
      addFuncs(nullptr), subFuncs(nullptr), mulFuncs(nullptr),
      divFuncs(nullptr), logicalFuncs(nullptr), functions(), variableScopes(),
      mainFunction(nullptr), getArgFunction(nullptr) {
  module.setDataLayout(dataLayout);

  std::vector<std::string> builtIns = {
      "print",
      "println",
  };
  for (auto &function : builtIns) {
    auto tmp = llvm::Function::Create(
        functionType, llvm::Function::ExternalLinkage, function, module);
    functions[function] = tmp;
  }

  initAllocs();
  initLogicals();
  initBinExprs();
  initGetArgFunction();

  llvm::PassBuilder passBuilder;

  passBuilder.registerModuleAnalyses(mam);
  passBuilder.registerCGSCCAnalyses(gam);
  passBuilder.registerFunctionAnalyses(fam);
  passBuilder.registerLoopAnalyses(lam);

  passBuilder.crossRegisterProxies(lam, fam, gam, mam);

  mpm = passBuilder.buildModuleOptimizationPipeline(
      llvm::PassBuilder::OptimizationLevel::O3, false);
}

PlsmContext::~PlsmContext() { disposeOptimizations(); }

void PlsmContext::initAllocs() {
  auto freeFunctionType = llvm::FunctionType::get(
      llvm::Type::getVoidTy(context), {pointerType}, false);
  freeFunction = llvm::Function::Create(
      freeFunctionType, llvm::Function::ExternalLinkage, "free", module);

  auto mallocFunctionType =
      llvm::FunctionType::get(pointerType, {i64Type}, false);
  mallocFunction = llvm::Function::Create(
      mallocFunctionType, llvm::Function::ExternalLinkage, "malloc", module);
}

void PlsmContext::initLogicals() {
  auto nullLogical = getNullLogical();
  auto intLogical = getIntLogical();
  auto floatLogical = getFloatLogical();

  std::vector<llvm::Constant *> funcs = {nullLogical, intLogical, floatLogical};
  auto arrT =
      llvm::ArrayType::get(logicalFunctionType->getPointerTo(), funcs.size());
  auto arr = llvm::ConstantArray::get(arrT, funcs);

  logicalFuncs = new llvm::GlobalVariable(
      module, arrT, true, llvm::GlobalVariable::ExternalLinkage, arr);
}

void PlsmContext::initGetArgFunction() {
  if (getArgFunction)
    return;

  auto ft = llvm::FunctionType::get(
      plsmType, {intType, plsmType->getPointerTo(), intType}, false);

  getArgFunction = llvm::Function::Create(ft, llvm::Function::ExternalLinkage,
                                          "getarg", module);

  auto count = getArgFunction->getArg(0);
  auto args = getArgFunction->getArg(1);
  auto index = getArgFunction->getArg(2);

  auto bb = llvm::BasicBlock::Create(context, "", getArgFunction);

  builder.SetInsertPoint(bb);

  auto condition = builder.CreateICmpUGE(index, count);

  auto ifBB = llvm::BasicBlock::Create(context, "", getArgFunction);
  auto elseBB = llvm::BasicBlock::Create(context, "", getArgFunction);

  builder.CreateCondBr(condition, ifBB, elseBB);

  builder.SetInsertPoint(ifBB);
  builder.CreateRet(getPlsmNull());

  builder.SetInsertPoint(elseBB);

  auto resultEP = builder.CreateGEP(args, index);
  auto result = builder.CreateLoad(plsmType, resultEP);
  builder.CreateRet(result);
}

void PlsmContext::initBinExprs() {
  initAdds();
  initSubs();
  initMuls();
  initDivs();
}

void PlsmContext::initAdds() {
  auto nullf = getNullBinExpr();
  auto intf = getIntAdd();
  auto floatf = getFloatAdd();

  addFuncs =
      new llvm::GlobalVariable(module, binExprMatrixType, true,
                               llvm::GlobalValue::PrivateLinkage, nullptr);
}

void PlsmContext::initSubs() {}

void PlsmContext::initMuls() {}

void PlsmContext::initDivs() {}

llvm::Function *PlsmContext::getNullBinExpr() {
  auto name = "null_binexpr";
  llvm::Function *f = module.getFunction(name);
  if (f)
    return f;

  auto nullAdd = llvm::Function::Create(
      binExprFunctionType, llvm::Function::PrivateLinkage, name, module);

  auto bb = llvm::BasicBlock::Create(context, "", nullAdd);
  builder.SetInsertPoint(bb);

  builder.CreateRet(getPlsmNull());

  return f;
}

llvm::Function *PlsmContext::getIntAdd() {
  auto name = "int_add";
  llvm::Function *f = module.getFunction(name);
  if (f)
    return f;

  f = llvm::Function::Create(binExprFunctionType,
                             llvm::Function::PrivateLinkage, name, module);

  auto bb = llvm::BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  auto left = getIntFromPlsm(f->getArg(0));
  auto right = getIntFromPlsm(f->getArg(1));

  auto result = builder.CreateAdd(left, right);

  auto resultPtr = createMalloc(intType);
  builder.CreateStore(result, resultPtr);

  builder.CreateRet(getPlsmValue(TYPE_INT, resultPtr));

  return f;
}

llvm::Function *PlsmContext::getIntSub() {
  auto name = "int_sub";
  llvm::Function *f = module.getFunction(name);
  if (f)
    return f;

  f = llvm::Function::Create(binExprFunctionType,
                             llvm::Function::PrivateLinkage, name, module);

  auto bb = llvm::BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  auto left = getIntFromPlsm(f->getArg(0));
  auto right = getIntFromPlsm(f->getArg(1));

  auto result = builder.CreateSub(left, right);

  auto resultPtr = createMalloc(intType);
  builder.CreateStore(result, resultPtr);

  builder.CreateRet(getPlsmValue(TYPE_INT, resultPtr));

  return f;
}

llvm::Function *PlsmContext::getIntMul() {
  auto name = "int_mul";
  llvm::Function *f = module.getFunction(name);
  if (f)
    return f;

  f = llvm::Function::Create(binExprFunctionType,
                             llvm::Function::PrivateLinkage, name, module);

  auto bb = llvm::BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  auto left = getIntFromPlsm(f->getArg(0));
  auto right = getIntFromPlsm(f->getArg(1));

  auto result = builder.CreateMul(left, right);

  auto resultPtr = createMalloc(intType);
  builder.CreateStore(result, resultPtr);

  builder.CreateRet(getPlsmValue(TYPE_INT, resultPtr));

  return f;
}

llvm::Function *PlsmContext::getIntDiv() {
  auto name = "int_div";
  llvm::Function *f = module.getFunction(name);
  if (f)
    return f;

  f = llvm::Function::Create(binExprFunctionType,
                             llvm::Function::PrivateLinkage, name, module);

  auto bb = llvm::BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  auto left = getIntFromPlsm(f->getArg(0));
  auto right = getIntFromPlsm(f->getArg(1));

  auto result = builder.CreateSDiv(left, right);

  auto resultPtr = createMalloc(intType);
  builder.CreateStore(result, resultPtr);

  builder.CreateRet(getPlsmValue(TYPE_INT, resultPtr));

  return f;
}

llvm::Function *PlsmContext::getFloatAdd() {
  auto name = "float_add";
  llvm::Function *f = module.getFunction(name);
  if (f)
    return f;

  f = llvm::Function::Create(binExprFunctionType,
                             llvm::Function::PrivateLinkage, name, module);

  auto bb = llvm::BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  auto left = getFloatFromPlsm(f->getArg(0));
  auto right = getFloatFromPlsm(f->getArg(1));

  auto result = builder.CreateFAdd(left, right);

  auto resultPtr = createMalloc(floatingPointType);
  builder.CreateStore(result, resultPtr);

  builder.CreateRet(getPlsmValue(TYPE_INT, resultPtr));

  return f;
}

llvm::Function *PlsmContext::getFloatSub() {
  auto name = "float_sub";
  llvm::Function *f = module.getFunction(name);
  if (f)
    return f;

  f = llvm::Function::Create(binExprFunctionType,
                             llvm::Function::PrivateLinkage, name, module);

  auto bb = llvm::BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  auto left = getFloatFromPlsm(f->getArg(0));
  auto right = getFloatFromPlsm(f->getArg(1));

  auto result = builder.CreateFSub(left, right);

  auto resultPtr = createMalloc(floatingPointType);
  builder.CreateStore(result, resultPtr);

  builder.CreateRet(getPlsmValue(TYPE_INT, resultPtr));

  return f;
}

llvm::Function *PlsmContext::getFloatMul() {
  auto name = "float_mul";
  llvm::Function *f = module.getFunction(name);
  if (f)
    return f;

  f = llvm::Function::Create(binExprFunctionType,
                             llvm::Function::PrivateLinkage, name, module);

  auto bb = llvm::BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  auto left = getFloatFromPlsm(f->getArg(0));
  auto right = getFloatFromPlsm(f->getArg(1));

  auto result = builder.CreateFMul(left, right);

  auto resultPtr = createMalloc(floatingPointType);
  builder.CreateStore(result, resultPtr);

  builder.CreateRet(getPlsmValue(TYPE_INT, resultPtr));

  return f;
}

llvm::Function *PlsmContext::getFloatDiv() {
  auto name = "float_div";
  llvm::Function *f = module.getFunction(name);
  if (f)
    return f;

  f = llvm::Function::Create(binExprFunctionType,
                             llvm::Function::PrivateLinkage, name, module);

  auto bb = llvm::BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  auto left = getFloatFromPlsm(f->getArg(0));
  auto right = getFloatFromPlsm(f->getArg(1));

  auto result = builder.CreateFDiv(left, right);

  auto resultPtr = createMalloc(floatingPointType);
  builder.CreateStore(result, resultPtr);

  builder.CreateRet(getPlsmValue(TYPE_INT, resultPtr));

  return f;
}

void PlsmContext::initNewScope() {
  auto scope = std::map<std::string, llvm::Value *>();
  variableScopes.push_back(scope);
  disposalDepth += 1;
}

void PlsmContext::disposeLastScope() {
  if (!variableScopes.size())
    return;

  for (auto &pair : variableScopes.back()) {
    auto ptr = builder.CreatePointerCast(pair.second, pointerType);
    builder.CreateCall(freeFunction, {ptr});
  }

  variableScopes.pop_back();
  disposalDepth -= 1;
}

void PlsmContext::disposeMarkedScopes() {
  std::cout << "disposing scopes" << std::endl;
  while (disposalDepth > 0 && variableScopes.size()) {
    std::cout << "disposing" << std::endl;
    disposeLastScope();
  }
  disposalDepth = 0;
}

void PlsmContext::setVariable(const std::string &id, llvm::Value *value) {
  if (!variableScopes.size())
    return;

  for (size_t i = variableScopes.size(); i > 0; i--) {
    auto scope = variableScopes[i - 1];
    if (scope.count(id)) {
      scope[id] = value;
      return;
    }
  }

  variableScopes.back()[id] = value;
}

llvm::Value *PlsmContext::getVariable(const std::string &id) {
  for (size_t i = variableScopes.size(); i > 0; i--) {
    auto scope = variableScopes[i - 1];
    if (scope.count(id))
      return scope[id];
  }

  return nullptr;
}

llvm::Function *PlsmContext::getNullLogical() {
  auto name = "null_logical";

  llvm::Function *f = module.getFunction(name);
  if (f)
    return f;

  f = llvm::Function::Create(logicalFunctionType,
                             llvm::Function::ExternalLinkage, name, module);

  auto bb = llvm::BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  auto result = llvm::ConstantInt::get(i1Type, 0);
  builder.CreateRet(result);

  return f;
}

llvm::Function *PlsmContext::getIntLogical() {
  auto name = "int_logical";

  llvm::Function *f = module.getFunction(name);
  if (f)
    return f;

  f = llvm::Function::Create(logicalFunctionType,
                             llvm::Function::ExternalLinkage, name, module);

  auto bb = llvm::BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  auto alloca = builder.CreateAlloca(plsmType);
  builder.CreateStore(f->getArg(0), alloca);

  auto valuePtrEP = builder.CreateStructGEP(alloca, 1);
  auto valuePtr = (llvm::Value *)builder.CreateLoad(pointerType, valuePtrEP);

  valuePtr = builder.CreatePointerCast(valuePtr, intType->getPointerTo());
  auto intValue = builder.CreateLoad(intType, valuePtr);

  auto result = builder.CreateICmpNE(intValue, getInt(0));
  builder.CreateRet(result);

  return f;
}

llvm::Function *PlsmContext::getFloatLogical() {
  auto name = "float_logical";

  llvm::Function *f = module.getFunction(name);
  if (f)
    return f;

  f = llvm::Function::Create(logicalFunctionType,
                             llvm::Function::ExternalLinkage, name, module);

  auto bb = llvm::BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  auto alloca = builder.CreateAlloca(plsmType);
  builder.CreateStore(f->getArg(0), alloca);

  auto valuePtrEP = builder.CreateStructGEP(alloca, 1);
  auto valuePtr = (llvm::Value *)builder.CreateLoad(pointerType, valuePtrEP);

  valuePtr =
      builder.CreatePointerCast(valuePtr, floatingPointType->getPointerTo());
  auto floatValue = builder.CreateLoad(floatingPointType, valuePtr);

  auto result = builder.CreateFCmpUNE(floatValue, getFloat(0));
  builder.CreateRet(result);

  return f;
}

llvm::Function *PlsmContext::getStringLogical() { return nullptr; }

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

llvm::Value *PlsmContext::getPlsmValue(int8_t type, llvm::Value *valuePointer) {
  auto alloca = builder.CreateAlloca(plsmType);

  auto typeEP = builder.CreateStructGEP(alloca, 0);
  auto valuePointerEP = builder.CreateStructGEP(alloca, 1);

  valuePointer = builder.CreatePointerCast(valuePointer, pointerType);

  builder.CreateStore(llvm::ConstantInt::get(typeType, type), typeEP);
  builder.CreateStore(valuePointer, valuePointerEP);

  return builder.CreateLoad(plsmType, alloca);
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
  std::vector<llvm::Constant *> chars;
  for (auto &c : string) {
    chars.push_back(llvm::ConstantInt::get(charType, c));
  }

  chars.push_back(llvm::ConstantInt::get(charType, 0));

  auto arrT = llvm::ArrayType::get(charType, string.size() + 1);
  auto str = llvm::ConstantArray::get(arrT, chars);

  auto ptr = createMalloc(arrT);

  builder.CreateStore(str, ptr);

  return getPlsmValue(TYPE_STRING, ptr);
}

llvm::Value *PlsmContext::getIntFromPlsm(llvm::Value *plsmValue) {
  auto alloca = createMalloc(plsmType);
  builder.CreateStore(plsmValue, alloca);

  auto resultEP = builder.CreateStructGEP(alloca, 1);
  resultEP = builder.CreatePointerCast(resultEP, intType->getPointerTo());

  auto result = builder.CreateLoad(intType, resultEP);

  createFree(alloca);

  return result;
}

llvm::Value *PlsmContext::getFloatFromPlsm(llvm::Value *plsmValue) {
  auto alloca = createMalloc(plsmType);
  builder.CreateStore(plsmValue, alloca);

  auto resultEP = builder.CreateStructGEP(alloca, 1);
  resultEP =
      builder.CreatePointerCast(resultEP, floatingPointType->getPointerTo());

  auto result = builder.CreateLoad(floatingPointType, resultEP);

  createFree(alloca);

  return result;
}

llvm::Value *PlsmContext::getStringFromPlsm(llvm::Value *plsmValue) {
  auto alloca = createMalloc(plsmType);
  builder.CreateStore(plsmValue, alloca);

  auto resultEP = builder.CreateStructGEP(alloca, 1);
  resultEP = builder.CreatePointerCast(
      resultEP, charType->getPointerTo()->getPointerTo());

  auto result = builder.CreateLoad(charType->getPointerTo(), resultEP);

  createFree(alloca);

  return result;
}

llvm::Value *PlsmContext::getPlsmLogicalValue(llvm::Value *value) {
  auto alloca = builder.CreateAlloca(plsmType);
  builder.CreateStore(value, alloca);
  auto typeEP = builder.CreateStructGEP(alloca, 0);
  llvm::Value *logicalIdx = builder.CreateLoad(typeType, typeEP);
  logicalIdx = builder.CreateIntCast(logicalIdx, i64Type, false);

  auto logicalFuncEP = builder.CreateGEP(logicalFuncs, {getI64(0), logicalIdx});
  auto tmpFunc =
      builder.CreateLoad(logicalFunctionType->getPointerTo(), logicalFuncEP);

  auto func = llvm::FunctionCallee(logicalFunctionType, tmpFunc);
  return builder.CreateCall(func, {value});
}

llvm::Function *PlsmContext::getMain() {
  if (mainFunction)
    return mainFunction;
  else if (!functions.count("main"))
    return nullptr;

  auto retT = llvm::IntegerType::getInt8Ty(context);
  auto ft = llvm::FunctionType::get(retT, false);
  auto f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "main",
                                  module);

  auto bb = llvm::BasicBlock::Create(context, "", f);

  builder.SetInsertPoint(bb);

  auto callExpr = new CallExpr("main", {});
  callExpr->genCode(*this);

  builder.CreateRet(llvm::ConstantInt::get(retT, 42));

  return f;
}

llvm::Value *PlsmContext::createFree(llvm::Value *pointer) {
  return builder.CreateCall(freeFunction, {pointer});
}

llvm::Value *PlsmContext::createMalloc(llvm::Type *resultType,
                                       int64_t numElements) {
  auto arg = getI64(getTypeSize(resultType) * numElements);

  auto result = (llvm::Value *)builder.CreateCall(mallocFunction, {arg});

  return builder.CreatePointerCast(result, resultType->getPointerTo());
}

llvm::Value *PlsmContext::createRet(llvm::Value *value) {
  disposeMarkedScopes();
  return builder.CreateRet(value);
}

llvm::Value *PlsmContext::createPlsmFunction(const std::string &id,
                                             std::vector<std::string> &args,
                                             std::vector<Stmt *> &body) {
  auto f = llvm::Function::Create(functionType, llvm::Function::PrivateLinkage,
                                  "", module);

  auto count = f->getArg(0);
  auto callArgs = f->getArg(1);

  auto bb = llvm::BasicBlock::Create(context, "", f);
  builder.SetInsertPoint(bb);

  initNewScope();

  for (size_t i = 0; i < args.size(); i++) {
    auto arg = builder.CreateCall(getArgFunction, {count, callArgs, getInt(i)});
    auto alloca = createMalloc(plsmType);
    builder.CreateStore(arg, alloca);
    setVariable(args[i], alloca);
  }

  for (auto &stmt : body) {
    stmt->genCode(*this);
  }

  // if (llvm::verifyFunction(*f)) {
  //   disposeLastScope();
  //   builder.CreateRet(getPlsmNull());
  // }

  // disposeLastScope();

  if (id.size() > 0)
    functions[id] = f;

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

  if (args.size() > 0) {
    auto indexType = llvm::Type::getInt64Ty(context);

    auto arraySize = llvm::ConstantInt::get(indexType, args.size());
    auto array = builder.CreateAlloca(plsmType, arraySize);

    for (size_t i = 0; i < args.size(); i++) {
      auto idx = llvm::ConstantInt::get(indexType, i);
      auto ptr = builder.CreateGEP(array, idx);

      auto value = args[i]->genCode(*this);

      builder.CreateStore(value, ptr);
    }
    callArgs.push_back(array);
  } else {
    callArgs.push_back(
        llvm::ConstantPointerNull::get(plsmType->getPointerTo()));
  }

  return builder.CreateCall(f, callArgs);
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
  auto trueV = trueExpr->genCode(*this);
  builder.CreateBr(mergeBB);

  builder.SetInsertPoint(falseBB);
  auto falseV = falseExpr->genCode(*this);
  builder.CreateBr(mergeBB);

  builder.SetInsertPoint(mergeBB);

  auto phiNode = builder.CreatePHI(plsmType, 2);
  phiNode->addIncoming(trueV, trueBB);
  phiNode->addIncoming(falseV, falseBB);

  return phiNode;
}

llvm::ExecutionEngine &PlsmContext::getExecutionEngine() {
  auto &result = *(llvm::EngineBuilder(std::unique_ptr<llvm::Module>(&module))
                       .setEngineKind(llvm::EngineKind::JIT)
                       .create());

  result.DisableSymbolSearching();

  result.addGlobalMapping(freeFunction, (void *)&free);
  result.addGlobalMapping(mallocFunction, (void *)&malloc);

  result.addGlobalMapping(functions["print"], (void *)&print);
  result.addGlobalMapping(functions["println"], (void *)&println);

  result.finalizeObject();
  return result;
}

void PlsmContext::optimize() { mpm.run(module, mam); }

void PlsmContext::printLLVMIR() { module.print(llvm::errs(), nullptr); }

void PlsmContext::disposeOptimizations() {
  mam.clear();
  gam.clear();
  fam.clear();
  lam.clear();
}
