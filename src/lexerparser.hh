#pragma once

#include <string>

#include "ast.hh"

namespace Plasmatum
{

    enum TokenType
    {
        INT,   // integer literal
        FLOAT, // float literal
        ID,    // identifier
        PO,    // '('
        PC,    // ')'
        QM,    // '?'
        COL,   // ':'
        ASS,   // '='
        ADD,   // '+'
        SUB,   // '-'
        MUL,   // '*'
        DIV,   // '/'
        MOD,   // '%'
        ARR,   // '->'
    };

    typedef struct Token_t
    {
        TokenType tt;
        std::string val;
    } Token;

    class Lexer
    {
    private:
        size_t idx = 0;
        std::string text;

    public:
        Lexer(const std::string &text) : text(text) {}
        Token nextT();
    };

    class Parser
    {
    private:
        Lexer lexer;
        Token tmpT;

    public:
        Parser(const Lexer &lexer) : lexer(lexer) {}

        AST::Expr *parseExpr();
    };

}
