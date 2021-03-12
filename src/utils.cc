#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include "utils.hh"

static llvm::Function *MainFunc;
static llvm::BasicBlock *MainBB;
static std::unique_ptr<llvm::Module> Module;
static std::unique_ptr<llvm::IRBuilder<>> Builder;
static std::unique_ptr<llvm::LLVMContext> Context;
static std::unique_ptr<PlsmJIT> JIT;
static std::map<std::string, llvm::AllocaInst*> IdValues;
static std::unique_ptr<llvm::legacy::FunctionPassManager> FPM;
static llvm::ExitOnError ExitOnErr;

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

extern int errors;

llvm::Value *NumExpr::genCode() {
    return llvm::ConstantFP::get(llvm::IntegerType::getDoubleTy(*Context), val);
}

llvm::Value *VarExpr::genCode() {
    llvm::Value *v = IdValues[id];
    if (v) return Builder->CreateLoad(v, id.c_str());
    logError("\'" + id + "\' is not defined");
    return 0;
}

llvm::Value *BinExpr::genCode() {
    llvm::Value *l = left->genCode();
    llvm::Value *r = right->genCode();
    if (!l || !r) return 0;
    switch (op) {
    case ADD_OP:  return Builder->CreateFAdd(l, r, "addtmp");
    case SUB_OP:  return Builder->CreateFSub(l, r, "subtmp");
    case MUL_OP:  return Builder->CreateFMul(l, r, "multmp");
    case DIV_OP:  return Builder->CreateFDiv(l, r, "divtmp");

    case OR_OP:   return Builder->CreateOr(l, r, "ortmp");
    case AND_OP:  return Builder->CreateAnd(l, r, "andtmp");
    case XOR_OP:  return Builder->CreateXor(l, r, "xortmp");
    case EQ_OP:   return Builder->CreateFCmpUEQ(l, r, "eqtmp");
    case GR_OP:   return Builder->CreateFCmpUGT(l, r, "gttmp");
    case LO_OP:   return Builder->CreateFCmpULT(l, r, "lttmp");
    case GREQ_OP: return Builder->CreateFCmpULT(l, r, "gteqtmp");
    case LOEQ_OP: return Builder->CreateFCmpULT(l, r, "lteqtmp");
    }
    logError("usage of unknown operator");
    return 0;
}


llvm::Value *BlockStmt::genCode() {
    llvm::BasicBlock *tmp = Builder->GetInsertBlock();
    llvm::BasicBlock *block = llvm::BasicBlock::Create(*Context, "block");
    Builder->SetInsertPoint(block);
    for (unsigned long i = 0; i < stmts->size(); i++)
        if (!stmts->at(i)->genCode())
    Builder->SetInsertPoint(tmp);
    return block;
}


llvm::Value *IfStmt::genCode() {
    llvm::Value *v = cond->genCode();
    llvm::BasicBlock *ifP = genBBStmtCode(ifPart);
    ifP->setName("if_part");
    if (elsePart) {
        llvm::BasicBlock *elseP = genBBStmtCode(elsePart);
        elseP->setName("else_part");
        return Builder->CreateCondBr(v, ifP, elseP);
    }
    return Builder->CreateCondBr(v, ifP, emptyBB);
}

llvm::Value *ForStmt::genCode() {
    llvm::Value *minVal = min->genCode();
    llvm::Value *maxVal = max->genCode();
    llvm::AllocaInst *alloc = Builder->CreateAlloca(llvm::Type::getDoubleTy(*Context), 0, counter->getId());
    IdValues[counter->getId()] = std::move(alloc);
    llvm::BasicBlock *loop = genBBStmtCode(body);
    Builder->CreateStore(minVal, alloc);
    Builder->CreateCondBr(Builder->CreateFCmpULT(Builder->CreateLoad(alloc, counter->getId()), maxVal, "lttmp"), loop, emptyBB);
    IdValues.erase(counter->getId());
    return (new NumExpr(0))->genCode();
}

llvm::Value *WhileStmt::genCode() {
    llvm::Value *condVal = cond->genCode();
    llvm::BasicBlock *loop = genBBStmtCode(body);
    Builder->CreateCondBr(condVal, loop, emptyBB);
    return (new NumExpr(0))->genCode();
}

llvm::Value *OutputStmt::genCode() {
    return Builder->CreateCall(
        Module->getFunction("print"),
        {val->genCode(), llvm::ConstantInt::get(llvm::IntegerType::getInt32Ty(*Context), prodNL)}
    );
}

llvm::Value *FDefStmt::genCode() {
    if (Module->getFunction(id)) {
        logError("function \'"+id+"\' already defined");
        return 0;
    }
    /*std::vector<llvm::Type*> Doubles(args->size(), llvm::Type::getDoubleTy(*Context));
    llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(*Context), Doubles, false);
    llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, id, *Module);
    llvm::AllocaInst *alloc = Builder->CreateAlloca(llvm::Type::getDoubleTy(*Context), 0, id);
    
    for (auto arg : F->args()) {
        // Create an alloca for this variable.
        llvm::AllocaInst *alloc1 = CreateEntryBlockAlloca(TheFunction, Arg.getName());

        // Store the initial value into the alloca.
        Builder->CreateStore(&Arg, Alloca);

        // Add arguments to variable symbol table.
        NamedValues[std::string(Arg.getName())] = Alloca;
    }*/
    return 0;
}

