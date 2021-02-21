%locations

%{
#include <math.h>
#include <stdio.h>
int yylex();
int yyerror(const char *s);

union plsm_dtype;
struct map;
struct list;
%}

%token POW ADD SUB MUL DIV MOD ABS
%token EOL
%token NUM

%token BR_O BR_C

%token END 0

%union {
    double num_v;
}

%type <num_v> expr

%%

program:
    | program expr newline      {printf("%lf\n", $2);}
    ;

expr: NUM                       {$$ = $<num_v>1;}
    | BR_O expr BR_C            {$$ = $2;}
    | ABS expr ABS              {$$ = $2 >= 0 ? $2 : $2 * -1;}
    | expr POW expr             {$$ = pow($1, $3);}
    | expr DIV expr             {$$ = $1 / $3;}
    | expr MOD expr             {$$ = fmod($1, $3);}
    | expr MUL expr             {$$ = $1 * $3;}
    | expr SUB expr             {$$ = $1 - $3;}
    | expr ADD expr             {$$ = $1 + $3;}
    ;


newline: EOL
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

union plsm_dtype {
    double num_v;
    struct map map_v;
    struct list list_v;
};

// MAP
struct map {
    char *key;
    union plsm_dtype value;
    struct map *next;
};

// LISTS
struct list {
    union plsm_dtype value;
    struct list *next;
};
