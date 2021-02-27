%locations

%{
#include <math.h>
#include <stdio.h>
int yylex();
int yyerror(const char *s);
%}

%token POW ADD SUB MUL DIV MOD ABS
%token EOL
%token NUM

%token BR_O BR_C

%token ECHO STDOUT

%token END 0

%union {
    double num_v;
}

%type <num_v> expr
%type <num_v> factor
%type <num_v> term

%start program

%%

program
    : statement
    | program statement
    ;

statement
    : expr newline
    | output_statement
    ;

output_statement
    : ECHO newline                  {printf("\n");}
    | ECHO expr newline             {printf("%lf\n", $2);}
    | STDOUT expr newline           {printf("%lf", $2);}
    ;

expr: factor                        {$$ = $<num_v>1;}
    | expr SUB factor               {$$ = $1 - $3;}
    | expr ADD factor               {$$ = $1 + $3;}
    ;


factor
    : term                          {$$ = $<num_v>1;}
    | factor POW term               {$$ = pow($1, $<num_v>3);}
    | factor DIV term               {$$ = $1 / $3;}
    | factor MOD term               {$$ = fmod($1, $3);}
    | factor MUL term               {$$ = $1 * $3;}
    ;

term: NUM                           {$$ = $<num_v>1;}
    | BR_O expr BR_C                {$$ = $2;}
    | ABS expr ABS                  {$$ = $2 < 0 ? $2 * -1 : $2;}
    ;


newline
    : EOL
    | END
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
