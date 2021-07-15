#include "context.hh"
#include "defines.hh"

#include <iostream>
#include <llvm/IR/Verifier.h>

void PlsmContext::initLogicals() {
  auto lft = llvm::FunctionType::get(i1Type, {plsmType}, false);

  auto intLogical = llvm::Function::Create(lft, llvm::Function::ExternalLinkage,
                                           "int_logical", module);

  auto bb = llvm::BasicBlock::Create(context, "", intLogical);
  builder.SetInsertPoint(bb);

  builder.CreateRet(llvm::ConstantInt::get(i1Type, 1));

  auto floatLogical = llvm::Function::Create(
      lft, llvm::Function::ExternalLinkage, "float_logical", module);
  
  bb = llvm::BasicBlock::Create(context, "", floatLogical);
  builder.SetInsertPoint(bb);

  builder.CreateRet(llvm::ConstantInt::get(i1Type, 1));

  std::vector<llvm::Constant *> funcs = {intLogical, floatLogical};
  auto arrT = llvm::ArrayType::get(lft, funcs.size());
  auto arr = llvm::ConstantArray::get(arrT, funcs);

  logicalFuncs = new llvm::GlobalVariable(
      module, arrT, true, llvm::GlobalVariable::ExternalLinkage, arr);
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

llvm::Value *PlsmContext::malloc(int64_t size, llvm::Type *resultType) {
  auto arg = getI64(size);

  auto result = (llvm::Value *)builder.CreateCall(mallocFunction, {arg});
  result = builder.CreatePointerCast(result, resultType->getPointerTo());

  return result;
}

llvm::Value *PlsmContext::malloc(llvm::Type *resultType, int64_t numElements) {
  return malloc(getTypeSize(resultType) * numElements, resultType);
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
  auto typeValue = llvm::ConstantInt::get(typeType, TYPE_NULL);
  auto nullPtr = llvm::ConstantPointerNull::get(pointerType);

  std::vector<llvm::Constant *> attrs = {typeValue, nullPtr};
  return llvm::ConstantStruct::get(plsmType, attrs);
}

llvm::Value *PlsmContext::getPlsmInt(int64_t value) {
  auto ptr = malloc(intType);

  builder.CreateStore(getInt(value), ptr);

  return getPlsmValue(TYPE_INT, ptr);
}

llvm::Value *PlsmContext::getPlsmFloat(double value) {
  auto ptr = malloc(floatingPointType);

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

  auto ptr = malloc(arrT);

  builder.CreateStore(str, ptr);

  return getPlsmValue(TYPE_STRING, ptr);
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

llvm::Value *PlsmContext::createRet(llvm::Value *value) {
  return builder.CreateRet(value);
}

llvm::Value *PlsmContext::createPlsmFunction(const std::string &id,
                                             std::vector<Stmt *> body) {
  auto f = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage,
                                  "", module);

  auto bb = llvm::BasicBlock::Create(context, "", f);

  builder.SetInsertPoint(bb);

  for (auto &stmt : body) {
    stmt->genCode(*this);
  }

  if (llvm::verifyFunction(*f)) {
    builder.CreateRet(getPlsmNull());
  }

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

llvm::Value *PlsmContext::createPlsmIf(llvm::Value *cond, llvm::Value *trueV,
                                       llvm::Value *falseV) {
  // asdf
  return nullptr;
}

llvm::ExecutionEngine &PlsmContext::getExecutionEngine() {
  auto &result = *(llvm::EngineBuilder(std::unique_ptr<llvm::Module>(&module))
                       .setEngineKind(llvm::EngineKind::Interpreter)
                       .setOptLevel(llvm::CodeGenOpt::Level::None)
                       .setRelocationModel(llvm::Reloc::Model::PIC_)
                       .create());

  result.DisableSymbolSearching();

  result.addGlobalMapping(mallocFunction, (void *)&::std::malloc);

  result.addGlobalMapping(functions["print"], (void *)&::print);
  result.addGlobalMapping(functions["println"], (void *)&::println);

  result.finalizeObject();
  return result;
}

void PlsmContext::printLLVMIR() { module.print(llvm::errs(), nullptr); }
