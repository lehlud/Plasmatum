%{
#include <cstdio>
#include <cstdlib>
#include "utils.hh"

extern "C" int yylex();
int yy_scan_string(const char*);
void yyerror(const char*);

std::vector<Stmt*> *program;

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
    Expr *expr;
    Stmt *stmt;
    IfStmt *ifstmt;
    VarExpr *vexpr;
    BlockStmt *bstmt;
    std::vector<Stmt*> *stmts;
    std::vector<Expr*> *cparams;
    std::vector<VarExpr*> *fparams;
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
%left ARR
%left BR_O BR_C
%left VAL ID

%start program

%%

program
    : END                                       {program = new std::vector<Stmt*>();}
    | newline END                               {program = new std::vector<Stmt*>();}
    | statements END                            {program = std::move($<stmts>1);}
    | newline statements END                    {program = std::move($<stmts>2);}
    ;

fake_stmts
    : statement                                 {$$ = new std::vector<Stmt*>(); if($<stmt>1) $$->push_back($<stmt>1);}
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
    : PECHO                                     {$$ = new OutputStmt(1, 0); }
    | STDOUT                                    {$$ = 0;}
    | PECHO expr                                {$$ = new OutputStmt(1, $2);}
    | STDOUT expr                               {$$ = new OutputStmt(0, $2);}
    ;

if_stmt
    : IF expr QM statement                      {$$ = new IfStmt($2, $4, 0);}
    | IF expr QM statement COL statement        {$$ = new IfStmt($2, $4, $6);}
//    | IF expr QM statement newline COL statement{$$ = new IfStmt($2, $4, $7);}
    ;

while_stmt
    : WHILE if_stmt                             {$$ = new WhileStmt($2);}
    ;

for_stmt
    : FOR ID ARR expr statement                 {$$ = new ForStmt(new NumExpr(0), $4, $2, $5);}
    | FOR ID ASSIGN expr ARR expr statement     {$$ = new ForStmt($4, $6, $2, $7);}
    ;

fun_params
    : ID                                        {$$ = new std::vector<VarExpr*>(); $$->push_back($1);}
    | fun_params COMMA ID                       {$$ = $1; $$->push_back($3);}
    ;

fun_decl
    : FDEF ID ON BR_O BR_C ARR expr             {$$ = new FDefStmt($2, new std::vector<VarExpr*>(), $7);}
    | FDEF ID ON BR_O fun_params BR_C ARR expr  {$$ = new FDefStmt($2, $5, $8);}
    ;

assigment
    : SET ID ASSIGN expr                        {$$ = new AssignStmt($2, $4);}
    ;

declaration
    : DEF ID ASSIGN expr                        {$$ = new DeclStmt($2, $4);}
    | UNDEF ID                                  {$$ = new UndefStmt($2);}
    ;

block_stmt
    : BR_O BR_C                                 {$$ = 0;}
    | BR_O newline BR_C                         {$$ = 0;}
    | BR_O statements BR_C                      {$$ = new BlockStmt($2);}
    | BR_O newline statements BR_C              {$$ = new BlockStmt($3);}
    ;


call_params
    : expr                                      {$$ = new std::vector<Expr*>(); $$->push_back($1);}
    | call_params COMMA expr                    {$$ = $1; $$->push_back($3);}
    ;

call_expr
    : ID ON BR_O BR_C                           {$$ = new CallExpr($1, new std::vector<Expr*>());}
    | ID ON BR_O call_params BR_C               {$$ = new CallExpr($1, $4);}
    ;


expr: VAL                                       {$$ = $1;}
    | ID                                        {$$ = $1;}
    | call_expr                                 {$$ = $1;}
    | BR_O expr BR_C                            {$$ = $2;}
    | expr ADD expr                             {$$ = new BinExpr(ADD_OP, $1, $3);}
    | expr SUB expr                             {$$ = new BinExpr(SUB_OP, $1, $3);}
    | expr MUL expr                             {$$ = new BinExpr(MUL_OP, $1, $3);}
    | expr DIV expr                             {$$ = new BinExpr(DIV_OP, $1, $3);}
    | expr MOD expr                             {$$ = new BinExpr(MOD_OP, $1, $3);}
    | expr POW expr                             {$$ = new BinExpr(POW_OP, $1, $3);}
    | expr GR expr                              {$$ = new BinExpr(GR_OP, $1, $3);}
    | expr LO expr                              {$$ = new BinExpr(LO_OP, $1, $3);}
    | expr EQ expr                              {$$ = new BinExpr(EQ_OP, $1, $3);}
    | expr NEQ expr                             {$$ = new BinExpr(NEQ_OP, $1, $3);}
    | expr GREQ expr                            {$$ = new BinExpr(GREQ_OP, $1, $3);}
    | expr LOEQ expr                            {$$ = new BinExpr(LOEQ_OP, $1, $3);}
    | NOT expr                                  {$$ = new BinExpr(EQ_OP, $2, new NumExpr(0));}
    | expr AND expr                             {$$ = new BinExpr(AND_OP, $1, $3);}
    | expr OR expr                              {$$ = new BinExpr(OR_OP, $1, $3);}
    | block_stmt ARR expr                       {$$ = new BlockExpr($1, $3);}
    | expr QM expr COL expr                     {$$ = new IfExpr($1, $3, $5);}
    | SUB expr                                  {$$ = new BinExpr(SUB_OP, new NumExpr(0), $2);}
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
