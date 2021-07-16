#pragma once

#include "ast.hh"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>

#include <cstdlib>
#include <map>
#include <string>
#include <vector>

class PlsmContext {
private:
  llvm::DataLayout dataLayout;

  llvm::LLVMContext context;
  llvm::Module module;
  llvm::IRBuilder<> builder;

  llvm::IntegerType *i1Type;
  llvm::IntegerType *i32Type;
  llvm::IntegerType *i64Type;
  llvm::IntegerType *intType;
  llvm::IntegerType *charType;
  llvm::Type *floatingPointType;

  llvm::IntegerType *typeType;
  llvm::PointerType *pointerType;
  llvm::StructType *plsmType;

  llvm::FunctionType *functionType;

  llvm::GlobalVariable *logicalFuncs;

  llvm::Function *mallocFunction;
  llvm::Function *mainFunction;

  std::map<std::string, llvm::Function *> functions;
  std::vector<std::map<std::string, llvm::Value *>> variableScopes;

  void init() {}

  void initLogicals();

public:
  PlsmContext()
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
        functions(), variableScopes(), mainFunction(nullptr) {
    module.setDataLayout(dataLayout);

    auto mallocFunctionType =
        llvm::FunctionType::get(pointerType, {i64Type}, false);
    mallocFunction = llvm::Function::Create(
        mallocFunctionType, llvm::Function::ExternalLinkage, "malloc", module);

    std::vector<std::string> builtIns = {"print", "println"};
    for (auto &function : builtIns) {
      auto tmp = llvm::Function::Create(
          functionType, llvm::Function::ExternalLinkage, function, module);
      functions[function] = tmp;
    }

    // initLogicals();
  }

  llvm::LLVMContext &getContext() { return context; }
  llvm::Module &getModule() { return module; }
  llvm::IRBuilder<> &getBuilder() { return builder; }

  llvm::IntegerType *geti64Type() { return i64Type; }
  llvm::IntegerType *getIntType() { return intType; }
  llvm::Type *getFloatingPointType() { return floatingPointType; }

  llvm::IntegerType *getTypeType() { return typeType; }
  llvm::PointerType *getPointerType() { return pointerType; }
  llvm::StructType *getPlsmType() { return plsmType; }

  llvm::Constant *getI64(int64_t value);
  llvm::Constant *getInt(int64_t value);
  llvm::Constant *getFloat(double value);

  int64_t getTypeSize(llvm::Type *type);

  llvm::Value *getPlsmValue(int8_t type, llvm::Value *valuePointer);
  llvm::Value *getPlsmNull();
  llvm::Value *getPlsmInt(int64_t value);
  llvm::Value *getPlsmFloat(double value);
  llvm::Value *getPlsmString(const std::u32string &string);

  llvm::Function *getMain();

  llvm::Value *createMalloc(llvm::Type *resultType, int64_t numElements = 1);
  llvm::Value *createRet(llvm::Value *value);
  llvm::Value *createPlsmFunction(const std::string &id,
                                  std::vector<Stmt *> body);

  llvm::Value *createPlsmCall(const std::string &id, std::vector<Expr *> args);

  llvm::Value *createPlsmIf(llvm::Value *cond, llvm::Value *trueV,
                            llvm::Value *falseV);

  llvm::ExecutionEngine &getExecutionEngine();

  void printLLVMIR();
};
