/*
void setBinOpPrecedence() {
  AST::BinOpPrecedence = {
      // logical operators
      {AST::BinExpr::Type::LOR, 10},
      {AST::BinExpr::Type::LAND, 10},
      {AST::BinExpr::Type::EQ, 10},
      {AST::BinExpr::Type::NE, 10},
      {AST::BinExpr::Type::GT, 10},
      {AST::BinExpr::Type::LT, 10},
      {AST::BinExpr::Type::GE, 10},
      {AST::BinExpr::Type::LE, 10},
      // bitwise operators
      {AST::BinExpr::Type::OR, 20},
      {AST::BinExpr::Type::AND, 20},
      // arithmetic operators
      {AST::BinExpr::Type::ADD, 30},
      {AST::BinExpr::Type::SUB, 30},
      {AST::BinExpr::Type::MUL, 31},
      {AST::BinExpr::Type::DIV, 31},
      {AST::BinExpr::Type::MOD, 31},
      {AST::BinExpr::Type::POW, 32},
  };
}
*/

#include "ast.hh"

#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

#include <llvm/Support/TargetSelect.h>

#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>

#include <iostream>

extern llvm::LLVMContext Context;
extern llvm::Module Module;
extern llvm::IRBuilder<> Builder;

void initializeModule() {
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  auto targetMachine = llvm::EngineBuilder().selectTarget();
  auto dataLayout = targetMachine->createDataLayout();

  Module.setDataLayout(dataLayout);
}

extern "C" int64_t foo() {
  std::cout << "Hello World!" << std::endl;
  return 1;
}

int main(int argc, char **argv) {
  if (argc <= 1) {
    // error here
  }

  initializeModule();

  extern llvm::Type *IntType;

  auto mainFuncT = llvm::FunctionType::get(IntType, {}, false);
  auto mainFunc = llvm::Function::Create(
      mainFuncT, llvm::Function::ExternalLinkage, "main", Module);
  
  auto bb = llvm::BasicBlock::Create(Context, "", mainFunc);
  Builder.SetInsertPoint(bb);

  auto testExpr = new MulBinExpr(
      new IntExpr(2), new MulBinExpr(new IntExpr(3), new IntExpr(7)));
  
  auto callee = llvm::Function::Create(llvm::FunctionType::get(IntType, {}, false), llvm::Function::ExternalLinkage, "foo", Module);

  Builder.CreateCall(callee, {});

  Builder.CreateRet(testExpr->genCode());

  auto engine = llvm::EngineBuilder(std::unique_ptr<llvm::Module>(&Module))
    .setEngineKind(llvm::EngineKind::JIT)
    .create();

  if (!engine) {
    std::cout << "error" << std::endl;
    return 1;
  }

  engine->DisableSymbolSearching();

  engine->addGlobalMapping((llvm::GlobalValue *) callee, (void *) &::foo);

  engine->finalizeObject();
  auto f = engine->FindFunctionNamed("main");
  engine->runFunction(f, {});

  return 0;
}
