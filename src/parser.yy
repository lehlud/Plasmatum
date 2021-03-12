%{
#include <cstdio>
#include <cstdlib>
#include "utils.hh"

extern "C" int yylex();
int yy_scan_string(const char*);
void yyerror(const char*);

std::vector<Stmt*> program;

int errors;
%}

%defines
%define parse.error verbose


%token POW ADD SUB MUL DIV MOD
%token EOL

%token BR_O BR_C BBR_O BBR_C

%token PECHO STDOUT
%token DEF SET UNDEF FDEF ON
%token FOR WHILE
%token ASSIGN ARR QM COL COMMA

%token GR LO EQ GREQ LOEQ NOT AND XOR OR

%token END 0

%union {
    Expr *expr;
    Stmt *stmt;
    VarExpr *vexpr;
    std::vector<Stmt*> *stmts;
    std::vector<std::string> *params;
}

%token <vexpr> ID
%token <expr> VAL

%type <expr> expr factor rel_term term term_expr
%type <stmt> statement only_stmt code_block
%type <stmt> decl_assign output_statement for_statement if_statement while_statement fun_decl
%type <stmts> statements
%type <params> fun_params

%start program

%%

program
    : END                                       {program = {};}
    | newline END                               {program = {};}
    | only_stmt END                             {program = {}; program.push_back(std::move($<stmt>1));}
    | newline only_stmt END                     {program = {}; program.push_back(std::move($<stmt>2));}
    | statements END                            {program = std::move(*($<stmts>1));}
    | newline statements END                    {program = std::move(*($<stmts>2));}
    | statements only_stmt END                  {program = std::move(*($<stmts>1)); program.push_back(std::move($<stmt>2));}
    | newline statements only_stmt END          {program = std::move(*($<stmts>2)); program.push_back(std::move($<stmt>3));}
    ;

statements
    : statement                                 {$$ = new std::vector<Stmt*>(); $$->push_back(std::move($<stmt>1));}
    | statements statement                      {$$ = $1; $$->push_back(std::move($<stmt>2));}
    ;

statement
    : only_stmt newline                         {$$ = $<stmt>1;}
    ;

only_stmt
    : expr                                      {$$ = 0;}
    | fun_decl                                  {$$ = $<stmt>1;}
    | decl_assign                               {$$ = $<stmt>1;}
    | if_statement                              {$$ = $<stmt>1;}
    | for_statement                             {$$ = $<stmt>1;}
    | while_statement                           {$$ = $<stmt>1;}
    | output_statement                          {$$ = $<stmt>1;}
    | code_block                                {$$ = $<stmt>1;}
    ;

for_statement
    : FOR ID ARR expr only_stmt                 {$$ = new ForStmt(0, $<expr>4, $<vexpr>2, $<stmt>5);}
    | FOR ID ASSIGN expr ARR expr only_stmt     {$$ = new ForStmt($<expr>4, $<expr>6, $<vexpr>2, $<stmt>7);}
    ;

if_statement
    : expr QM only_stmt                         {$$ = new IfStmt($<expr>1, $<stmt>3, 0);}
    | expr QM only_stmt COL only_stmt           {$$ = new IfStmt($<expr>1, $<stmt>3, $<stmt>5);}
    ;

while_statement
    : WHILE expr QM only_stmt                   {$$ = new WhileStmt($<expr>2, $<stmt>4);}
    ;


code_block
    : BR_O BR_C                                 {$$ = 0;}
    | BR_O newline BR_C                         {$$ = 0;}
    | BR_O statements BR_C                      {$$ = new BlockStmt($2);}
    | BR_O newline statements BR_C              {$$ = new BlockStmt($3);}
    ;


output_statement
    : PECHO                                     {$$ = new OutputStmt(-1, new NumExpr(0));}
    | PECHO expr                                {$$ = new OutputStmt(1, $2);}
    | STDOUT expr                               {$$ = new OutputStmt(0, $2);}
    ;

decl_assign
    : DEF ID ASSIGN expr                        {$$ = new DeclStmt($<vexpr>2, $<expr>4);}
    | SET ID ASSIGN expr                        {$$ = new AssignStmt($<vexpr>2, $<expr>4);}
    | UNDEF ID                                  {$$ = new UndefStmt($<vexpr>2);}
    ;


fun_params
    : ID                                        {$$ = new std::vector<std::string>(); $$->push_back($<vexpr>1->getId());}
    | fun_params COMMA ID                       {$$ = $1; $$->push_back($<vexpr>2->getId());}
    ;

fun_decl
    : FDEF ID ON BBR_O BBR_C ARR expr           {$$ = new FDefStmt($<vexpr>1->getId(), new std::vector<std::string>(), $<expr>7);}
    | FDEF ID ON BBR_O fun_params BBR_C ARR expr{$$ = new FDefStmt($<vexpr>1->getId(), $<params>5, $<expr>8);}
    ;

expr
    : term_expr                                 {$$ = $<expr>1;}
    | expr NOT term_expr                        {$$ = new BinExpr(EQ_OP, $1, new NumExpr(0));}
    | expr AND term_expr                        {$$ = new BinExpr(AND_OP, $1, $3);}
    | expr XOR term_expr                        {$$ = new BinExpr(XOR_OP, $1, $3);}
    | expr OR term_expr                         {$$ = new BinExpr(OR_OP, $1, $3);}
    | expr GR term_expr                         {$$ = new BinExpr(GR_OP, $1, $3);}
    | expr LO term_expr                         {$$ = new BinExpr(LO_OP, $1, $3);}
    | expr EQ term_expr                         {$$ = new BinExpr(EQ_OP, $1, $3);}
    | expr GREQ term_expr                       {$$ = new BinExpr(GREQ_OP, $1, $3);}
    | expr LOEQ term_expr                       {$$ = new BinExpr(LOEQ_OP, $1, $3);}
    ;

term_expr: factor                               {$$ = $<expr>1;}
    | term_expr SUB factor                      {$$ = new BinExpr(SUB_OP, $1, $3);}
    | term_expr ADD factor                      {$$ = new BinExpr(ADD_OP, $1, $3);}
    ;


factor
    : rel_term                                  {$$ = $<expr>1;}
    | factor DIV rel_term                       {$$ = new BinExpr(DIV_OP, $1, $3);}
    | factor MOD rel_term                       {$$ = new BinExpr(MOD_OP, $1, $3);}
    | factor MUL rel_term                       {$$ = new BinExpr(MUL_OP, $1, $3);}
    ;

rel_term
    : term                                      {$$ = $<expr>1;}
    | rel_term POW term                         {$$ = new BinExpr(POW_OP, $1, $3);}
    ;

term: VAL                                       {$$ = $<expr>1;}
    | ID                                        {$$ = $<vexpr>1;}
    | BR_O expr BR_C                            {$$ = $2;}
    ;


newline
    : EOL
    | newline EOL
    ;

%%

/*
 * This function is used to handle parse errors.
 * The error messages, especially the position may
 * be inaccurate by now.
 */
void yyerror(const char *s) {
    fprintf(stderr,"error: %s\n", s);
    errors = 1;
}
