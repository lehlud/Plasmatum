%{
    #include <map>
    #include <vector>
    #include <string>

    #include "AST/Expr.h"
    #include "AST/Stmt.h"
    #include "AST/Type.h"

    extern int errors;
    extern std::vector<Stmt *> program;

    extern int yylineno;
    extern "C" int yylex();
    void yyerror(const char*);
%}

%defines
%define parse.error verbose

%token
    PAREN_OPEN          "("
    PAREN_CLOSE         ")"
    BRACKET_OPEN        "["
    BRACKET_CLOSE       "]"
    BRACE_OPEN          "{"
    BRACE_CLOSE         "}"
    COMMA               ","
    COLON               ":"
    SEMICOLON           ";"
    DOT                 "."
    EQUAL               "="
    ADD                 "+"
    SUB                 "-"
    MUL                 "*"
    DIV                 "/"
    MOD                 "%"
    NOT                 "!"
    AND                 "&&"
    OR                  "||"
    LESS                "<"
    GREATER             ">"
    LESS_EQUAL          "<="
    GREATER_EQUAL       ">="
    EQUAL_EQUAL         "=="
    NOT_EQUAL           "!="
    SINGLE_ARROW        "->"
    DOUBLE_ARROW        "=>"
    AS                  "as"
    DEFINE              "define"
;

%token END 0

%union {
    std::string *text;

    Expr *expr;
    Stmt *stmt;

    std::vector<Stmt *> *stmts;
    std::vector<Expr *> *call_params;

    TypeRef *type_ref;

    std::pair<std::string, TypeRef *> *identifier_with_type;
    std::vector<std::pair<std::string, TypeRef *>> *definition_params;
}

%token <text> IDENTIFIER   "identifier"
%token <expr> LITERAL_EXPR "literal expression"

%type <expr>
    expr            "expression"
    bin_expr        "binary expression"
    call_expr       "call expression"
    block_expr      "block expression"
    function_expr   "function expression"
;

%type <stmt>
    stmt            "statement"
    top_stmt        "top-level statement"
    block           "block"
;

%type <stmts>
    stmts           "statements"
    top_stmts       "top-level statements"
;

%type <call_params> call_params "call parameters"

%type <type_ref> type_ref "type reference"

%type <identifier_with_type> identifier_with_type "identifier with type"
%type <definition_params> definition_params "definition parameters"

%start program

// https://en.wikipedia.org/wiki/Order_of_operations#Programming_languages
%left DOUBLE_ARROW
%left ADD SUB
%left MUL DIV MOD
%left PAREN_OPEN PAREN_CLOSE

%%

program
    : top_stmts                             {program = *$1; delete $1;}
    ;

stmt: top_stmt                              {$$ = $1;}
    | expr SEMICOLON                        {$$ = new ExprStmt($1);}
    | block                                 {$$ = $1;}
    ;

top_stmt
    : DEFINE IDENTIFIER AS expr SEMICOLON   {$$ = new DefineStmt(*$2, $4); delete $2;}
    ;

block
    : BRACE_OPEN stmts BRACE_CLOSE          {$$ = new BlockStmt(*$2); delete $2;}
    ;

stmts
    : stmt                                  {$$ = new std::vector<Stmt *>({$1});}
    | stmts stmt                            {$1->push_back($2); $$ = $1;}
    ;

top_stmts
    : top_stmt                              {$$ = new std::vector<Stmt *>({$1});}
    | top_stmts top_stmt                    {$1->push_back($2); $$ = $1;}
    ;

expr: IDENTIFIER                            {$$ = new IdExpr(*$1); delete $1;}
    | LITERAL_EXPR                          {$$ = $1;}
    | call_expr                             {$$ = $1;}
    | bin_expr                              {$$ = $1;}
    | block_expr                            {$$ = $1;}
    | function_expr                         {$$ = $1;}
    ;

bin_expr
    : expr ADD expr                         {$$ = new BinExpr(BinExpr::Op::ADD, $1, $3);}
    | expr SUB expr                         {$$ = new BinExpr(BinExpr::Op::SUB, $1, $3);}
    | expr MUL expr                         {$$ = new BinExpr(BinExpr::Op::MUL, $1, $3);}
    | expr DIV expr                         {$$ = new BinExpr(BinExpr::Op::DIV, $1, $3);}
    | expr MOD expr                         {$$ = new BinExpr(BinExpr::Op::MOD, $1, $3);}
    | PAREN_OPEN expr PAREN_CLOSE           {$$ = $2;}
    ;

call_expr
    : expr PAREN_OPEN PAREN_CLOSE               {$$ = new CallExpr($1, std::vector<Expr *>());}
    | expr PAREN_OPEN call_params PAREN_CLOSE   {$$ = new CallExpr($1, *$3); delete $3;}
    ;

block_expr
    : PAREN_OPEN stmts PAREN_CLOSE DOUBLE_ARROW expr    {$$ = new BlockExpr(*$2, $5); delete $2;}
    ;

call_params
    : expr                                  {$$ = new std::vector<Expr *>({$1});}
    | call_params COMMA expr                {$1->push_back($3); $$ = $1;}
    ;

type_ref
    : IDENTIFIER                            {$$ = new TypeRef(std::string(*$1)); delete $1;}
    ;

identifier_with_type
    : IDENTIFIER COLON type_ref             {$$ = new std::pair<std::string, TypeRef *>(*$1, $3); delete $1;}
    ;

definition_params
    : identifier_with_type                            {$$ = new std::vector<std::pair<std::string, TypeRef *>>({*$1}); delete $1;}
    | definition_params COMMA identifier_with_type    {$1->push_back(*$3); $$ = $1; delete $3;}
    ;

function_expr
    : PAREN_OPEN PAREN_CLOSE DOUBLE_ARROW expr                      {$$ = new FunctionExpr(std::vector<std::pair<std::string, TypeRef *>>(), $4);}
    | PAREN_OPEN definition_params PAREN_CLOSE DOUBLE_ARROW expr    {$$ = new FunctionExpr(*$2, $5); delete $2;}
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "line %d: %s\n", yylineno, s);
    errors = 1;
}

