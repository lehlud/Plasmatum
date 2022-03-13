#include "Context.h"

#include "AST/Type.h"

#include <vector>
#include <iostream>

Context::Context() : dataLayout(llvm::EngineBuilder().selectTarget()->createDataLayout()),
        llvmContext(), module("", llvmContext), builder(llvmContext) {
    module.setDataLayout(dataLayout);
    auto targetTriple = llvm::EngineBuilder().selectTarget()->getTargetTriple();
    module.setTargetTriple(targetTriple.str());

    initTypes();
    initScopes();
    initBuiltins();

    setupMain();
}

Context::~Context() {
    for (auto type : types) {
        delete type.second;
    }

    for (auto valueScope : valueScopes) {
        for (auto value : valueScope) {
            delete value.second;
        }
    }
}

void Context::initTypes() {
    std::vector<llvm::Type*> fields;

    fields = {builder.getDoubleTy()};
    numType = llvm::StructType::create(llvmContext, fields, "Num");

    fields = {builder.getInt8PtrTy()};
    stringType = llvm::StructType::create(llvmContext, fields, "Str");

    registerType(numType, {});
    registerType(stringType, {});
}

void Context::initScopes() {
    initNewValueScope();
}

void Context::initBuiltins() {
    auto mallocFT = llvm::FunctionType::get(builder.getInt8PtrTy(), {builder.getInt64Ty()}, false);
    mallocFunction = (llvm::Function *)module.getOrInsertFunction("malloc", mallocFT).getCallee();

    auto freeFT = llvm::FunctionType::get(builder.getVoidTy(), {builder.getInt8PtrTy()}, false);
    freeFunction = (llvm::Function *)module.getOrInsertFunction("free", freeFT).getCallee();
}

void Context::setupMain() {
    auto mainFT = llvm::FunctionType::get(builder.getVoidTy(), {}, false);
    mainFunction = llvm::Function::Create(mainFT, llvm::Function::ExternalLinkage, "main", &module);
    auto mainBB = llvm::BasicBlock::Create(llvmContext, "", mainFunction);
    builder.SetInsertPoint(mainBB);
}

llvm::StructType* Context::getType(const std::string &id) {
    if (!types.count(id)) return nullptr;
    return types[id];
}

void Context::registerType(llvm::StructType* type, const std::vector<std::string> &fields) {
    if (types.count(type->getName().str())) {
        std::cerr << "Type '" << type->getName().str() << "' already registered" << std::endl;
        std::exit(1);
    }

    types[type->getName().str()] = type;
    structFields[type] = fields;
}

StoredValue* Context::getValue(const std::string &id) {
    for (auto scope : valueScopes) {
        auto it = scope.find(id);
        if (it != scope.end()) return it->second;
    }

    return nullptr;
}

void Context::setValue(const std::string &id, StoredValue *value) {
    if (valueScopes.back().count(id)) {
        auto current = valueScopes.back()[id];
        if (current->isDefinition) {
            std::cout << "Redefinition of variable '" << id << "'" << std::endl;
            std::exit(1);
        } else {
            delete current;
        }
    }

    valueScopes.back()[id] = value;
}

void Context::initNewValueScope() {
    valueScopes.push_back(ValueScope());
}

void Context::disposeLastValueScope() {
    for (auto valueScope : valueScopes.back()) {
        auto store = builder.CreateBitCast(valueScope.second->getValue(), builder.getInt8PtrTy());
        builder.CreateCall(freeFunction, {store});
        delete valueScope.second;
    }

    valueScopes.pop_back();
}

llvm::Value *Context::createMalloc(llvm::Type *type) {
    auto size = dataLayout.getTypeAllocSize(type);
    auto sizeValue = builder.getInt64(size);
    auto store = builder.CreateCall(mallocFunction, {sizeValue});
    return builder.CreateBitCast(store, type->getPointerTo());
}
