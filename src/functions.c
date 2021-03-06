#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"


/*
 * This bloaty mess underneath this comment is basically
 * just used for implementing adding and subtracting
 * and multiplying and so on for dynamic typing usage.
 * 
 * Please don't look at the code, just skip to the next
 * comment if you just want to know how Plasmatum works.
 * 
 * Maybe I'll simplify this in a future commit.
 */
plsm_dtype calc(int operator, plsm_dtype a, plsm_dtype b) {
    plsm_dtype result;
    switch (a.used) {
    case CHAR_INDEX:
    case NUM_INDEX:
        switch (b.used) {
        case CHAR_INDEX:
        case NUM_INDEX:
            result.used = NUM_INDEX;
            switch (operator) {
            case ADD_OP:
                result.data.num_v = a.data.num_v + b.data.num_v;
                break;
            case SUB_OP:
                result.data.num_v = a.data.num_v - b.data.num_v;
                break;
            case MUL_OP:
                result.data.num_v = a.data.num_v * b.data.num_v;
                break;
            case DIV_OP:
                result.data.num_v = a.data.num_v / b.data.num_v;
                break;
            case MOD_OP:
                result.data.num_v = fmod(a.data.num_v, b.data.num_v);
                break;
            case POW_OP:
                result.data.num_v = pow(a.data.num_v, b.data.num_v);
                break;  
            default:
                result.used = NULL_INDEX;
                break;
            }
            break;
        case BOOL_INDEX:
            result.used = NUM_INDEX;
            switch (operator) {
            case ADD_OP:
                result.data.num_v = (((int) a.data.num_v) + ((int) b.data.num_v)) ? 1 : 0;
                break;
            case SUB_OP:
                result.data.num_v = (((int) a.data.num_v) - ((int) b.data.num_v)) ? 1 : 0;
                break;
            case MUL_OP:
                result.data.num_v = ((int) a.data.num_v) && ((int) b.data.num_v);
                break;
            case DIV_OP:
                result.data.num_v = (((int) a.data.num_v) / ((int) b.data.num_v)) ? 1 : 0;
                break;
            case MOD_OP:
                result.data.num_v = (((int) a.data.num_v) % ((int) b.data.num_v)) ? 1 : 0;
                break;
            case POW_OP:
                result.data.num_v = a.data.num_v || b.data.num_v;
                break;  
            default:
                result = null_val();
                break;
            }
            break;
        default:
            result.used = NULL_INDEX;
            break;
        }
        break;

    case BOOL_INDEX:
        switch (b.used) {
        case CHAR_INDEX:
        case NUM_INDEX:
            result.used = NUM_INDEX;
            switch (operator) {
            case ADD_OP:
                result.data.num_v = ((int) a.data.num_v) + b.data.num_v;
                break;
            case SUB_OP:
                result.data.num_v = ((int) a.data.num_v) - b.data.num_v;
                break;
            case MUL_OP:
                result.data.num_v = ((int) a.data.num_v) * b.data.num_v;
                break;
            case DIV_OP:
                result.data.num_v = ((int) a.data.num_v) / b.data.num_v;
                break;
            case MOD_OP:
                result.data.num_v = fmod((int) a.data.num_v, b.data.num_v);
                break;
            case POW_OP:
                result.data.num_v = pow((int) a.data.num_v, b.data.num_v);
                break;  
            default:
                result = null_val();
                break;
            }
            break;
        case BOOL_INDEX:
            result.used = BOOL_INDEX;
            switch (operator) {
            case ADD_OP:
                result.data.num_v = (((int) a.data.num_v) + ((int) b.data.num_v)) ? 1 : 0;
                break;
            case SUB_OP:
                result.data.num_v = (((int) a.data.num_v) - ((int) b.data.num_v)) ? 1 : 0;
                break;
            case MUL_OP:
                result.data.num_v = ((int) a.data.num_v) && ((int) b.data.num_v);
                break;
            case DIV_OP:
                result.data.num_v = (((int) a.data.num_v) / ((int) b.data.num_v)) ? 1 : 0;
                break;
            case MOD_OP:
                result.data.num_v = (((int) a.data.num_v) % ((int) b.data.num_v)) ? 1 : 0;
                break;
            case POW_OP:
                result.data.num_v = a.data.num_v || b.data.num_v;
                break;  
            default:
                result = null_val();
                break;
            }
            break;
        default:
            result.used = NULL_INDEX;
            break;
        }
        break;

    default:
        result.used = NULL_INDEX;
        break;
    }
    return result;
}


/*
 * This simple function is used for casting values to
 * a specific data type by just changing the 'used'
 * attribute of the passed value.
 * 
 * This will be made a tiny bit faster by using pointers
 * instead soon.
 */
