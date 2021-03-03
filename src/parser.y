%locations

%{
#include <math.h>
#include <stdio.h>
#include "functions.h"
int yylex();
int yyerror(const char *s);
%}

%token POW ADD SUB MUL DIV MOD
%token EOL
%token VAL

%token BR_O BR_C
%token CHAR BOOL NUMBER

%token ECHO STDOUT

%token END 0

%union {
    plsm_dtype value;
}

%type <value> expr
%type <value> factor
%type <value> term

%start program

%%

program
    : statements END
    | newline statements END
    ;

statements
    : statement
    | statements statement
    ;

statement
    : expr newline
    | output_statement newline
    ;

output_statement
    : ECHO                          {printf("\n");}
    | ECHO expr                     {printval($2); printf("\n");}
    | STDOUT expr                   {printval($2);}
    ;

expr: factor                        {$$ = $<value>1;}
    | expr SUB factor               {$$ = sub($1, $3);}
    | expr ADD factor               {$$ = add($1, $3);}
    ;


factor
    : term                          {$$ = $<value>1;}
    | factor POW term               {$$ = ppow($1, $<value>3);}
    | factor DIV term               {$$ = pdiv($1, $3);}
    | factor MOD term               {$$ = pmod($1, $3);}
    | factor MUL term               {$$ = mul($1, $3);}
    ;

term: VAL                           {$$ = $<value>1;}
    | BR_O expr BR_C                {$$ = $2;}
    | BR_O CHAR BR_C term           {$$ = cast($4, CHAR_INDEX);}
    | BR_O BOOL BR_C term           {$$ = cast($4, BOOL_INDEX);}
    | BR_O NUMBER BR_C term         {$$ = cast($4, NUM_INDEX);}
    ;


newline
    : EOL
    | newline EOL
    ;

%%

int main(int argc, char **argv) {
    yyparse();
    return 0;
}

int yyerror(const char *s) {
    fprintf(stderr, "error: %s\n", s);
}
