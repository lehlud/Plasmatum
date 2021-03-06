%{
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
int yylex();
int yy_scan_string(const char*);

void yyerror(const char*);

extern int yylineno;
extern int column;

code_block program;

int errors;

#define YYERROR_VERBOSE 1
%}

%token POW ADD SUB MUL DIV MOD
%token EOL
%token VAL
%token ID

%token BR_O BR_C
%token CHAR BOOL NUMBER

%token PECHO STDOUT
%token DEF UNDEF
%token EQ

%token END 0

%union {
    expr expr;
    statement *stmt;
    code_block *cblock;
}

%type <expr> expr factor term
%type <stmt> statement decl_assign output_statement
%type <cblock> statements

%start program

%%

program
    : statements END                {program = *($<cblock>1);}
    | newline statements END        {program = *($<cblock>2);}
    ;

statements
    : statement                     {$$ = init_cblock($<stmt>1);}
    | statements statement          {$$ = append_cblock($1, $<stmt>2);}
    ;

statement
    : expr newline                  {$$ = empty_stmt();}
    | output_statement newline      {$$ = $<stmt>1;}
    | decl_assign newline           {$$ = $<stmt>1;}
    ;

output_statement
    : PECHO                         {$$ = create_output(0, 1);}
    | PECHO expr                    {$$ = create_output(&($2), 1);}
    | STDOUT expr                   {$$ = create_output(&($2), 0);}
    ;

decl_assign
    : DEF ID EQ expr                {$$ = create_decl_assign(&($<expr>2), &($<expr>4));}
    | UNDEF ID                      {$$ = create_decl_assign(&($<expr>2), 0);}
    ;

expr: factor                        {$$ = $<expr>1;}
    | expr SUB factor               {$$ = create_term(SUB_OP, &($1), &($3));}
    | expr ADD factor               {$$ = create_term(ADD_OP, &($1), &($3));}
    ;


factor
    : term                          {$$ = $<expr>1;}
    | factor POW term               {$$ = create_term(POW_OP, &($1), &($3));}
    | factor DIV term               {$$ = create_term(DIV_OP, &($1), &($3));}
    | factor MOD term               {$$ = create_term(MOD_OP, &($1), &($3));}
    | factor MUL term               {$$ = create_term(MUL_OP, &($1), &($3));}
    ;

term: VAL                           {$$ = $<expr>1;}
    | ID                            {$$ = $<expr>1;}
    | BR_O expr BR_C                {$$ = $2;}
    | BR_O CHAR BR_C term           {$$ = create_cast(CHAR_INDEX, &($4));}
    | BR_O BOOL BR_C term           {$$ = create_cast(BOOL_INDEX, &($4));}
    | BR_O NUMBER BR_C term         {$$ = create_cast(NUM_INDEX, &($4));}
    ;


newline
    : EOL
    | newline EOL
    ;

%%

int main(int argc, char **argv) {
    if (argc > 1) {
        char *tmp = readfile(argv[1]);
        if(tmp) {
            yy_scan_string(tmp);
            yyparse();
            if (errors) exit(1);
            exec_code_block(&program);
        } else {
            printf("Cannot read from file \'%s\'!\n", argv[1]);
            return 1;
        }
    } else printf("Please specify a name of a file!\n");
    return 0;
}

/*
 * This function is used to handle parse errors.
 * The error messages, especially the position may
 * be inaccurate by now.
 */
void yyerror(const char *s) {
    fprintf(stderr,"error: %s in line %d:%d\n", s, yylineno, column);
    errors = 1;
}
