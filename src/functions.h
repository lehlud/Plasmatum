#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define NULL_INDEX  0
#define NUM_INDEX   1
#define CHAR_INDEX  2
#define BOOL_INDEX  3

#define PLSM_INDEX  1
#define TERM_INDEX  2
#define CTERM_INDEX 3
#define COND_INDEX  4
#define ID_INDEX    5

#define ADD_OP  1
#define SUB_OP  2
#define MUL_OP  3
#define DIV_OP  4
#define MOD_OP  5
#define POW_OP  6

#define GR_OP   1
#define LO_OP   2
#define EQ_OP   3
#define GREQ_OP 4
#define LOEQ_OP 5
#define OR_OP   6
#define XOR_OP  7
#define NOT_OP  8
#define AND_OP  9


#define OUTPUT_INDEX        1
#define DECL_ASS_INDEX      2
#define IF_INDEX            3
#define FOR_INDEX           4
#define CBLOCK_INDEX        5

/*
 * This union is used to store the data in the one
 * data type (which actually is a struct) that exists
 * in Plasmatum.
 */
union plsm_dtype_union {
    double num_v;
};


/*
 * This is the 'plsm_dtype', which stands for
 * 'plsm data type' and is used for the bloaty
 * dynamic typing mess.
 */
typedef struct plsm_dtype {
    int used;
    union plsm_dtype_union data;
} plsm_dtype;


plsm_dtype incr(plsm_dtype);
plsm_dtype decr(plsm_dtype);
/*
 * If you want to know why this is bloated, look
 * at the 'function.c' source file.
 */
plsm_dtype calc(int, plsm_dtype, plsm_dtype);

plsm_dtype cast(plsm_dtype, int);

typedef struct expression expr;

void printval(plsm_dtype val);

/*
 * This struct is used for saving variables and
 * their values in a inefficient iterative way.
 * 
 * It has a 'size' attribute because you cannot
 * get the size of an array that easily in C.
 * 
 * char**       -> array of char arrays
 * plsm_dtype   -> array of plsm_dtype values
 */

typedef struct map_pair {
    char *key;
    plsm_dtype value;
} map_pair;

typedef struct map {
    map_pair **pairs;
    unsigned long size;
} map;

map* map_init();
plsm_dtype map_get(map*, char*);
void map_set(map*, char*, plsm_dtype);
void map_remove(map*, char*);


/*
 * The following structs and unions are used
 * to avoid executing the code while parsing
 * and first generating an abstract syntax tree.
 * 
 * This AST (abstract syntax tree) will be simplified
 * by evaluating constant expressions before executing.
 */

typedef struct condition {
    int operator;
    expr *left;
    expr *right;
} condition;

typedef struct term {
    int operator;
    expr *left;
    expr *right;
} term;

typedef struct cast_term {
    int cast_to;
    expr *value;
} cast_term;

union expression_union {
    char *id_v;
    term *term_v;
    plsm_dtype plsm_v;
    condition *cond_v;
    cast_term *cterm_v;
};

struct expression {
    int used;
    union expression_union data;
};

typedef struct statement statement;

typedef struct code_block {
    map *var_scope;
    statement **stmts;
    unsigned long stmt_size;
} code_block;

typedef struct output_stmt {
    int prod_newline;
    expr *value;
} output_stmt;

typedef struct decl_assign_stmt {
    char *id;
    expr *value;
} decl_assign_stmt;

typedef struct for_stmt {
    expr *def_val;
    expr *max_val;
    char *counter_id;
    statement **stmts;
    unsigned long stmt_size;
} for_stmt;

typedef struct if_stmt {
    expr *condition;
    code_block *if_part;
    code_block *else_part;
} if_stmt;

union statement_union {
    if_stmt *if_stmt;
    for_stmt *for_stmt;
    code_block *code_block;
    output_stmt *output_stmt;
    decl_assign_stmt *decl_assign_stmt;
};

struct statement {
    int used;
    union statement_union data;
};


/*
 * The following header definitions are actually
 * useful.
 */

plsm_dtype get_expr_val(expr*, map*);

expr* simplify_expr(expr*);

expr* create_id_expr(char*);
expr* create_bool_expr(int);
expr* create_char_expr(char);
expr* create_num_expr(double);
expr* create_plsm_expr(plsm_dtype);

expr* create_cast(int, expr*);
expr* create_cond(int, expr*, expr*);
expr* create_term(int, expr*, expr*);

statement* cblock_to_stmt(code_block*);

statement* create_output(expr*, int);
statement* create_decl_assign(expr*, expr*);
statement* create_if_stmt(expr*, code_block*, code_block*);
statement* create_for_stmt(code_block*, expr*, expr*, expr*);

code_block* init_cblock(statement*);
code_block* append_cblock(code_block*, statement*);

plsm_dtype eval_term(term*, map*);
plsm_dtype eval_cterm(cast_term*, map*);
plsm_dtype eval_cond(condition*, map*);

expr* zero_expr();
plsm_dtype null_val();


char* read_file(char*);

void exec_if_stmt(if_stmt*, map*);
void exec_for_stmt(for_stmt*, map*);
void exec_output_stmt(output_stmt*, map*);
void exec_decl_ass_stmt(decl_assign_stmt*, map*);

void exec_stmt(statement*, map*);

/*
 * This function is used for executing code blocks.
 */
void exec_code_block(code_block*, map*);
void exec_program(code_block*);
#endif
