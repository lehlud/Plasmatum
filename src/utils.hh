#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/Orc/TargetProcessControl.h"


class Expr {
public:
    virtual ~Expr() = default;
    virtual llvm::Value *genCode() = 0;
};

class NumExpr : public Expr {
private:
    double val;
public:
    NumExpr(double val) : val(val) {}
    llvm::Value *genCode() override;
};


class VarExpr : public Expr {
private:
    std::string id;
public:
    VarExpr(std::string id) : id(id) {}
    std::string getId() { return id; }
    llvm::Value *genCode() override;
};

#define ADD_OP  10
#define SUB_OP  11
#define MUL_OP  12
#define DIV_OP  13
#define MOD_OP  14
#define POW_OP  15

#define OR_OP   30
#define AND_OP  31
#define XOR_OP  32
#define EQ_OP   33
#define GR_OP   34
#define LO_OP   35
#define GREQ_OP 36
#define LOEQ_OP 37


class BinExpr : public Expr {
private:
    int op;
    Expr *left, *right;
public:
    BinExpr(int op, Expr *left, Expr *right)
        : op(op), left(std::move(left)), right(std::move(right)) {}
    llvm::Value *genCode() override;
};


class Stmt {
public:
    virtual ~Stmt() = default;
    virtual llvm::Value *genCode() = 0;
};


class BlockStmt : public Stmt {
private:
    std::vector<Stmt*> *stmts;
public:
    BlockStmt(std::vector<Stmt*> *stmts)
        : stmts(std::move(stmts)) {}
    llvm::Value *genCode() override;
};


class IfStmt : public Stmt {
private:
    Expr *cond;
    Stmt *ifPart, *elsePart;
public:
    IfStmt(Expr *cond, Stmt *ifPart, Stmt *elsePart)
        : cond(std::move(cond)), ifPart(std::move(ifPart)),
            elsePart(std::move(elsePart)) {}
    llvm::Value *genCode() override;
};


class ForStmt : public Stmt {
private:
    Expr *min, *max;
    VarExpr *counter;
    Stmt *body;
public:
    ForStmt(Expr *min, Expr *max, VarExpr *counter, Stmt *body)
        : min(std::move(min)), max(std::move(max)),
            counter(counter), body(std::move(body)) {}
    llvm::Value *genCode() override;
};


class WhileStmt : public Stmt {
private:
    Expr *cond;
    Stmt *body;
public:
    WhileStmt(Expr *cond, Stmt *body)
        : cond(std::move(cond)), body(std::move(body)) {}
    llvm::Value *genCode() override;
};


class OutputStmt : public Stmt {
private:
    bool prodNL;
    Expr *val;
public:
    OutputStmt(bool prodNL, Expr *val)
        : prodNL(prodNL), val(std::move(val)) {}
    llvm::Value *genCode() override;
};

class FDefStmt : public Stmt {
private:
    std::string id;
    std::vector<std::string> *args;
    Expr *body;
public:
    FDefStmt(std::string id, std::vector<std::string> *args, Expr *body)
        : id(std::move(id)), args(std::move(args)), body(std::move(body)) {}
    llvm::Value *genCode() override;
};

class DeclStmt : public Stmt {
private:
    std::string id;
    Expr *val;
public:
    DeclStmt(VarExpr *varExpr, Expr *val)
        : id(varExpr->getId()), val(std::move(val)) {}
    llvm::Value *genCode() override;
};

class AssignStmt : public Stmt {
private:
    std::string id;
    Expr *val;
public:
    AssignStmt(VarExpr *varExpr, Expr *val)
        : id(varExpr->getId()), val(std::move(val)) {}
    llvm::Value *genCode() override;
};

class UndefStmt : public Stmt {
private:
    std::string id;
public:
    UndefStmt(VarExpr *varExpr) : id(varExpr->getId()) {}
    llvm::Value *genCode() override;
};


llvm::BasicBlock *genBBStmtCode(Stmt *stmt);
void logError(std::string s);
std::string readFile(std::string name);


#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// 'extern "C"' is used to avoid name mangling
extern "C" DLLEXPORT void print(double num, int prodNL);

int execProgram(std::vector<Stmt*> program);

#define emptyBB llvm::BasicBlock::Create(*Context, "empty")


class PlsmJIT {
private:
    std::unique_ptr<llvm::orc::TargetProcessControl> TPC;
    std::unique_ptr<llvm::orc::ExecutionSession> ES;

    llvm::DataLayout DL;
    llvm::orc::MangleAndInterner Mangle;

    llvm::orc::RTDyldObjectLinkingLayer ObjectLayer;
    llvm::orc::IRCompileLayer CompileLayer;

    llvm::orc::JITDylib &MainJD;

public:
    PlsmJIT(std::unique_ptr<llvm::orc::TargetProcessControl> TPC,
            std::unique_ptr<llvm::orc::ExecutionSession> ES,
            llvm::orc::JITTargetMachineBuilder JTMB, llvm::DataLayout DL)
        :   TPC(std::move(TPC)), ES(std::move(ES)), DL(std::move(DL)),
            Mangle(*this->ES, this->DL),
            ObjectLayer(*this->ES, []() { return std::make_unique<llvm::SectionMemoryManager>(); }),
            CompileLayer(*this->ES, ObjectLayer, std::make_unique<llvm::orc::ConcurrentIRCompiler>(std::move(JTMB))),
            MainJD(this->ES->createBareJITDylib("<<main>>")) {
                MainJD.addGenerator(
                    llvm::cantFail(
                        llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(
                            DL.getGlobalPrefix())));
            }

    ~PlsmJIT() {
        if (auto Err = ES->endSession())
        ES->reportError(std::move(Err));
    }

    static llvm::Expected<std::unique_ptr<PlsmJIT>> Create() {
        auto SSP = std::make_shared<llvm::orc::SymbolStringPool>();
        auto TPC = llvm::orc::SelfTargetProcessControl::Create(SSP);
        if (!TPC) return TPC.takeError();

        auto ES = std::make_unique<llvm::orc::ExecutionSession>(std::move(SSP));
        
        llvm::orc::JITTargetMachineBuilder JTMB((*TPC)->getTargetTriple());

        auto DL = JTMB.getDefaultDataLayoutForTarget();
        if (!DL) return DL.takeError();

        return std::make_unique<PlsmJIT>(std::move(*TPC), std::move(ES), std::move(JTMB), std::move(*DL));
    }

    const llvm::DataLayout &getDataLayout() const { return DL; }

    llvm::orc::JITDylib &getMainJITDylib() { return MainJD; }

    llvm::Error addModule(llvm::orc::ThreadSafeModule TSM, llvm::orc::ResourceTrackerSP RT = nullptr) {
        if (!RT) RT = MainJD.getDefaultResourceTracker();
        return CompileLayer.add(RT, std::move(TSM));
    }

    llvm::Expected<llvm::JITEvaluatedSymbol> lookup(llvm::StringRef Name) {
        return ES->lookup({&MainJD}, Mangle(Name.str()));
    }
};



#endif
