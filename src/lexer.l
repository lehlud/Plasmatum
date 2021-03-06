
%{
#include "functions.h"
#include "y.tab.h"
#include <stdio.h>

int column = 1;
static int next_column = 1;

void hande_column();
#define HC hande_column();
%}

%option noyywrap noinput nounput yylineno

%%

"...".*                         {HC /* IGNORE COMMENT */ }
"../"(.|[\n\r])*"/.."           {HC /* IGNORE COMMENT */ return EOL;}

"\'"[_a-zA-Z0-9]"\'"            {HC yylval.expr = create_char_expr(yytext[1]); return VAL;}

"-"?[0-9]*"."?[0-9]+            {HC yylval.expr = create_num_expr(atof(yytext)); return VAL;}

"true"                          {HC yylval.expr = create_bool_expr(1); return VAL;}
"false"                         {HC yylval.expr = create_bool_expr(0); return VAL;}

"**"                            {HC return POW;}
"+"                             {HC return ADD;}
"-"                             {HC return SUB;}
"*"                             {HC return MUL;}
"/"                             {HC return DIV;}
"%"                             {HC return MOD;}

"("                             {HC return BR_O;}
")"                             {HC return BR_C;}

">"                             {HC return GR;}
"<"                             {HC return LO;}
">="                            {HC return GREQ;}
"<="                            {HC return LOEQ;}
"=="                            {HC return EQ;}

"="                             {HC return ASSIGN;}

"->"                            {HC return ARR;}

"?"                             {HC return QM;}
":"                             {HC return COL;}

"char"                          {HC return CHAR;}
"bool"                          {HC return BOOL;}
"number"                        {HC return NUMBER;}

[\n\r]                          {HC return EOL;}

"echo"                          {HC return PECHO;}
"stdout"                        {HC return STDOUT;}

"def"                           {HC return DEF;}
"undef"                         {HC return UNDEF;}

"for"                           {HC return FOR;}
"while"                         {HC return WHILE;}

[_a-zA-Z][_a-zA-Z0-9]*          {HC yylval.expr = create_id_expr(strdup(yytext)); return ID;}

[ \f\v\t]                       {HC /* IGNORE WHITESPACES */}
.                               {HC printf("unexpected character: '%c'\n", *yytext);}

%%

void hande_column() {
    column = next_column;
    next_column += strlen(yytext);
}