llvm::Value *DeclStmt::genCode() {
    if (!IdValues[id]) {
        llvm::Value *v = val->genCode();
        llvm::AllocaInst *alloc = Builder->CreateAlloca(v->getType(), 0, id);
        Builder->CreateStore(v, alloc);
        IdValues[id] = alloc;
        return (new NumExpr(0))->genCode();
    }
    logError("variable \'"+id+"\' is already defined");
    return 0;
}

llvm::Value *AssignStmt::genCode() {
    if (IdValues[id]) {
        Builder->CreateStore(val->genCode(), IdValues[id]);
        return (new NumExpr(0))->genCode();
    }
    logError("variable \'"+id+"\' is not defined");
    return 0;
}

llvm::Value *UndefStmt::genCode() {
    if (IdValues[id]) {
        IdValues.erase(id);
        return (new NumExpr(0))->genCode();
    }
    logError("variable \'"+id+"\' is not defined");
    return 0;
}

llvm::BasicBlock *genBBStmtCode(Stmt *stmt) {
    if (instanceof<BlockStmt*>(stmt))
        return (llvm::BasicBlock*) stmt->genCode();
    
    llvm::BasicBlock *tmp = Builder->GetInsertBlock();
    llvm::BasicBlock *bBlock = llvm::BasicBlock::Create(*Context, "block");
    Builder->SetInsertPoint(bBlock);
    stmt->genCode();
    Builder->SetInsertPoint(tmp);
    return bBlock;
}


void logError(std::string s) {
    std::cout << "error: " << s << std::endl;
    errors = 1;
}

std::string readFile(std::string name) {
    std::ifstream file;
    file.open(name);
    std::stringstream sstream;
    sstream << file.rdbuf();
    return sstream.str();
}

// 'extern "C"' is used to avoid name mangling
extern "C" DLLEXPORT void print(double num, int prodNL) {
    if (prodNL != -1) printf("%lf", num);
    if (prodNL) putchar('\n');
}

extern "C" DLLEXPORT void doooo() {
    puts("hi");
}

int execProgram(std::vector<Stmt*> program) {
    errors = 0;
    
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    JIT = ExitOnErr(PlsmJIT::Create());

    Context = std::make_unique<llvm::LLVMContext>();
    Module = std::make_unique<llvm::Module>("plsmjit", *Context);
    Module->setDataLayout(JIT->getDataLayout());
    Builder = std::make_unique<llvm::IRBuilder<>>(*Context);

    FPM = std::make_unique<llvm::legacy::FunctionPassManager>(Module.get());
    FPM->add(llvm::createInstructionCombiningPass());
    FPM->add(llvm::createReassociatePass());
    FPM->add(llvm::createGVNPass());
    FPM->doInitialization();

    ///*
    // create print functions
    Module->getOrInsertFunction(
        "print",
        llvm::FunctionType::get(
            llvm::IntegerType::getVoidTy(*Context),
            {llvm::Type::getDoubleTy(*Context), llvm::Type::getInt32Ty(*Context)},
            false
        )
    );
    //*/
    
    /*
    auto printFunc = (llvm::Function*) llvm::Function::Create(
        llvm::FunctionType::get(
            llvm::IntegerType::getVoidTy(*Context),
            {llvm::Type::getDoubleTy(*Context), llvm::Type::getInt32Ty(*Context)},
            false
        ), 
        llvm::Function::ExternalLinkage,
        "print",
        Module.get()
    );
    */

    // create main function
    MainFunc = llvm::Function::Create(
        llvm::FunctionType::get(llvm::Type::getInt32Ty(*Context), false),
        llvm::Function::InternalLinkage,
        "start",
        *Module
    );
    
    MainBB = llvm::BasicBlock::Create(*Context, "entry", MainFunc);
    Builder->SetInsertPoint(MainBB);

    for (unsigned long i = 0; i < program.size(); i++)
        program.at(i)->genCode();
    
    Builder->CreateRet(llvm::ConstantInt::get(llvm::IntegerType::getInt32Ty(*Context), 0));
    llvm::verifyFunction(*MainFunc);
    FPM->run(*MainFunc);
    Module->print(llvm::errs(), 0);

    auto TSM = llvm::orc::ThreadSafeModule(std::move(Module), std::move(Context));
    ExitOnErr(JIT->addModule(std::move(TSM)));

    std::cout << "looking up main" << std::endl;
    auto Sym = ExitOnErr(JIT->lookup("start"));
    auto plsmMainFunc = (int(*)()) Sym.getAddress();
    
    std::cout << "executing main ..." << std::endl;
    std::cout << "mainFunc result: " << plsmMainFunc() << std::endl;
    
    Module.release();
    Builder.release();
    Context.release();
    JIT.release();
    FPM.release();

    std::cout << "returning 0 now..." << std::endl;
    return 0;
}
