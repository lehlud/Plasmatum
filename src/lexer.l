
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

"\'"[_a-zA-Z0-9]"\'"            {HC
                                    plsm_dtype result;
                                    result.used = CHAR_INDEX;
                                    result.data.num_v = yytext[1];
                                    yylval.value = result;
                                    return VAL;
                                }

"-"?[0-9]*"."?[0-9]+            {HC
                                    plsm_dtype result;
                                    result.used = NUM_INDEX;
                                    result.data.num_v = atof(yytext);
                                    yylval.value = result;
                                    return VAL;
                                }

"true"                          {HC 
                                    plsm_dtype result;
                                    result.used = BOOL_INDEX;
                                    result.data.num_v = 1;
                                    yylval.value = result;
                                    return VAL;
                                }

"false"                         {HC
                                    plsm_dtype result;
                                    result.used = BOOL_INDEX;
                                    result.data.num_v = 0;
                                    yylval.value = result;
                                    return VAL;
                                }

"**"                            {HC return POW;}
"+"                             {HC return ADD;}
"-"                             {HC return SUB;}
"*"                             {HC return MUL;}
"/"                             {HC return DIV;}
"%"                             {HC return MOD;}

"("                             {HC return BR_O;}
")"                             {HC return BR_C;}

"="                             {HC return EQ;}

"char"                          {HC return CHAR;}
"bool"                          {HC return BOOL;}
"number"                        {HC return NUMBER;}

[\n\r]                          {HC return EOL;}

"echo"                          {HC return PECHO;}
"stdout"                        {HC return STDOUT;}

"def"                           {HC return DEF;}
"undef"                         {HC return UNDEF;}

[_a-zA-Z][_a-zA-Z0-9]*          {HC yylval.id = strdup(yytext); return ID;}

[ \f\v\t]                       {HC /* IGNORE WHITESPACES */}
.                               {HC printf("unexpected character: '%c'\n", *yytext);}

%%

void hande_column() {
    column = next_column;
    next_column += strlen(yytext);
}