plsm_dtype cast(plsm_dtype val, int type) {
    plsm_dtype result;
    result.used = type;
    result.data = val.data;
    return result;
}


/*
 * This function prints a plsm_dtype value corresponding to its type.
 * The printf function needs to know what it is printing, right?
 */
void printval(plsm_dtype val) {
    switch (val.used) {
        case NUM_INDEX:
            if (fmod(val.data.num_v, 1) == 0)
                printf("%ld", (long) val.data.num_v);
            else printf("%lf", val.data.num_v);
            break;
        case CHAR_INDEX:
            putchar((int) val.data.num_v);
            break;
        case BOOL_INDEX:
            printf("%s", val.data.num_v ? "true" : "false");
            break;
        default:
            printf("%s", "null");
            break;
    }
}



/*
 * These are functions corresponding to a map implementation.
 * A hash map would probably be better, maybe I'll cover this
 * in a future commit.
 * 
 * This would be the corresponding hash function:
 * 
 * unsigned long hash(const char *s) {
 *     int c;
 *     unsigned long result = 5381;
 *     while ((c = *s++))
 *         result = c + result + (result << 5);
 *     return result;
 * }
 */

map* map_init() {
    map *m = malloc(sizeof(map));
    m->keys = malloc(0);
    m->size = 0;
    m->values = malloc(0);
    return m;
}

/* 
 * 'map_set' basically just checks if there already is a
 * entry corresponding to the passed key and if so overwrites
 * the entry, else it would just create a new entry.
 */
void map_set(map *m, char *key, plsm_dtype value) {
    for (unsigned long i = 0; i < m->size; i++) {
        if (strcmp(key, m->keys[i]) == 0) {
            m->values[i] = value;
            return;
        }
    }
    m->keys = realloc(m->keys, (m->size+1) * sizeof(char*));
    m->values = realloc(m->values, (m->size+1) * sizeof(plsm_dtype));
    
    m->keys[m->size] = key;
    m->values[m->size] = value;
    m->size = m->size + 1;
}

/*
 * 'map_get' iterates over the keys of a map and returns
 * the corresponding value if a key matched the passed key.
 * It just returns a null value if no key matched.
 */
plsm_dtype map_get(map *m, char *key) {
    for (unsigned long i = 0; i < m->size; i++) {
        if (strcmp(key, m->keys[i]) == 0) {
            return m->values[i]; 
        }
    }
    plsm_dtype result;
    result.used = NULL_INDEX;
    return result;
}

/*
 * 'map_remove' iterates over the keys of a map and removes
 * the corresponding entry if a key matched the passed key.
 * It does so by moving every entry on the right of the 
 * one to be removed one index to the left.
 * If no key matches it basically does nothing else than wasting
 * computing power.
 */
void map_remove(map *m, char *key) {
    for (unsigned long i = 0; i < m->size; i++) {
        if (strcmp(key, m->keys[i]) == 0) {
            for(unsigned long i1 = i; i1 < m->size - 1; i++)
                m->keys[i] = m->keys[i + 1];
            for(unsigned long i1 = i; i1 < m->size - 1; i++)
                m->values[i] = m->values[i + 1];
            m->size = m->size - 1;
            m->keys = realloc(m->keys, (m->size) * sizeof(char*));
            m->values = realloc(m->values, (m->size) * sizeof(plsm_dtype));
            return;
        }
    }
}

/*
 * This function reads the contents of a file and returns
 * it as a char array.
 */
char* readfile(char* path) {
    char *tmp = 0;
    FILE * f = fopen(path, "r");
    if(f) {
        fseek (f, 0, SEEK_END);
        unsigned long size = ftell(f);
        fseek (f, 0, SEEK_SET);
        tmp = malloc(size);
        if(tmp) fread(tmp, 1, size, f);
        fclose(f);
    }
    return tmp;
}

/*
 * This now all has to do with creating the AST
 */

expr* create_id_expr(char *s) {
    expr *result = malloc(sizeof(expr));
    result->used = ID_INDEX;
    result->data.id_v = s;
    return result;
}

expr* create_bool_expr(int value) {
    plsm_dtype tmp;
    tmp.used = BOOL_INDEX;
    tmp.data.num_v = (double) value;
    return create_plsm_expr(tmp);
}

expr* create_char_expr(char value) {
    plsm_dtype tmp;
    tmp.used = CHAR_INDEX;
    tmp.data.num_v = (double) value;
    return create_plsm_expr(tmp);
}

expr* create_num_expr(double value) {
    plsm_dtype tmp;
    tmp.used = NUM_INDEX;
    tmp.data.num_v = value;
    return create_plsm_expr(tmp);
}

expr* create_plsm_expr(plsm_dtype value) {
    expr *result = malloc(sizeof(expr));
    result->used = PLSM_INDEX;
    result->data.plsm_v = value;
    return result;
}

