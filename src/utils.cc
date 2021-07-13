#include "ast.hh"
#include "utils.hh"
#include "defines.hh"

#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>

#include <iostream>

int64_t typeSize(llvm::Type *type) {
  return 8;
  /*extern llvm::DataLayout DataLayout;

  return DataLayout.getTypeAllocSize(type).getValue();*/
}

llvm::Function *createMain() {
  extern llvm::LLVMContext Context;
  extern llvm::Module Module;
  extern llvm::IRBuilder<> Builder;
  extern llvm::Type *IntType;
  extern llvm::StructType *PlsmType;
  extern std::map<std::string, llvm::Function *> Functions;

  if (!Functions.count("main")) return nullptr;

  auto retT = llvm::IntegerType::getInt8Ty(Context);
  auto ft = llvm::FunctionType::get(retT, false);
  auto f =
      llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "main", Module);
  
  auto bb = llvm::BasicBlock::Create(Context, "", f);

  Builder.SetInsertPoint(bb);

  auto callExpr = new CallExpr("main", {});
  callExpr->genCode();

  Builder.CreateRet(llvm::ConstantInt::get(retT, 42));

  return f;
}

llvm::Function *createAddFunction() { return nullptr; }

llvm::Value *plsmValue(int8_t type, llvm::Value *value) {
  extern llvm::Type *TypeType;
  extern llvm::PointerType *PointerType;
  extern llvm::StructType *PlsmType;
  extern llvm::IRBuilder<> Builder;

  auto result = Builder.CreateAlloca(PlsmType);

  auto typePtr = Builder.CreateStructGEP(result, 0);
  auto valuePtr = Builder.CreateStructGEP(result, 1);

  Builder.CreateStore(llvm::ConstantInt::get(TypeType, type), typePtr);

  value = Builder.CreatePointerCast(value, PointerType);
  Builder.CreateStore(value, valuePtr);

  return Builder.CreateLoad(PlsmType, result);
}

llvm::Constant *plsmConstValue(int8_t type, llvm::Constant *value) {
  extern llvm::Type *TypeType;
  extern llvm::StructType *PlsmType;

  auto attrs = {llvm::ConstantInt::get(TypeType, type), value};
  return llvm::ConstantStruct::get(PlsmType, attrs);
}

llvm::Constant *nullValue(int8_t type) {
  extern llvm::PointerType *PointerType;
  return plsmConstValue(type, llvm::ConstantPointerNull::get(PointerType));
}

llvm::Value *plsmMalloc(int64_t size, llvm::Type *pointerType) {
  extern llvm::LLVMContext Context;
  extern llvm::IRBuilder<> Builder;
  extern llvm::Function *MallocFunc;
  
  auto arg = llvm::ConstantInt::get(llvm::Type::getInt64Ty(Context), size);

  auto result = (llvm::Value *) Builder.CreateCall(MallocFunc, {arg});
  result = Builder.CreatePointerCast(result, pointerType->getPointerTo());

  return result;
}
