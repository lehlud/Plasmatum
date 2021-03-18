#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

typedef long double num;

class ExprAST {
public:
    virtual ~ExprAST() = default;
    virtual num eval() = 0;
};

class NumExprAST : public ExprAST {
private:
    num val;
public:
    NumExprAST(num val) : val(val) {}
    num eval() override;
};


class VarExprAST : public ExprAST {
private:
    std::string id;
public:
    VarExprAST(std::string id) : id(id) {}
    std::string getId() { return id; }
    num eval() override;
};

#define ADD_OP  10
#define SUB_OP  11
#define MUL_OP  12
#define DIV_OP  13
#define MOD_OP  14
#define POW_OP  15

#define OR_OP   30
#define AND_OP  31
#define NEQ_OP  32
#define EQ_OP   33
#define GR_OP   34
#define LO_OP   35
#define GREQ_OP 36
#define LOEQ_OP 37


class BinExprAST : public ExprAST {
private:
    int op;
    ExprAST *left, *right;
public:
    BinExprAST(int op, ExprAST *left, ExprAST *right)
        : op(op), left(std::move(left)), right(std::move(right)) {}
    num eval() override;
};

class CallExprAST : public ExprAST {
private:
    std::string id;
    std::vector<ExprAST*> *args;
public:
    CallExprAST(VarExprAST *varExpr, std::vector<ExprAST*> *args)
        : id(varExpr->getId()), args(std::move(args)) {}
    CallExprAST(VarExprAST *varExpr, std::vector<VarExprAST*> *args)
        : id(varExpr->getId()) {
            this->args = new std::vector<ExprAST*>();
            this->args->insert(this->args->end(), args->begin(), args->end());
        }
    num eval() override;
};

class IfExprAST : public ExprAST {
private:
    ExprAST *cond, *exprTrue, *exprFalse;
public:
    IfExprAST(ExprAST *cond, ExprAST *exprTrue, ExprAST *exprFalse)
        :   cond(std::move(cond)), exprTrue(std::move(exprTrue)), 
            exprFalse(std::move(exprFalse)){}
    num eval() override;
};


class StmtAST {
public:
    virtual ~StmtAST() = default;
    virtual int exec() = 0;
};


class BlockStmtAST : public StmtAST {
private:
    std::vector<StmtAST*> *stmts;
public:
    BlockStmtAST(std::vector<StmtAST*> *stmts) : stmts(std::move(stmts)) {}
    int exec() override;
};

class IfStmtAST : public StmtAST {
private:
    ExprAST *cond;
    StmtAST *ifBody, *elseBody;
public:
    IfStmtAST(ExprAST *cond, StmtAST *ifBody, StmtAST *elseBody)
        :   cond(std::move(cond)), ifBody(std::move(ifBody)),
            elseBody(elseBody ? std::move(elseBody) : 0) {}

    ExprAST *getCond() { return cond; }
    StmtAST *getIfBody() { return ifBody; }
    StmtAST *getElseBody() { return elseBody; }
    
    int exec() override;
};


class ForStmtAST : public StmtAST {
private:
    ExprAST *min, *max;
    std::string counterId;
    StmtAST *body;
public:
    ForStmtAST(ExprAST *min, ExprAST *max, VarExprAST *counter, StmtAST *body)
        :   min(std::move(min)), max(std::move(max)),
            counterId(counter->getId()), body(std::move(body)) {}
    int exec() override;
};


class WhileStmtAST : public StmtAST {
private:
    ExprAST *cond;
    StmtAST *ifBody, *elseBody;
public:
    WhileStmtAST(ExprAST *cond, StmtAST *ifBody, StmtAST *elseBody)
        :   cond(std::move(cond)), ifBody(std::move(ifBody)),
            elseBody(elseBody ? std::move(elseBody) : 0) {}
    int exec() override;
};


class OutputStmtAST : public StmtAST {
private:
    int prodNL;
    ExprAST *val;
public:
    OutputStmtAST(int prodNL, ExprAST *val)
        : prodNL(prodNL), val(val ? std::move(val) : 0) {}
    int exec() override;
};

class FDefStmtAST : public StmtAST {
private:
    std::string id;
    std::vector<std::string> *args;
    ExprAST *body;
public:
    FDefStmtAST(VarExprAST *varExpr, std::vector<VarExprAST*> *args, ExprAST *body)
        : id(varExpr->getId()), body(std::move(body)) {
            this->args = new std::vector<std::string>();
            for (unsigned long i = 0; i < args->size(); i++)
                this->args->push_back(args->at(i)->getId());
        }
    int exec() override;
};

class DeclStmtAST : public StmtAST {
private:
    std::string id;
    ExprAST *val;
public:
    DeclStmtAST(VarExprAST *varExpr, ExprAST *val)
        : id(varExpr->getId()), val(std::move(val)) {}
    int exec() override;
};

class AssignStmtAST : public StmtAST {
private:
    std::string id;
    ExprAST *val;
public:
    AssignStmtAST(VarExprAST *varExpr, ExprAST *val)
        : id(varExpr->getId()), val(std::move(val)) {}
    int exec() override;
};

class UndefStmtAST : public StmtAST {
private:
    std::string id;
public:
    UndefStmtAST(VarExprAST *varExpr) : id(varExpr->getId()) {}
    int exec() override;
};

class BlockExprAST : public ExprAST {
private:
    BlockStmtAST *body;
    ExprAST *result;
public:
    BlockExprAST(BlockStmtAST *body, ExprAST *result)
        : body(std::move(body)), result(std::move(result)) {}
    num eval() override;
};


class Function {
private:
    std::string id;
    std::vector<std::string> *args;
    ExprAST *body;
public:
    Function(std::string id, std::vector<std::string> *args, ExprAST *body)
        : id(std::move(id)), args(std::move(args)), body(std::move(body)) {}
    num exec(std::vector<ExprAST*> *inputArgs);
};


std::string readFile(std::string name);

int execProgram(std::vector<StmtAST*> *program);

std::map<std::string, num> mergeScopes(std::map<std::string, num> orig, std::map<std::string, num> curr);

#endif
