#pragma once

#include <map>
#include <string>

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>

class Type;
class Expr;

class StoredValue {
private:
    llvm::Type *type;
    llvm::Value *value;

public:
    const bool isDefinition;

public:
    StoredValue(llvm::Type *type, llvm::Value *value, bool isDefinition = false)
        : type(type), value(value), isDefinition(isDefinition) {}

    llvm::Type *getType() { return type; }
    llvm::Value *getValue() { return value; }
};

typedef std::map<std::string, StoredValue *> ValueScope;

class Context {
private:
    std::vector<ValueScope> valueScopes;

    std::map<std::string, llvm::StructType *> types;
    std::map<llvm::StructType *, std::vector<std::string>> structFields;

public:
    llvm::DataLayout dataLayout;
    llvm::LLVMContext llvmContext;
    llvm::Module module;
    llvm::IRBuilder<> builder;

    llvm::StructType *numType, *stringType;
    
    llvm::Function *mainFunction, *mallocFunction, *freeFunction;

public:
    Context();
    ~Context();

private:
    void initTypes();
    void initScopes();
    void initBuiltins();

    void setupMain();

public:
    llvm::StructType* getType(const std::string &id);
    void registerType(llvm::StructType* type, const std::vector<std::string> &fields);

    StoredValue* getValue(const std::string &id);
    void setValue(const std::string &id, StoredValue *value);

    void initNewValueScope();
    void disposeLastValueScope();

    llvm::Value *createMalloc(llvm::Type *type);
};
