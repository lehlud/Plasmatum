#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>


class Expr {
public:
    virtual ~Expr() = default;
    virtual double eval() = 0;
};

class NumExpr : public Expr {
private:
    double val;
public:
    NumExpr(double val) : val(val) {}
    double eval() override;
};


class VarExpr : public Expr {
private:
    std::string id;
public:
    VarExpr(std::string id) : id(id) {}
    std::string getId() { return id; }
    double eval() override;
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


class BinExpr : public Expr {
private:
    int op;
    Expr *left, *right;
public:
    BinExpr(int op, Expr *left, Expr *right)
        : op(op), left(std::move(left)), right(std::move(right)) {}
    double eval() override;
};

class CallExpr : public Expr {
private:
    std::string id;
    std::vector<Expr*> *args;
public:
    CallExpr(VarExpr *varExpr, std::vector<Expr*> *args)
        : id(varExpr->getId()), args(std::move(args)) {}
    CallExpr(VarExpr *varExpr, std::vector<VarExpr*> *args)
        : id(varExpr->getId()) {
            this->args = new std::vector<Expr*>();
            this->args->insert(this->args->end(), args->begin(), args->end());
        }
    double eval() override;
};

class IfExpr : public Expr {
private:
    Expr *cond, *exprTrue, *exprFalse;
public:
    IfExpr(Expr *cond, Expr *exprTrue, Expr *exprFalse)
        :   cond(std::move(cond)), exprTrue(std::move(exprTrue)), 
            exprFalse(std::move(exprFalse)){}
    double eval() override;
};


class Stmt {
public:
    virtual ~Stmt() = default;
    virtual int exec() = 0;
};


class BlockStmt : public Stmt {
private:
    std::vector<Stmt*> *stmts;
public:
    BlockStmt(std::vector<Stmt*> *stmts) : stmts(std::move(stmts)) {}
    int exec() override;
};

class IfStmt : public Stmt {
private:
    Expr *cond;
    Stmt *ifBody, *elseBody;
public:
    IfStmt(Expr *cond, Stmt *ifBody, Stmt *elseBody)
        :   cond(std::move(cond)), ifBody(std::move(ifBody)),
            elseBody(elseBody ? std::move(elseBody) : 0) {}

    Expr *getCond() { return cond; }
    Stmt *getIfBody() { return ifBody; }
    Stmt *getElseBody() { return elseBody; }
    
    int exec() override;
};


class ForStmt : public Stmt {
private:
    Expr *min, *max;
    std::string counterId;
    Stmt *body;
public:
    ForStmt(Expr *min, Expr *max, VarExpr *counter, Stmt *body)
        :   min(std::move(min)), max(std::move(max)),
            counterId(counter->getId()), body(std::move(body)) {}
    int exec() override;
};


class WhileStmt : public Stmt {
private:
    Expr *cond;
    Stmt *ifBody, *elseBody;
public:
    WhileStmt(IfStmt *ifStmt)
        :   cond(std::move(ifStmt->getCond())),
            ifBody(std::move(ifStmt->getIfBody())),
            elseBody(std::move(ifStmt->getElseBody())) {}
    int exec() override;
};


class OutputStmt : public Stmt {
private:
    int prodNL;
    Expr *val;
public:
    OutputStmt(int prodNL, Expr *val)
        : prodNL(prodNL), val(val ? std::move(val) : 0) {}
    int exec() override;
};

class FDefStmt : public Stmt {
private:
    std::string id;
    std::vector<std::string> *args;
    Expr *body;
public:
    FDefStmt(VarExpr *varExpr, std::vector<VarExpr*> *args, Expr *body)
        : id(varExpr->getId()), body(std::move(body)) {
            this->args = new std::vector<std::string>();
            for (unsigned long i = 0; i < args->size(); i++)
                this->args->push_back(args->at(i)->getId());
        }
    int exec() override;
};

class DeclStmt : public Stmt {
private:
    std::string id;
    Expr *val;
public:
    DeclStmt(VarExpr *varExpr, Expr *val)
        : id(varExpr->getId()), val(std::move(val)) {}
    int exec() override;
};

class AssignStmt : public Stmt {
private:
    std::string id;
    Expr *val;
public:
    AssignStmt(VarExpr *varExpr, Expr *val)
        : id(varExpr->getId()), val(std::move(val)) {}
    int exec() override;
};

class UndefStmt : public Stmt {
private:
    std::string id;
public:
    UndefStmt(VarExpr *varExpr) : id(varExpr->getId()) {}
    int exec() override;
};


class Function {
private:
    std::string id;
    std::vector<std::string> *args;
    Expr *body;
public:
    Function(std::string id, std::vector<std::string> *args, Expr *body)
        : id(std::move(id)), args(std::move(args)), body(std::move(body)) {}
    double exec(std::vector<Expr*> *inputArgs);
};

class BlockExpr : public Expr {
private:
    BlockStmt *body;
    Expr *result;
public:
    BlockExpr(BlockStmt *body, Expr *result)
        : body(std::move(body)), result(std::move(result)) {}
    double eval() override;
};


std::string readFile(std::string name);

int execProgram(std::vector<Stmt*> *program);

std::map<std::string, double> mergeScope(std::map<std::string, double> orig, std::map<std::string, double> curr);

#endif
