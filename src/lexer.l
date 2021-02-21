
%{
#include "y.tab.h"
#include <stdio.h>
%}

%%

"...".*                         {/* IGNORE COMMENT */}
"../"(.|"\n")*"/.."             {/* IGNORE COMMENT */}

"-"?[0-9]*"."?[0-9]+            {yylval.num_v = atof(yytext); return NUM;}
"**"                            {return POW;}
"+"                             {return ADD;}
"-"                             {return SUB;}
"*"                             {return MUL;}
"/"                             {return DIV;}
"%"                             {return MOD;}
"|"                             {return ABS;}

"("                             {return BR_O;}
")"                             {return BR_C;}

"\n"|"\r"                       {return EOL;}

[ \f\v\t]                       {/* IGNORE WHITESPACES */}
.                               {printf("unexpected character: '%d'\n", *yytext);}

%%
