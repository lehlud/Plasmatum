#include <map>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>

#include "utils.hh"


std::string readFile(std::string name) {
    std::ifstream file;
    file.open(name);
    std::stringstream sstream;
    sstream << file.rdbuf();
    return sstream.str();
}

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

std::map<std::string, Function*> functions;
std::map<std::string, num> varValues;

num NumExprAST::eval() {
    return val;
}

num VarExprAST::eval() {
    if (!varValues.count(id)) {
        std::cout << "Variable \'" << id << "\' is not defined." << std::endl;
        exit(1);
    }
    return varValues[id];
}

num BinExprAST::eval() {
    switch (op) {
    case ADD_OP:    return left->eval() + right->eval();
    case SUB_OP:    return left->eval() - right->eval();
    case MUL_OP:    return left->eval() * right->eval();
    case DIV_OP:    return left->eval() / right->eval();
    case MOD_OP:    return fmod(left->eval(), right->eval());
    case POW_OP:    return pow(left->eval(), right->eval());

    case OR_OP:     return left->eval() || right->eval();
    case AND_OP:    return left->eval() && right->eval();
    case NEQ_OP:    return left->eval() != right->eval();
    case EQ_OP:     return left->eval() == right->eval();
    case GR_OP:     return left->eval() > right->eval();
    case LO_OP:     return left->eval() < right->eval();
    case GREQ_OP:   return left->eval() >= right->eval();
    case LOEQ_OP:   return left->eval() <= right->eval();
    }
    std::cout << "Invalid operator occurred while evaluating binary expression." << std::endl;
    exit(1);
}

num CallExprAST::eval() {
    if (!functions.count(id)) {
        std::cout << "Function \'" << id << "\' is not defined." << std::endl;
        exit(1);
    }
    return functions[id]->exec(std::move(args));
}

num IfExprAST::eval() {
    if (cond->eval()) return exprTrue->eval();
    return exprFalse->eval();
}

num BlockExprAST::eval() {
    auto tmpVars = varValues;
    body->exec();
    varValues = mergeScopes(tmpVars, varValues);
    return result->eval();
}


int BlockStmtAST::exec() {
    int res = 0;
    auto tmpVars = varValues;
    for (unsigned long i = 0; i < stmts->size(); i++) {
        res = stmts->at(i)->exec();
        if (res) break;
    }
    varValues = mergeScopes(tmpVars, varValues);
    return res;
}

int IfStmtAST::exec() {
    if (cond->eval()) return ifBody->exec();
    return elseBody->exec();
}

int ForStmtAST::exec() {
    while (varValues[counterId] < max->eval()) {
        if (body->exec()) break;
        varValues[counterId] = ((int) varValues[counterId]) + 1;
    }
    return 0;
}

int WhileStmtAST::exec() {
    if (!cond->eval() && elseBody) return elseBody->exec();
    while (cond->eval())
        if (ifBody->exec()) break;
    return 0;
}


int OutputStmtAST::exec() {
    if (val) std::cout << val->eval();
    if (prodNL) std::cout << std::endl;
    return 0;
}

int FDefStmtAST::exec() {
    if (functions.count(id)) {
        std::cout << "Function \'" << id << "\' is already defined." << std::endl;
        exit(1);
    }
    functions[id] = new Function(id, args, body);
    return 0;
}

int DeclStmtAST::exec() {
    if (varValues.count(id)) {
        std::cout << "Variable \'" << id << "\' is already defined." << std::endl;
        exit(1);
    }
    varValues[id] = val->eval();
    return 0;
}

int AssignStmtAST::exec() {
    if (!varValues.count(id)) {
        std::cout << "Variable \'" << id << "\' is not defined." << std::endl;
        exit(1);
    }
    varValues[id] = val->eval();
    return 0;
}

int UndefStmtAST::exec() {
    if (!varValues.count(id)) {
        std::cout << "Variable \'" << id << "\' is not defined." << std::endl;
        exit(1);
    }
    varValues.erase(id);
    return 0;
}

int SDefStmtAST::exec() {
    return 0;
}

num Function::exec(std::vector<ExprAST*> *inputArgs) {
    if (args->size() > inputArgs->size()) {
        std::cout << "Got too many arguments for function \'" << id
            << "\'(exected: " << args->size() << ", got: "
            << inputArgs->size() << ")." << std::endl;
        exit(1);
    } else if (args->size() < inputArgs->size()) {
        std::cout << "Got too few arguments for function \'" << id
            << "\'(exected: " << args->size() << ", got: "
            << inputArgs->size() << ")." << std::endl;
        exit(1);
    }

    std::map<std::string, num> funVars;

    for (unsigned long i = 0; i < args->size(); i++) {
        if (funVars.count(args->at(i))) {
            std::cout << "Variable \'" << args->at(i)
                << "\' is already defined." << std::endl;
            exit(1);
        }
        funVars[args->at(i)] = inputArgs->at(i)->eval();
    }

    auto tmpVars = varValues;
    varValues = funVars;
    num result = body->eval();
    varValues = tmpVars;
    return result;
}

int execProgram(std::vector<StmtAST*> *program) {
    for (unsigned long i = 0; i < program->size(); i++)
        if (program->at(i)->exec()) return 1;
    return 0;
}

std::map<std::string, num> mergeScopes(std::map<std::string, num> orig, 
                                         std::map<std::string, num> curr) {
    for (auto it = curr.begin(); it != curr.end(); it++)
        if (!orig.count(it->first)) curr.erase(it->first);
    return curr;
}

