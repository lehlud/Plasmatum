#include "Context.h"

#include "AST/Type.h"

#include <vector>
#include <iostream>

ValueScope::~ValueScope() {
    for (auto value : createdValues) delete value;
}

StoredValue* ValueScope::getValue(const std::string &id) {
    if (values.count(id)) return values[id];
    return nullptr;
}

void ValueScope::setValue(const std::string &id, StoredValue *value) {
    values[id] = value;
    createdValues.push_back(value);
}

void ValueScope::dispose(Context *context) {
    for (auto value : createdValues) {
        auto store = context->builder.CreateBitCast(value->getValue(), context->builder.getInt8PtrTy());
        context->builder.CreateCall(context->freeFunction, {store});
    }
}

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
    for (auto scope : valueScopes) {
        scope->dispose(this);
        delete scope;
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

    auto numPtrType = numType->getPointerTo();
    auto NumNumFT = llvm::FunctionType::get(numPtrType, {numPtrType, numPtrType}, false);
    module.getOrInsertFunction("__plsm_add_Num_Num", NumNumFT);
    module.getOrInsertFunction("__plsm_sub_Num_Num", NumNumFT);
    module.getOrInsertFunction("__plsm_mul_Num_Num", NumNumFT);
    module.getOrInsertFunction("__plsm_div_Num_Num", NumNumFT);
    module.getOrInsertFunction("__plsm_mod_Num_Num", NumNumFT);
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
        auto value = scope->getValue(id);
        if (value) return value;
    }

    return nullptr;
}

void Context::setValue(const std::string &id, StoredValue *value, bool overwrite) {
    if (valueScopes.size() > 0) {
        for (size_t i = valueScopes.size() - 1; i > 0 ; i--) {
            auto scope = valueScopes[i];
            auto value = scope->getValue(id);
            if (value) {
                if (value->isDefinition && scope == valueScopes.back()) {
                    std::cout << "Redefinition of constant '" << id << "'" << std::endl;
                    std::exit(1);
                } else if (overwrite) {
                    scope->setValue(id, value);
                    return;
                }
            }
        }
    }

    valueScopes.back()->setValue(id, value);
}

void Context::initNewValueScope() {
    valueScopes.push_back(new ValueScope());
}

void Context::disposeLastValueScope() {
    auto scope = valueScopes.back();
    scope->dispose(this);
    delete scope;

    valueScopes.pop_back();
}

llvm::Value *Context::createMalloc(llvm::Type *type) {
    auto size = dataLayout.getTypeAllocSize(type);
    auto sizeValue = builder.getInt64(size);
    auto store = builder.CreateCall(mallocFunction, {sizeValue});
    return builder.CreateBitCast(store, type->getPointerTo());
}
