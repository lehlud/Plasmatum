#pragma once

#include "ast.hh"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include <llvm/Passes/PassBuilder.h>

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
  llvm::FunctionType *binExprFunctionType;

  llvm::Function *mainFunction;

  llvm::Function *freeFunction;
  llvm::Function *mallocFunction;
  llvm::Function *memcpyFunction;
  llvm::Function *plsmFreeFunction;

  llvm::Function *getArgFunction;
  llvm::Function *logicalFunction;

  llvm::Function *addFunction;
  llvm::Function *subFunction;
  llvm::Function *mulFunction;
  llvm::Function *divFunction;
  llvm::Function *modFunction;
  llvm::Function *eqFunction;
  llvm::Function *neFunction;
  llvm::Function *gtFunction;
  llvm::Function *ltFunction;
  llvm::Function *geFunction;
  llvm::Function *leFunction;

  std::map<std::string, llvm::Function *> functions;

  size_t disposalDepth = 0;
  std::vector<std::pair<std::map<std::string, llvm::Value *>,
                        std::vector<llvm::Value *>>>
      variableScopes;

  void initNewScope();
  void disposeLastScope();

  void freeLastScope();
  void freeScope(size_t index);
  void freeMarkedScopes();

  void setVariable(const std::string &id, llvm::Value *value);
  llvm::Value *getVariable(const std::string &id);

public:
  PlsmContext();

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

  void addToPlsmPointers(llvm::Value *value);

  llvm::Value *getPlsmValue(int8_t type, llvm::Value *valuePointer);
  llvm::Value *getPlsmNull();
  llvm::Value *getPlsmInt(int64_t value);
  llvm::Value *getPlsmFloat(double value);
  llvm::Value *getPlsmString(const std::u32string &string);

  llvm::Value *getTypeFromPlsm(llvm::Value *plsmValue);
  llvm::Value *getValueFromPlsm(llvm::Value *value, llvm::Type *type);

  llvm::Value *getPlsmLogicalValue(llvm::Value *value);

  llvm::Function *getMain();

  llvm::Value *createFree(llvm::Value *pointer);
  llvm::Value *createPlsmFree(llvm::Value *value);
  llvm::Value *createMalloc(llvm::Type *resultType, int64_t numElements = 1);
  llvm::Value *createMemCpy(llvm::Value *value, bool addToPlsmPointers = true);

  llvm::Value *createBinExpr(llvm::Function *f, llvm::Value *left,
                             llvm::Value *right);

  llvm::Value *createAdd(llvm::Value *left, llvm::Value *right);
  llvm::Value *createSub(llvm::Value *left, llvm::Value *right);
  llvm::Value *createMul(llvm::Value *left, llvm::Value *right);
  llvm::Value *createDiv(llvm::Value *left, llvm::Value *right);
  llvm::Value *createMod(llvm::Value *left, llvm::Value *right);
  llvm::Value *createEq(llvm::Value *left, llvm::Value *right);
  llvm::Value *createNE(llvm::Value *left, llvm::Value *right);
  llvm::Value *createGT(llvm::Value *left, llvm::Value *right);
  llvm::Value *createLT(llvm::Value *left, llvm::Value *right);
  llvm::Value *createGE(llvm::Value *left, llvm::Value *right);
  llvm::Value *createLE(llvm::Value *left, llvm::Value *right);

  llvm::Value *createVariableLoad(const std::string &id);

  llvm::Value *createRet(llvm::Value *value);
  llvm::Value *createPlsmFunction(const std::string &id,
                                  std::vector<std::string> &args,
                                  std::vector<Stmt *> &body);

  llvm::Value *createPlsmCall(const std::string &id, std::vector<Expr *> args);

  llvm::Value *createPlsmConditional(Expr *condExpr, Expr *trueExpr,
                                     Expr *falseExpr);

  llvm::ExecutionEngine &getExecutionEngine();

  void printLLVMIR();

  void optimize();
};
