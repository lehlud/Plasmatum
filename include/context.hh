#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>

#include <vector>

class Stmt;
class Context;

typedef llvm::Value *(*native)(Context &, llvm::Value *, llvm::Value *);
#define _VAR_VALUE_TYPE std::pair<llvm::Type *, llvm::Value *>
#define _VAR_SCOPE_TYPE std::map<std::string, _VAR_VALUE_TYPE>

#define _TYPE_ATTRS std::map<std::string, unsigned long>
#define _TYPE_VALUE_TYPE std::pair<llvm::StructType *, _TYPE_ATTRS>
#define _TYPE_VALUE std::pair<llvm::Constant *, _TYPE_VALUE_TYPE>
#define _TYPE_SCOPE_TYPE std::map<std::string, _TYPE_VALUE>

class Context {
public:
  llvm::DataLayout dataLayout;
  llvm::LLVMContext llvmContext;
  llvm::Module module;
  llvm::IRBuilder<> builder;

  _VAR_SCOPE_TYPE namedValues;
  _TYPE_SCOPE_TYPE namedTypes;
  std::map<std::string, native> natives;

  Context();

private:
  void initNatives();
  void initTypes();
  void initFunctions();

public:
  auto getPointerType() { return pointerType; }
  auto getClassTypeNameType() { return classTypeNameType; }
  auto getDummyFunctionType() { return dummyFunctionType; }

  auto getNativeFloatType() { return nativeFloatType; }
  auto getNativeIntType() { return nativeFloatType; }
  auto getNativeCharType() { return nativeCharType; }

  llvm::Type *createClassType(std::string, std::map<std::string, llvm::Type *>);

  void print();
  void optimizeIR();
  void createMain(std::vector<Stmt *> stmts);

  llvm::ExecutionEngine &getExecutionEngine();

  llvm::Type *pointerType;
  llvm::Type *classTypeNameType;
  llvm::FunctionType *dummyFunctionType;

  llvm::Type *nativeFloatType;
  llvm::IntegerType *nativeIntType;
  llvm::IntegerType *nativeCharType;

  llvm::Type *builtinIntType;
  llvm::Type *builtinFloatType;
};
