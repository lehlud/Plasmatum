#pragma once

#include <map>
#include <vector>
#include <string>

#include <llvm/IR/DerivedTypes.h>

class Context;

class TypeRef {
private:
    std::string name;

public:
    TypeRef(std::string name) : name(name) {}

    std::string getName() { return name; }

    llvm::StructType *codegen(Context *context);
};

class Type {
private:
    std::string name;
    std::vector<std::pair<std::string, TypeRef*>> fields;

public:
    Type(const std::string &name, const std::vector<std::pair<std::string, TypeRef*>> &fields)
        : name(name), fields(fields) {}
    ~Type();

    std::vector<std::pair<std::string, Type*>> getFields();

    void registerType(Context *context);
};
