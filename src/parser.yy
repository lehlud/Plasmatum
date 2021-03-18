%{
#include <cstdio>
#include <cstdlib>
#include "utils.hh"

extern "C" int yylex();
int yy_scan_string(const char*);
void yyerror(const char*);

std::vector<StmtAST*> *program;

int errors;
%}

%defines
%define parse.error verbose


%token
    POW             "**"
    ADD             "+"
    SUB             "-"
    MUL             "*"
    DIV             "/"
    MOD             "%"

    EOL             "new line"

    BR_O            "("
    BR_C            ")"

    PECHO           "echo"
    STDOUT          "stdout"

    DEF             "def"
    SET             "set"
    UNDEF           "undef"
    FDEF            "fdef"
    ON              "on"

    IF              "if"
    FOR             "for"
    WHILE           "while"

    ASSIGN          "="
    ARR             "->"
    QM              "?"
    COL             ":"
    COMMA           ","

    GR              ">"
    LO              "<"
    EQ              "=="
    NEQ             "!="
    GREQ            ">="
    LOEQ            "<="

    NOT             "not"
    AND             "and"
    OR              "or"
;


%token END 0

%union {
    ExprAST *expr;
    StmtAST *stmt;
    IfStmtAST *ifstmt;
    VarExprAST *vexpr;
    BlockStmtAST *bstmt;
    std::vector<StmtAST*> *stmts;
    std::vector<ExprAST*> *cparams;
    std::vector<VarExprAST*> *fparams;
}

%token <expr> VAL       "value expression"
%token <vexpr> ID       "identifier"

%type <expr>
    expr                    "expression"
    call_expr               "function call"
;

%type <stmt>
    fun_decl                "function declaration"
    assigment               "assigment"
    statement               "statement"
    declaration             "declaration"
    for_stmt                "for loop"
    while_stmt              "while loop"
    output_stmt             "output statement"
;

%type <ifstmt> if_stmt      "if statement"
%type <bstmt> block_stmt    "statement block"
%type <stmts>
    statements    "statement list"
    fake_stmts    "pseudo statement list"
;

%type <fparams> fun_params  "function parameter list"
%type <cparams> call_params "function call parameter list"

%left QM
%left COL

%left OR
%left AND
%left NOT
%left GR LO EQ NEQ GREQ LOEQ
%left ADD SUB
%left MUL DIV MOD
%left POW
//%left ARR
%left BR_O BR_C

%start program

%%

program
    : END                                       {program = new std::vector<StmtAST*>();}
    | newline END                               {program = new std::vector<StmtAST*>();}
    | statements END                            {program = std::move($<stmts>1);}
    | newline statements END                    {program = std::move($<stmts>2);}
    ;

fake_stmts
    : statement                                 {$$ = new std::vector<StmtAST*>(); if($<stmt>1) $$->push_back($<stmt>1);}
    | fake_stmts newline statement              {$$ = $1; if($<stmt>3) $$->push_back($<stmt>3);}
    ;

statements
    : fake_stmts                                {$$ = $1;}
    | fake_stmts newline                        {$$ = $1;}
    ;

statement
    : if_stmt                                   {$$ = $1;}
    | for_stmt                                  {$$ = $1;}
    | fun_decl                                  {$$ = $1;}
    | assigment                                 {$$ = $1;}
    | while_stmt                                {$$ = $1;}
    | declaration                               {$$ = $1;}
    | output_stmt                               {$$ = $1;}
    ;

output_stmt
    : PECHO                                     {$$ = new OutputStmtAST(1, 0); }
    | STDOUT                                    {$$ = 0;}
    | PECHO expr                                {$$ = new OutputStmtAST(1, $2);}
    | STDOUT expr                               {$$ = new OutputStmtAST(0, $2);}
    ;

if_stmt
    : IF expr QM statement                      {$$ = new IfStmtAST($2, $4, 0);}
    | IF expr QM statement COL statement        {$$ = new IfStmtAST($2, $4, $6);}
//    | IF expr QM statement newline COL statement{$$ = new IfStmtAST($2, $4, $7);}
    ;

