%{
#include "utils.hh"
#include "parser.tab.hh"
#include <cstdio>
#include <cstdlib>

extern int errors;
extern "C" int yylex();
%}

%option noyywrap nounput noinput batch

%%

[ \f\v\t]                       {/* IGNORE WHITESPACES */}
"...".*                         {/* IGNORE COMMENT */}
"../"(.|[\n\r\v\t])*"/.."       {/* IGNORE COMMENT */ return EOL;}

"\'"[_a-zA-Z0-9]"\'"            {yylval.expr = new NumExprAST(yytext[1]); return VAL;}

[0-9]*"."?[0-9]+                {yylval.expr = new NumExprAST(std::stod(yytext)); return VAL;}

"true"                          {yylval.expr = new NumExprAST((double) true); return VAL;}
"false"                         {yylval.expr = new NumExprAST((double) false); return VAL;}

"**"                            {return POW;}
"+"                             {return ADD;}
"-"                             {return SUB;}
"*"                             {return MUL;}
"/"                             {return DIV;}
"%"                             {return MOD;}

"("                             {return BR_O;}
")"                             {return BR_C;}

">"                             {return GR;}
"<"                             {return LO;}
"=="                            {return EQ;}
"!="                            {return NEQ;}
">="                            {return GREQ;}
"<="                            {return LOEQ;}

"not"                           {return NOT;}
"and"                           {return AND;}
"or"                            {return OR;}

"="                             {return ASSIGN;}

"->"                            {return ARR;}

"?"                             {return QM;}
":"                             {return COL;}

","                             {return COMMA;}


"echo"                          {return PECHO;}
"stdout"                        {return STDOUT;}

"def"                           {return DEF;}
"set"                           {return SET;}
"undef"                         {return UNDEF;}

"fdef"                          {return FDEF;}
"sdef"                          {return SDEF;}

"if"                            {return IF;}
"for"                           {return FOR;}
"while"                         {return WHILE;}

[_a-zA-Z][_a-zA-Z0-9]*          {yylval.expr = new VarExprAST(yytext); return ID;}

[\n\r]                          {return EOL;}

.                               {fprintf(stderr, "unexpected character: '%c'\n", *yytext); errors = 1;}

%%
