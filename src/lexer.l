
%{
#include "functions.h"
#include "y.tab.h"
#include <stdio.h>
%}

%%

"...".*                         {/* IGNORE COMMENT */}
"../"(.|[\n\r])*"/.."           {/* IGNORE COMMENT */ return EOL;}

"\'"[_a-zA-Z0-9]"\'"            {
                                    plsm_dtype result;
                                    result.used = CHAR_INDEX;
                                    result.data.num_v = yytext[1];
                                    yylval.value = result;
                                    return VAL;
                                }

"-"?[0-9]*"."?[0-9]+            {
                                    plsm_dtype result;
                                    result.used = NUM_INDEX;
                                    result.data.num_v = atof(yytext);
                                    yylval.value = result;
                                    return VAL;
                                }

"true"                          {
                                    plsm_dtype result;
                                    result.used = BOOL_INDEX;
                                    result.data.num_v = 1;
                                    yylval.value = result;
                                    return VAL;
                                }

"false"                         {
                                    plsm_dtype result;
                                    result.used = BOOL_INDEX;
                                    result.data.num_v = 0;
                                    yylval.value = result;
                                    return VAL;
                                }

"**"                            {return POW;}
"+"                             {return ADD;}
"-"                             {return SUB;}
"*"                             {return MUL;}
"/"                             {return DIV;}
"%"                             {return MOD;}

"("                             {return BR_O;}
")"                             {return BR_C;}

"char"                          {return CHAR;}
"bool"                          {return BOOL;}
"number"                        {return NUMBER;}

[\n\r]                          {return EOL;}

"echo"                          {return ECHO;}
"stdout"                        {return STDOUT;}

[ \f\v\t]                       {/* IGNORE WHITESPACES */}
.                               {printf("unexpected character: '%d'\n", *yytext);}

%%
