#include "ast.hh"

StructType::~StructType() {
    for (auto field : fields) {
        delete field;
    }
}

llvm::Type *StructType::llvmType(Context &context) {
    std::vector<llvm::Type *> fieldTypes;
    for (auto field : fields) {
        fieldTypes.push_back(field->llvmType(context));
    }
    return llvm::StructType::create(fieldTypes);
}

llvm::Value *Integer::genCode(Context &context) {
    auto type = context.builder.getInt64Ty();
    return llvm::ConstantInt::get(type, value);
}