while_stmt
    : WHILE expr QM statement                   {$$ = new WhileStmtAST($2, $4, 0);}
    | WHILE expr QM statement COL statement     {$$ = new WhileStmtAST($2, $4, $6);}
    ;

for_stmt
    : FOR ID ARR expr statement                 {$$ = new ForStmtAST(new NumExprAST(0), $4, $2, $5);}
    | FOR ID ASSIGN expr ARR expr statement     {$$ = new ForStmtAST($4, $6, $2, $7);}
    ;

fun_params
    : ID                                        {$$ = new std::vector<VarExprAST*>(); $$->push_back($1);}
    | fun_params COMMA ID                       {$$ = $1; $$->push_back($3);}
    ;

fun_decl
    : FDEF ID ON BR_O BR_C ARR expr             {$$ = new FDefStmtAST($2, new std::vector<VarExprAST*>(), $7);}
    | FDEF ID ON BR_O fun_params BR_C ARR expr  {$$ = new FDefStmtAST($2, $5, $8);}
    ;

assigment
    : SET ID ASSIGN expr                        {$$ = new AssignStmtAST($2, $4);}
    ;

declaration
    : DEF ID ASSIGN expr                        {$$ = new DeclStmtAST($2, $4);}
    | UNDEF ID                                  {$$ = new UndefStmtAST($2);}
    ;

block_stmt
    : BR_O BR_C                                 {$$ = 0;}
    | BR_O newline BR_C                         {$$ = 0;}
    | BR_O statements BR_C                      {$$ = new BlockStmtAST($2);}
    | BR_O newline statements BR_C              {$$ = new BlockStmtAST($3);}
    ;


call_params
    : expr                                      {$$ = new std::vector<ExprAST*>(); $$->push_back($1);}
    | call_params COMMA expr                    {$$ = $1; $$->push_back($3);}
    ;

call_expr
    : ID ON BR_O BR_C                           {$$ = new CallExprAST($1, new std::vector<ExprAST*>());}
    | ID ON BR_O call_params BR_C               {$$ = new CallExprAST($1, $4);}
    ;


expr: VAL                                       {$$ = $1;}
    | ID                                        {$$ = $1;}
    | call_expr                                 {$$ = $1;}
    | BR_O expr BR_C                            {$$ = $2;}
    | expr ADD expr                             {$$ = new BinExprAST(ADD_OP, $1, $3);}
    | expr SUB expr                             {$$ = new BinExprAST(SUB_OP, $1, $3);}
    | expr MUL expr                             {$$ = new BinExprAST(MUL_OP, $1, $3);}
    | expr DIV expr                             {$$ = new BinExprAST(DIV_OP, $1, $3);}
    | expr MOD expr                             {$$ = new BinExprAST(MOD_OP, $1, $3);}
    | expr POW expr                             {$$ = new BinExprAST(POW_OP, $1, $3);}
    | expr GR expr                              {$$ = new BinExprAST(GR_OP, $1, $3);}
    | expr LO expr                              {$$ = new BinExprAST(LO_OP, $1, $3);}
    | expr EQ expr                              {$$ = new BinExprAST(EQ_OP, $1, $3);}
    | expr NEQ expr                             {$$ = new BinExprAST(NEQ_OP, $1, $3);}
    | expr GREQ expr                            {$$ = new BinExprAST(GREQ_OP, $1, $3);}
    | expr LOEQ expr                            {$$ = new BinExprAST(LOEQ_OP, $1, $3);}
    | NOT expr                                  {$$ = new BinExprAST(EQ_OP, $2, new NumExprAST(0));}
    | expr AND expr                             {$$ = new BinExprAST(AND_OP, $1, $3);}
    | expr OR expr                              {$$ = new BinExprAST(OR_OP, $1, $3);}
    | block_stmt ARR expr                       {$$ = new BlockExprAST($1, $3);}
    | expr QM expr COL expr                     {$$ = new IfExprAST($1, $3, $5);}
    | SUB expr                                  {$$ = new BinExprAST(SUB_OP, new NumExprAST(0), $2);}
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