expr* create_cast(int cast_to, expr *value) {
    cast_term cast_term;
    cast_term.cast_to = cast_to;
    cast_term.value = malloc(sizeof(cast_term));
    *(cast_term.value) = *value;

    expr *result = malloc(sizeof(expr));
    result->used = CTERM_INDEX;
    result->data.cterm_v = malloc(sizeof(cast_term));
    *(result->data.cterm_v) = cast_term;
    return result;
}

expr* create_term(int op, expr *left, expr *right) {
    term term;
    term.left = malloc(sizeof(expr));
    term.right = malloc(sizeof(expr));
    *(term.left) = *left;
    *(term.right) = *right;
    term.operator = op;

    expr *result = malloc(sizeof(expr));
    result->used = TERM_INDEX;
    result->data.term_v = malloc(sizeof(term));
    *(result->data.term_v) = term;
    return result;
}

statement* empty_stmt() {
    statement *result = malloc(sizeof(statement));
    result->used = -1;
    return result;
}

statement* create_output(expr *value, int prod_nl){
    output_stmt output;
    output.prod_newline = prod_nl;
    output.value = malloc(sizeof(expr));
    if (value) *(output.value) = *value;
    else output.value->used = -1;

    statement *result = malloc(sizeof(statement));
    result->used = OUTPUT_INDEX;
    result->data.output_stmt = malloc(sizeof(output_stmt));
    *(result->data.output_stmt) = output;
    return result;
}

statement* create_decl_assign(expr *id, expr *value) {
    decl_assign_stmt decl_assign;
    decl_assign.id = id->data.id_v;
    decl_assign.value = malloc(sizeof(expr));
    if (value) *(decl_assign.value) = *value;
    else decl_assign.value->used = -1;

    statement *result = malloc(sizeof(statement));
    result->used = DECL_ASS_INDEX;
    result->data.decl_assign_stmt = malloc(sizeof(decl_assign_stmt));
    *(result->data.decl_assign_stmt) = decl_assign;
    return result;
}


code_block* init_cblock(statement *stmt) {
    code_block *cblock = malloc(sizeof(cblock));
    cblock->var_scope = malloc(sizeof(map));
    *(cblock->var_scope) = *(map_init());
    cblock->stmts = malloc(0);
    cblock->stmt_size = 0;
    return append_cblock(cblock, stmt);
}

code_block* append_cblock(code_block *cblock, statement *stmt) {
    if(stmt->used >= 0) {
        cblock->stmts = realloc(
            cblock->stmts,
            (cblock->stmt_size + 1) * sizeof(statement*)
        );
        cblock->stmts[cblock->stmt_size] = malloc(sizeof(statement));
        *(cblock->stmts[cblock->stmt_size]) = *stmt;
        cblock->stmt_size = cblock->stmt_size + 1;
    }
    return cblock;
}

/*
 * The following functions are used for
 * interpreting/executing the AST.
 */

plsm_dtype null_val() {
    plsm_dtype result;
    result.used = NULL_INDEX;
    return result;
}

plsm_dtype get_expr_val(expr *expr, map *m) {
    switch (expr->used) {
        case PLSM_INDEX:
            return expr->data.plsm_v;
        case TERM_INDEX:
            return eval_term(expr->data.term_v, m);
        case CTERM_INDEX:
            return eval_cterm(expr->data.cterm_v, m);
        case ID_INDEX:
            return map_get(m, expr->data.id_v);
        default:
            return null_val();
    }
}

plsm_dtype eval_term(term *t, map *m) {
    return calc(
        t->operator,
        get_expr_val(t->left, m),
        get_expr_val(t->right, m)
    );
}

plsm_dtype eval_cterm(cast_term *ct, map *m) {
    return cast(get_expr_val(ct->value, m), ct->cast_to);
}


void exec_output_stmt(output_stmt *stmt, map *m) {
    if (stmt->value->used >= 0)
        printval(get_expr_val(stmt->value, m));
    if (stmt->prod_newline) printf("%s", "\n");
}

void exec_decl_ass_stmt(decl_assign_stmt *stmt, map *m) {
    if (stmt->value->used < 0) {
        map_remove(m, stmt->id);
        return;
    }
    map_set(m, stmt->id, get_expr_val(stmt->value, m));
}

void exec_stmt(statement *stmt, map *m) {
    switch (stmt->used) {
        case OUTPUT_INDEX:
            exec_output_stmt(stmt->data.output_stmt, m);
            break;
        case DECL_ASS_INDEX:
            exec_decl_ass_stmt(stmt->data.decl_assign_stmt, m);
            break;
    }
}


void exec_code_block(code_block *code) {
    for (unsigned long i = 0; i < code->stmt_size; i++)
        exec_stmt(code->stmts[i], code->var_scope);
}

