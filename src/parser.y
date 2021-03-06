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
%token FOR
%token ASSIGN ARR QM COL

%token GR LO EQ GREQ LOEQ NOT AND XOR OR

%token END 0

%union {
    expr *expr;
    statement *stmt;
    code_block *cblock;
}

%type <expr> expr factor rel_term term term_expr
%type <stmt> statement only_stmt
%type <stmt> decl_assign output_statement for_statement if_statement
%type <cblock> statements code_block

%start program

%%

program
    : END                                       {program = *(init_cblock(0));}
    | newline END                               {program = *(init_cblock(0));}
    | statements END                            {program = *($<cblock>1);}
    | newline statements END                    {program = *($<cblock>2);}
    | statements only_stmt END                  {program = *($<cblock>1); append_cblock(&program, $<stmt>2);}
    | newline statements only_stmt END          {program = *($<cblock>2); append_cblock(&program, $<stmt>3);}
    ;

statements
    : statement                                 {$$ = init_cblock($<stmt>1);}
    | statements statement                      {$$ = append_cblock($1, $<stmt>2);}
    ;

statement
    : only_stmt newline                         {$$ = $<stmt>1;}
    ;

only_stmt
    : expr                                      {$$ = 0;}
    | decl_assign                               {$$ = $<stmt>1;}
    | if_statement                              {$$ = $<stmt>1;}
    | for_statement                             {$$ = $<stmt>1;}
    | output_statement                          {$$ = $<stmt>1;}
    | code_block                                {$$ = cblock_to_stmt($<cblock>1);}
    ;

for_statement
    : FOR ID ARR expr code_block                {$$ = create_for_stmt($<cblock>5, $<expr>2, 0, $<expr>4);}
    | FOR ID ASSIGN expr ARR expr code_block    {$$ = create_for_stmt($<cblock>7, $<expr>2, $<expr>4, $<expr>6);}
    ;

if_statement
    : expr QM code_block                        {$$ = create_if_stmt($<expr>1, $<cblock>3, 0);}
    | expr QM code_block COL code_block         {$$ = create_if_stmt($<expr>1, $<cblock>3, $<cblock>5);}
    | expr QM code_block COL if_statement       {$$ = create_if_stmt($<expr>1, $<cblock>3, init_cblock($<stmt>5));}
    ;


code_block
    : BR_O BR_C                                 {$$ = init_cblock(0);}
    | BR_O newline BR_C                         {$$ = init_cblock(0);}
    | BR_O statements BR_C                      {$$ = $2;}
    | BR_O newline statements BR_C              {$$ = $3;}
    ;

output_statement
    : PECHO                                     {$$ = create_output(0, 1);}
    | PECHO expr                                {$$ = create_output($2, 1);}
    | STDOUT expr                               {$$ = create_output($2, 0);}
    ;

decl_assign
    : DEF ID ASSIGN expr                        {$$ = create_decl_assign($<expr>2, $<expr>4);}
    | UNDEF ID                                  {$$ = create_decl_assign($<expr>2, 0);}
    ;

expr
    : term_expr                                 {$$ = $<expr>1;}
    | expr NOT term_expr                        {$$ = create_cond(NOT_OP, $1, 0);}
    | expr AND term_expr                        {$$ = create_cond(AND_OP, $1, $3);}
    | expr XOR term_expr                        {$$ = create_cond(XOR_OP, $1, $3);}
    | expr OR term_expr                         {$$ = create_cond(OR_OP, $1, $3);}
    | expr GR term_expr                         {$$ = create_cond(GR_OP, $1, $3);}
    | expr LO term_expr                         {$$ = create_cond(LO_OP, $1, $3);}
    | expr EQ term_expr                         {$$ = create_cond(EQ_OP, $1, $3);}
    | expr GREQ term_expr                       {$$ = create_cond(GREQ_OP, $1, $3);}
    | expr LOEQ term_expr                       {$$ = create_cond(LOEQ_OP, $1, $3);}
    ;

term_expr: factor                               {$$ = $<expr>1;}
    | term_expr SUB factor                      {$$ = create_term(SUB_OP, $1, $3);}
    | term_expr ADD factor                      {$$ = create_term(ADD_OP, $1, $3);}
    ;


factor
    : rel_term                                  {$$ = $<expr>1;}
    | factor DIV rel_term                       {$$ = create_term(DIV_OP, $1, $3);}
    | factor MOD rel_term                       {$$ = create_term(MOD_OP, $1, $3);}
    | factor MUL rel_term                       {$$ = create_term(MUL_OP, $1, $3);}
    ;

rel_term
    : term                                      {$$ = $<expr>1;}
    | rel_term POW term                         {$$ = create_term(POW_OP, $1, $3);}
    ;

term: VAL                                       {$$ = $<expr>1;}
    | ID                                        {$$ = $<expr>1;}
    | BR_O expr BR_C                            {$$ = $2;}
    | BR_O CHAR BR_C term                       {$$ = create_cast(CHAR_INDEX, $4);}
    | BR_O BOOL BR_C term                       {$$ = create_cast(BOOL_INDEX, $4);}
    | BR_O NUMBER BR_C term                     {$$ = create_cast(NUM_INDEX, $4);}
    ;


newline
    : EOL
    | newline EOL
    ;

%%

int main(int argc, char **argv) {
    if (argc > 1) {
        char *tmp = read_file(argv[1]);
        if(tmp) {
            yy_scan_string(tmp);
            yyparse();
            if (errors) exit(1);
            exec_program(&program);
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
