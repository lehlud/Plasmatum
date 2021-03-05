#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define NULL_INDEX  0
#define NUM_INDEX   1
#define CHAR_INDEX  2
#define BOOL_INDEX  3

#define PLSM_INDEX  1
#define TERM_INDEX  2
#define CTERM_INDEX 3
#define ID_INDEX    4

#define ADD_OP 1
#define SUB_OP 2
#define MUL_OP 3
#define DIV_OP 4
#define MOD_OP 5
#define POW_OP 6

#define OUTPUT_INDEX        1
#define DECL_ASS_INDEX      2

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

/*
 * If you want to know why this is bloated, look
 * at the 'function.c' source file.
 */

plsm_dtype add(plsm_dtype, plsm_dtype);
plsm_dtype sub(plsm_dtype, plsm_dtype);
plsm_dtype mul(plsm_dtype, plsm_dtype);
plsm_dtype pdiv(plsm_dtype, plsm_dtype);
plsm_dtype pmod(plsm_dtype, plsm_dtype);
plsm_dtype ppow(plsm_dtype, plsm_dtype);

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
typedef struct map {
    char **keys;
    plsm_dtype *values;
    unsigned long size;
} map;

map map_init();
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
/*
 * This struct is used for simple mathematical
 * arithmetic operations. The operator ('+', '-', ...)
 * is stored as an integer int the 'operator' attribute.
 */
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
    char* id_v;
    term term_v;
    plsm_dtype plsm_v;
    cast_term cterm_v;
};

struct expression {
    int used;
    union expression_union data;
};

typedef struct statement statement;

typedef struct output_stmt {
    int prod_newline;
    expr value;
} output_stmt;

typedef struct decl_assign_stmt {
    char *id;
    expr value;
} decl_assign_stmt;

union statement_union {
    output_stmt output_stmt;
    decl_assign_stmt decl_assign_stmt;
};

struct statement {
    int used;
    union statement_union data;
};

typedef struct code_block {
    map var_scope;
    statement *stmts;
    unsigned long stmt_size;
} code_block;


/*
 * The following header definitions are actually
 * useful.
 */

expr create_cast(int, expr);
expr create_term(int, expr, expr);

statement empty_stmt();
statement create_output(expr*, int);
statement create_decl_assign(expr, expr*);

code_block init_cblock(statement);
code_block append_cblock(code_block, statement);

plsm_dtype eval_term(term, map*);
plsm_dtype eval_cterm(cast_term, map*);

char *readfile(char*);


void exec_stmt(statement, map*);
/*
 * This function is used for executing code blocks.
 */
void exec_code_block(code_block);

#endif
