#include "utils.hh"
#include "defines.hh"

#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>

llvm::Value *PlsmValue(llvm::Value *value) {
  extern llvm::Type *IntType;
  extern llvm::Type *PlsmType;
  extern llvm::IRBuilder<> Builder;

  auto valueType = value->getType();

  uint8_t type = TYPE_ERR;
  if (valueType->isIntegerTy()) {
    auto integerType = (llvm::IntegerType *)valueType;
    auto integerBitWidth = integerType->getIntegerBitWidth();
    if (integerBitWidth == INT_SIZE) {
      type = TYPE_INT;
    } else if (integerBitWidth == CHAR_SIZE) {
      type = TYPE_CHAR;
    }
  } else if (valueType->isFloatingPointTy()) {
    type = TYPE_FLOAT;
  } else if (valueType->isPointerTy()) {
    extern int isMap;
    extern int isList;
    extern int isReference;
    if (isMap) {
      type = TYPE_MAP;
    } else if (isList) {
      type = TYPE_LIST;
    } else if (isReference) {
      type = TYPE_POINTER;
    }
  }

  if (type == TYPE_ERR) {
    // error here
    exit(1);
  }

  auto result = llvm::UndefValue::get(PlsmType);

  auto one = llvm::ConstantInt::get(IntType, 1);
  auto zero = llvm::ConstantInt::get(IntType, 0);

  auto typePointer = Builder.CreateGEP(result, zero);
  auto valuePointer = Builder.CreateGEP(result, one);

  auto typeValue = llvm::ConstantInt::get(IntType, type);

  Builder.CreateStore(typePointer, typeValue);
  Builder.CreateStore(valuePointer, value);

  return result;
}

llvm::Value *NullValue() {
  extern llvm::LLVMContext Context;
  auto voidPointerType = llvm::Type::getVoidTy(Context)->getPointerTo();
  auto nullPointer = llvm::ConstantPointerNull::get(voidPointerType);

  return PlsmValue(nullPointer);
}