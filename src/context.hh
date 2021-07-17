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

  llvm::Type *binExprMatrixType;

  llvm::FunctionType *functionType;
  llvm::FunctionType *logicalFunctionType;
  llvm::FunctionType *binExprFunctionType;

  llvm::GlobalVariable *addFuncs;
  llvm::GlobalVariable *subFuncs;
  llvm::GlobalVariable *mulFuncs;
  llvm::GlobalVariable *divFuncs;
  llvm::GlobalVariable *logicalFuncs;

  llvm::Function *freeFunction;
  llvm::Function *mallocFunction;
  llvm::Function *getArgFunction;
  llvm::Function *mainFunction;

  std::map<std::string, llvm::Function *> functions;

  size_t disposalDepth = 0;
  std::vector<std::map<std::string, llvm::Value *>> variableScopes;

  llvm::ModuleAnalysisManager mam;
  llvm::CGSCCAnalysisManager gam;
  llvm::FunctionAnalysisManager fam;
  llvm::LoopAnalysisManager lam;
  llvm::ModulePassManager mpm;

  void initAllocs();
  void initLogicals();
  void initGetArgFunction();

  void initBinExprs();
  void initAdds();
  void initSubs();
  void initMuls();
  void initDivs();

  llvm::Function *getNullBinExpr();

  llvm::Function *getIntAdd();
  llvm::Function *getIntSub();
  llvm::Function *getIntMul();
  llvm::Function *getIntDiv();

  llvm::Function *getFloatAdd();
  llvm::Function *getFloatSub();
  llvm::Function *getFloatMul();
  llvm::Function *getFloatDiv();

  void initNewScope();
  void disposeLastScope();
  void disposeMarkedScopes();
  void setVariable(const std::string &id, llvm::Value *value);
  llvm::Value *getVariable(const std::string &id);

  llvm::Function *getNullLogical();
  llvm::Function *getIntLogical();
  llvm::Function *getFloatLogical();
  llvm::Function *getStringLogical();

public:
  PlsmContext();
  ~PlsmContext();

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

  llvm::Value *getIntFromPlsm(llvm::Value *plsmValue);
  llvm::Value *getFloatFromPlsm(llvm::Value *plsmValue);
  llvm::Value *getStringFromPlsm(llvm::Value *plsmValue);

  llvm::Value *getPlsmLogicalValue(llvm::Value *value);

  llvm::Function *getMain();

  llvm::Value *createFree(llvm::Value *pointer);
  llvm::Value *createMalloc(llvm::Type *resultType, int64_t numElements = 1);


  llvm::Value *createRet(llvm::Value *value);
  llvm::Value *createPlsmFunction(const std::string &id,
                                  std::vector<std::string> &args,
                                  std::vector<Stmt *> &body);

  llvm::Value *createPlsmCall(const std::string &id, std::vector<Expr *> args);

  llvm::Value *createPlsmConditional(Expr *condExpr, Expr *trueExpr,
                                     Expr *falseExpr);

  llvm::ExecutionEngine &getExecutionEngine();

  void optimize();
  void printLLVMIR();

  void disposeOptimizations();
};
