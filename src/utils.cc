#include "utils.hh"
#include "defines.hh"

#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>

#include <iostream>

llvm::Value *createBinExpr(llvm::Value *left, llvm::Value *right,
                           BinExprCreator intBEC, BinExprCreator floatBEC,
                           const std::string &name) {
  auto leftT = left->getType();
  auto rightT = right->getType();

  if (leftT->isIntegerTy() && rightT->isIntegerTy()) {
    return intBEC(left, right);
  } else if (leftT->isFloatingPointTy() || rightT->isFloatingPointTy()) {
    if (!(leftT->isFloatingPointTy())) {
      left = createFloatCast(left);
    } else if (!(rightT->isFloatingPointTy())) {
      right = createFloatCast(right);
    }

    return floatBEC(left, createFloatCast(right));
  } else {
    std::cout << "unable to create " << name << " instruction" << std::endl;
  }

  return nullptr;
}

extern llvm::IRBuilder<> Builder;

llvm::Value *createIntCast(llvm::Value *v) {
  auto vT = v->getType();

  if (vT->isIntegerTy()) {
    return v;
  } else if (vT->isFloatingPointTy()) {
    extern llvm::Type *IntType;

    return Builder.CreateFPToSI(v, IntType);
  } else {
    std::cout << "unable to create integer cast" << std::endl;
  }

  return nullptr;
}

llvm::Value *createFloatCast(llvm::Value *v) {
  auto vT = v->getType();

  if (vT->isFloatingPointTy()) {
    return v;
  } else if (vT->isIntegerTy()) {
    extern llvm::Type *FloatType;

    return Builder.CreateSIToFP(v, FloatType);
  } else {
    std::cout << "unable to create floating point cast" << std::endl;
  }

  return nullptr;
}

llvm::Value *createIntAdd(llvm::Value *left, llvm::Value *right) {
  return Builder.CreateAdd(left, right);
}

llvm::Value *createFloatAdd(llvm::Value *left, llvm::Value *right) {
  return Builder.CreateFAdd(left, right);
}

llvm::Value *createIntSub(llvm::Value *left, llvm::Value *right) {
  return Builder.CreateSub(left, right);
}

llvm::Value *createFloatSub(llvm::Value *left, llvm::Value *right) {
  return Builder.CreateFSub(left, right);
}

llvm::Value *createIntMul(llvm::Value *left, llvm::Value *right) {
  return Builder.CreateMul(left, right);
}

llvm::Value *createFloatMul(llvm::Value *left, llvm::Value *right) {
  return Builder.CreateFMul(left, right);
}

llvm::Value *createIntMod(llvm::Value *left, llvm::Value *right) {
  return Builder.CreateSRem(left, right);
}

llvm::Value *createFloatMod(llvm::Value *left, llvm::Value *right) {
  return Builder.CreateFRem(left, right);
}



llvm::Value *createAdd(llvm::Value *left, llvm::Value *right) {
  if (!right) {
    extern llvm::Type *IntType;

    right = llvm::ConstantInt::get(IntType, 0);
  }

  return createBinExpr(left, right, createIntAdd, createFloatAdd, "add");
}

llvm::Value *createSub(llvm::Value *left, llvm::Value *right) {
  if (!right) {
    extern llvm::Type *IntType;

    auto temp = left;
    left = llvm::ConstantInt::get(IntType, 0);
    right = temp;
  }

  return createBinExpr(left, right, createIntSub, createFloatSub, "sub");
}

llvm::Value *createMul(llvm::Value *left, llvm::Value *right) {
  return createBinExpr(left, right, createIntMul, createFloatMul, "mul");
}

llvm::Value *createDiv(llvm::Value *left, llvm::Value *right) {
  left = createFloatCast(left);
  right = createFloatCast(right);

  return Builder.CreateFDiv(left, right);
}

llvm::Value *createMod(llvm::Value *left, llvm::Value *right) {
  return createBinExpr(left, right, createIntMod, createFloatMod, "mod");
}
