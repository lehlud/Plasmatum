#include "ast.hh"
#include "defines.hh"
#include "utils.hh"

#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <iostream>

extern llvm::Module Module;
extern llvm::LLVMContext Context;
extern llvm::IRBuilder<> Builder;

llvm::Value *IntExpr::genCode() {
  extern llvm::Type *IntType;
  auto intVal = llvm::ConstantInt::get(IntType, value);

  auto ptr = plsmMalloc(typeSize(IntType), IntType);
  Builder.CreateStore(intVal, ptr);

  return plsmValue(TYPE_INT, ptr);
}

llvm::Value *FloatExpr::genCode() {
  extern llvm::Type *FloatType;
  auto floatVal = llvm::ConstantFP::get(FloatType, value);

  auto ptr = plsmMalloc(typeSize(FloatType), FloatType);
  Builder.CreateStore(floatVal, ptr);

  return plsmValue(TYPE_FLOAT, ptr);
}

llvm::Value *StringExpr::genCode() {
  extern llvm::Type *IntType;

  auto charType = llvm::Type::getInt32Ty(Context);

  std::vector<llvm::Constant *> chars;
  for (auto &c : value) {
    chars.push_back(llvm::ConstantInt::get(charType, c));
  }

  chars.push_back(llvm::ConstantInt::get(charType, 0));

  auto arrT = llvm::ArrayType::get(charType, value.size() + 1);
  auto str = llvm::ConstantArray::get(arrT, chars);

  auto ptr = plsmMalloc(typeSize(charType) * (value.size() + 1), charType);
  ptr = Builder.CreatePointerCast(ptr, arrT->getPointerTo());

  Builder.CreateStore(str, ptr);

  return plsmValue(TYPE_STRING, ptr);
}

llvm::Value *AddBinExpr::genCode() {
  auto leftV = left->genCode();
  auto rightV = right->genCode();

  return nullptr;
}

llvm::Value *SubBinExpr::genCode() {
  auto leftV = left->genCode();
  auto rightV = right->genCode();

  return nullptr;
}

llvm::Value *MulBinExpr::genCode() {
  auto leftV = left->genCode();
  auto rightV = right->genCode();

  return nullptr;
}

llvm::Value *DivBinExpr::genCode() {
  auto leftV = left->genCode();
  auto rightV = right->genCode();

  return nullptr;
}

llvm::Value *CallExpr::genCode() {
  extern llvm::Type *IntType;
  extern llvm::StructType *PlsmType;
  extern std::map<std::string, llvm::Function *> Functions;

  auto f = Functions[callee];
  if (!f) {
    std::cout << "unable to find function '" << callee << "'" << std::endl;
  }

  auto argCount = llvm::ConstantInt::get(IntType, args.size());
  std::vector<llvm::Value *> callArgs = {argCount};

  if (args.size() > 0) {
    auto indexType = llvm::Type::getInt64Ty(Context);

    auto arraySize = llvm::ConstantInt::get(indexType, args.size());
    auto array = Builder.CreateAlloca(PlsmType, arraySize);

    for (size_t i = 0; i < args.size(); i++) {
      auto idx = llvm::ConstantInt::get(indexType, i);
      auto ptr = Builder.CreateGEP(array, idx);

      auto value = args[i]->genCode();

      Builder.CreateStore(value, ptr);
    }
    callArgs.push_back(array);
  } else {
    callArgs.push_back(
        llvm::ConstantPointerNull::get(PlsmType->getPointerTo()));
  }

  return Builder.CreateCall(f, callArgs);
}

llvm::Value *ExprStmt::genCode() {
  auto result = expr->genCode();
  return result;
}

llvm::Value *ReturnStmt::genCode() {
  auto returnValue = value->genCode();

  return Builder.CreateRet(returnValue);
}

llvm::Value *FunctionStmt::genCode() {
  extern std::map<std::string, llvm::Function *> Functions;
  extern llvm::FunctionType *FunctionType;

  auto f = llvm::Function::Create(FunctionType, llvm::Function::ExternalLinkage,
                                  "", Module);

  auto bb = llvm::BasicBlock::Create(Context, "", f);

  Builder.SetInsertPoint(bb);

  for (auto &stmt : body) {
    stmt->genCode();
  }

  if (llvm::verifyFunction(*f)) {
    Builder.CreateRet(nullValue());
  }

  Functions[id] = f;

  return f;
}
