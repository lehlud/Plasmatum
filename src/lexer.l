
%{
#include "y.tab.h"
#include <stdio.h>
%}

%%

"...".*"..."?                   {/* IGNORE COMMENT */}

[0-9]*"."?[0-9]+                {yylval.num_v = atof(yytext); return NUM;}
"+"                             {return ADD;}
"-"                             {return SUB;}
"*"                             {return MUL;}
"/"                             {return DIV;}
"|"                             {return ABS;}
\n|EOF                          {return EOL;}

[ \f\v\t]                       {/* IGNORE WHITESPACES */}
.                               {printf("Unexpected character: %c\n", *yytext);}

%%

