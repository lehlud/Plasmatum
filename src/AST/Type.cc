#include "Type.h"

#include "../Context.h"

llvm::StructType *TypeRef::codegen(Context *context) {
    return context->getType(name);
}

Type::~Type() {
    for (auto field : fields) delete field.second;
}

void Type::registerType(Context *context) {
    std::vector<llvm::Type*> fields;
    std::vector<std::string> namedFields;
    for (auto field : this->fields) {
        namedFields.push_back(field.first);
        fields.push_back(field.second->codegen(context)->getPointerTo());
    }

    auto structType = llvm::StructType::create(context->llvmContext, fields, name);
    context->registerType(structType, namedFields);
}
