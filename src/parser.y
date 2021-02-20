%{
#include<stdio.h>
%}

%token ADD SUB MUL DIV ABS
%token EOL
%token NUM

%union {
    double num_v;
}

%type <num_v> expr

%%

program:                 {printf(">>> ");}
    | program expr EOL   {printf("%f\n>>> ", $2);}
    ;

expr: NUM                {$$ = $<num_v>1;}
    | ABS expr ABS       {$$ = $2 >= 0 ? $2 : $2 * -1;}
    | expr DIV expr      {$$ = $1 / $3;}
    | expr MUL expr      {$$ = $1 * $3;}
    | expr SUB expr      {$$ = $1 - $3;}
    | expr ADD expr      {$$ = $1 + $3;}
    ;

%%

int main(int argc, char **argv) {
    yyparse();
    return 0;
}

int yyerror(char *s) {
    fprintf(stderr, "error: %s\n", s);
}
