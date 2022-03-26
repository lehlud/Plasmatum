#include "Expr.h"

#include "Stmt.h"
#include "Type.h"
#include "../Context.h"

#include <iostream>

BinExpr::~BinExpr() {
    delete left;
    delete right;
}

CallExpr::~CallExpr() {
    delete callee;
    for (auto arg : args) delete arg;
}

BlockExpr::~BlockExpr() {
    for (auto stmt : stmts) delete stmt;
    delete result;
}

FunctionExpr::~FunctionExpr() {
    for (auto arg : args) delete arg.second;
    delete result;
}

void IdExpr::print() {
    std::cout << "IdExpr: \'" << id << "\'";
}

void NumExpr::print() {
    std::cout << "NumExpr: " << value;
}

void StringExpr::print() {
    std::cout << "StringExpr: \'" << value << "\'";
}

void BinExpr::print() {
    std::cout << "BinExpr: { op: " << op << ", left: ";
    left->print();
    std::cout << ", right: ";
    right->print();
    std::cout << " }";
}

void CallExpr::print() {
    std::cout << "CallExpr: { callee: ";
    callee->print();
    std::cout << ", args: [";
    for (auto arg : args) {
        arg->print();
        if (arg != args.back()) std::cout << ", ";
    }
    std::cout << "] }";
}

void BlockExpr::print() {
    std::cout << "BlockExpr: ";
}

void FunctionExpr::print() {
    std::cout << "FunctionExpr: { ";
    std::cout << "args: [";
    for (auto arg : args) {
        std::cout << "{ id: '" << arg.first << "', type: ";
        if (arg.second) std::cout << "'" << arg.second->getName() << "'";
        else std::cout << "none";
        std::cout << " }";
        
        if (arg != args.back()) std::cout << ", ";
    }
    std::cout << "], ";
    std::cout << "result: ";
    result->print();
    std::cout << " }";
}

llvm::Value *IdExpr::codegen(Context *context) {
    auto store = context->getValue(id);
    auto value = store->getValue();
    auto type = value->getType()->getPointerElementType();

    return context->builder.CreateLoad(type, value);
}

llvm::Value *NumExpr::codegen(Context *context) {
    auto value = llvm::ConstantFP::get(context->builder.getDoubleTy(), this->value);
    auto store = context->createMalloc(context->numType);
    context->builder.CreateStore(value, store);
    return store;
}

llvm::Value *StringExpr::codegen(Context *context) {
    auto value = context->builder.CreateGlobalStringPtr(this->value);
    auto store = context->createMalloc(context->stringType);
    context->builder.CreateStore(value, store);
    return store;
}

llvm::Value *BinExpr::codegen(Context *context) {
    auto left = this->left->codegen(context);
    auto right = this->right->codegen(context);

    auto leftName = ((llvm::StructType *)left->getType()->getPointerElementType())->getName().str();
    auto rightName = ((llvm::StructType *)right->getType()->getPointerElementType())->getName().str();

    auto opName = this->opName();
    auto functionName = opName + "_" + leftName + "_" + rightName;

    auto function = context->module.getFunction("__plsm_" + functionName);
    if (!function) {
        std::cerr << "Binary operation not implemented (" << functionName << ")" << std::endl;
        std::exit(1);
    }

    return context->builder.CreateCall(function, {left, right});
}

llvm::Value *CallExpr::codegen(Context *context) {
    auto callee = this->callee->codegen(context);
    
    auto args = std::vector<llvm::Value *>();
    for (auto arg : this->args) args.push_back(arg->codegen(context));

    return context->builder.CreateCall((llvm::Function *)callee, args);
}

llvm::Value *BlockExpr::codegen(Context *context) {
    return 0;
}

llvm::Value *FunctionExpr::codegen(Context *context) {
    auto previousBB = context->builder.GetInsertBlock();

    auto dummyFT = llvm::FunctionType::get(context->builder.getVoidTy(), false);
    auto dummyFunction = llvm::Function::Create(dummyFT, llvm::Function::ExternalLinkage, "", context->module);

    auto firstBB = llvm::BasicBlock::Create(context->llvmContext, "", dummyFunction);
    auto secondBB = llvm::BasicBlock::Create(context->llvmContext, "", dummyFunction);
    auto thirdBB = llvm::BasicBlock::Create(context->llvmContext, "", dummyFunction);

    context->builder.SetInsertPoint(firstBB);

    context->initNewValueScope();

    auto args = std::vector<llvm::Value *>();
    auto argTypes = std::vector<llvm::Type *>();
    for (auto arg : this->args) {
        auto type = arg.second->codegen(context)->getPointerTo();
        argTypes.push_back(type);

        auto store = context->builder.CreateAlloca(type);
        args.push_back(store);

        context->setValue(arg.first, new StoredValue(type, store));
    }

    context->builder.CreateBr(secondBB);
    context->builder.SetInsertPoint(thirdBB);

    auto result = this->result->codegen(context);

    context->disposeLastValueScope();
    context->builder.CreateRet(result);

    auto functionType = llvm::FunctionType::get(result->getType(), argTypes, false);
    auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "", context->module);

    context->builder.SetInsertPoint(secondBB);

    for (size_t i = 0; i < function->arg_size(); i++) {
        auto arg = function->getArg(i);
        context->builder.CreateStore(arg, args[i]);
    }

    context->builder.CreateBr(thirdBB);

    function->getBasicBlockList().push_back(firstBB);
    function->getBasicBlockList().push_back(secondBB);
    function->getBasicBlockList().push_back(thirdBB);

    dummyFunction->removeFromParent();

    if (previousBB) context->builder.SetInsertPoint(previousBB);

    return function;
}
