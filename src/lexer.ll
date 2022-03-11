%{
    extern int errors;
    extern "C" int yylex();

    #include <string>
    #include <cstdlib>
    
    #include "AST/Expr.h"
    #include "AST/Stmt.h"

    #include "parser.tab.hh"
%}

%option noyywrap nounput noinput batch yylineno

%%

[ \n\f\v\t]                       {/* IGNORE WHITESPACES */}
"../"(.|[\n\r\v\t])*"/.."       {/* IGNORE COMMENT */}

 /* keywords before identifier (priority) */
"as"                            {return AS              ;}
"define"                        {return DEFINE          ;}

[_a-zA-Z][_a-zA-Z0-9]*          {yylval.text = new std::string(yytext); return IDENTIFIER;}

"\'"[_a-zA-Z0-9]*"\'"           {yylval.expr = StringExpr::fromWithQuotes(std::string(yytext)); return LITERAL_EXPR;}
[0-9]*"."?[0-9]+                {yylval.expr = new NumExpr(std::stod(yytext)); return LITERAL_EXPR;}

"("                             {return PAREN_OPEN      ;}
")"                             {return PAREN_CLOSE     ;}
"["                             {return BRACKET_OPEN    ;}
"]"                             {return BRACKET_CLOSE   ;}
"{"                             {return BRACE_OPEN      ;}
"}"                             {return BRACE_CLOSE     ;}
","                             {return COMMA           ;}
":"                             {return COLON           ;}
";"                             {return SEMICOLON       ;}
"."                             {return DOT             ;}
"="                             {return EQUAL           ;}
"+"                             {return ADD             ;}
"-"                             {return SUB             ;}
"*"                             {return MUL             ;}
"/"                             {return DIV             ;}
"%"                             {return MOD             ;}
"!"                             {return NOT             ;}
"&&"                            {return AND             ;}
"||"                            {return OR              ;}
"<"                             {return LESS            ;}
">"                             {return GREATER         ;}
"<="                            {return LESS_EQUAL      ;}
">="                            {return GREATER_EQUAL   ;}
"=="                            {return EQUAL_EQUAL     ;}
"!="                            {return NOT_EQUAL       ;}
"++"                            {return PLUS_PLUS       ;}
"--"                            {return MINUS_MINUS     ;}
"+="                            {return PLUS_EQUAL      ;}
"-="                            {return MINUS_EQUAL     ;}
"*="                            {return MULTIPLY_EQUAL  ;}
"/="                            {return DIVIDE_EQUAL    ;}
"%="                            {return MODULO_EQUAL    ;}
"&&="                           {return AND_EQUAL       ;}
"||="                           {return OR_EQUAL        ;}
"->"                            {return SINGLE_ARROW    ;}
"=>"                            {return DOUBLE_ARROW    ;}

.                               {fprintf(stderr, "unexpected character: '%c'\n", *yytext); errors = 1;}

%%
