#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

plsm_dtype incr(plsm_dtype d) {
    d.data.num_v = d.data.num_v + 1;
    return d;
}

plsm_dtype decr(plsm_dtype d) {
    d.data.num_v = d.data.num_v - 1;
    return d;
}

/*
 * This bloaty function underneath this comment is basically
 * just used for implementing adding and subtracting
 * and multiplying and so on for dynamic typing usage.
 * 
 * Please don't look at the code, just skip to the next
 * function if you just want to know how Plasmatum works.
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
                if(b.data.num_v == 0) return null_val();
                result.data.num_v = a.data.num_v / b.data.num_v;
                break;
            case MOD_OP:
                if(b.data.num_v == 0) return null_val();
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
                result.data.num_v = a.data.num_v + ((int) b.data.num_v);
                break;
            case SUB_OP:
                result.data.num_v = a.data.num_v - ((int) b.data.num_v);
                break;
            case MUL_OP:
                result.data.num_v = a.data.num_v * ((int) b.data.num_v);
                break;
            case DIV_OP:
                if(b.data.num_v == 0) return null_val();
                result.data.num_v = a.data.num_v / ((int) b.data.num_v);
                break;
            case MOD_OP:
                if(b.data.num_v == 0) return null_val();
                result.data.num_v = fmod(a.data.num_v, (int) b.data.num_v);
                break;
            case POW_OP:
                if(a.data.num_v == 0 && b.data.num_v == 0) return null_val();
                result.data.num_v = pow(a.data.num_v, (int) b.data.num_v);
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
                if(b.data.num_v == 0) return null_val();
                result.data.num_v = ((int) a.data.num_v) / b.data.num_v;
                break;
            case MOD_OP:
                if(b.data.num_v == 0) return null_val();
                result.data.num_v = fmod((int) a.data.num_v, b.data.num_v);
                break;
            case POW_OP:
                if(a.data.num_v == 0 && b.data.num_v == 0) return null_val();
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
                if(b.data.num_v == 0) return null_val();
                result.data.num_v = ((int) a.data.num_v) / ((int) b.data.num_v) ? 1 : 0;
                break;
            case MOD_OP:
                if(b.data.num_v == 0) return null_val();
                result.data.num_v = (((int) a.data.num_v) % ((int) b.data.num_v)) ? 1 : 0;
                break;
            case POW_OP:
                if(a.data.num_v == 0 && b.data.num_v == 0) return null_val();
                result.data.num_v = ((int) a.data.num_v) ? 1 : 0;
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

plsm_dtype calc_cond(int op, plsm_dtype a, plsm_dtype b) {
    plsm_dtype result;
    result.used = BOOL_INDEX;
    switch (op) {
    case GR_OP:
        result.data.num_v = a.data.num_v > b.data.num_v;
        break;
    case LO_OP:
        result.data.num_v = a.data.num_v < b.data.num_v;
        break;
    case EQ_OP:
        result.data.num_v = a.data.num_v == b.data.num_v;
        break;
    case GREQ_OP:
        result.data.num_v = a.data.num_v >= b.data.num_v;
        break;
    case LOEQ_OP:
        result.data.num_v = a.data.num_v >= b.data.num_v;
        break;
    case OR_OP:
        result.data.num_v = a.data.num_v || b.data.num_v;
        break;
    case XOR_OP:
        result.data.num_v = a.data.num_v != b.data.num_v;
        break;
    case NOT_OP:
        result.data.num_v = !a.data.num_v;
        break;
    case AND_OP:
        result.data.num_v = a.data.num_v && b.data.num_v;
        break;
    default: return null_val();
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
    m->size = 0;
    m->pairs = malloc(0);
    return m;
}

/* 
 * 'map_set' basically just checks if there already is a
 * entry corresponding to the passed key and if so overwrites
 * the entry, else it would just create a new entry.
 */
void map_set(map *m, char *key, plsm_dtype value) {
    for (unsigned long i = 0; i < m->size; i++) {
        if (strcmp(key, m->pairs[i]->key) == 0) {
            m->pairs[i]->value = value;
            return;
        }
    }
    m->pairs = realloc(m->pairs, (m->size+1) * sizeof(map_pair*));
    m->pairs[m->size] = malloc(sizeof(map_pair));

    map_pair *pair = malloc(sizeof(map_pair));
    pair->key = key;
    pair->value = value;

    m->pairs[m->size] = pair;
    m->size = m->size + 1;
}

/*
 * 'map_get' iterates over the keys of a map and returns
 * the corresponding value if a key matched the passed key.
 * It just returns a null value if no key matched.
 */
plsm_dtype map_get(map *m, char *key) {
    for (unsigned long i = 0; i < m->size; i++) {
        if (strcmp(key, m->pairs[i]->key) == 0) {
            return m->pairs[i]->value; 
        }
    }
    return null_val();
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
        if (strcmp(key, m->pairs[i]->key) == 0) {
            free(m->pairs[i]);
            for(unsigned long i1 = i; i1 < m->size - 1; i++)
                m->pairs[i1] = m->pairs[i1 + 1];
            m->size = m->size - 1;
            m->pairs = realloc(m->pairs, (m->size) * sizeof(map_pair*));
            return;
        }
    }
}

/*
 * This function reads the contents of a file and returns
 * it as a char array.
 */
char* read_file(char* path) {
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
 * Everything until the the next comment has to do
 * with creating the AST.
 */

expr* simplify_expr(expr* expr) {
    switch (expr->used) {
    case CTERM_INDEX:
        if (expr->data.cterm_v->value->used == PLSM_INDEX) {
            expr->used = PLSM_INDEX;
            expr->data.plsm_v = cast(
                expr->data.cterm_v->value->data.plsm_v,
                expr->data.cterm_v->cast_to
            );
        } 
        break;
    case TERM_INDEX:
        if(expr->data.term_v->left->used == PLSM_INDEX && expr->data.term_v->right->used == PLSM_INDEX) {
            expr->used = PLSM_INDEX;
            expr->data.plsm_v = calc(
                expr->data.term_v->operator,
                expr->data.term_v->left->data.plsm_v,
                expr->data.term_v->right->data.plsm_v
            );
        }
        break;
    case COND_INDEX:
        if(expr->data.cond_v->left->used == PLSM_INDEX && expr->data.cond_v->right->used == PLSM_INDEX) {
            expr->used = PLSM_INDEX;
            expr->data.plsm_v = calc_cond(
                expr->data.cond_v->operator,
                expr->data.cond_v->left->data.plsm_v,
                expr->data.cond_v->right->data.plsm_v
            );
        }
        break;
    }
    return expr;
}

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
    return simplify_expr(result);
}

expr* create_cond(int op, expr *left, expr *right) {
    condition cond;
    cond.left = malloc(sizeof(expr));
    cond.right = malloc(sizeof(expr));
    *(cond.left) = *left;
    *(cond.right) = *right;
    cond.operator = op;

    expr *result = malloc(sizeof(expr));
    result->used = COND_INDEX;
    result->data.cond_v = malloc(sizeof(condition));
    *(result->data.cond_v) = cond;
    return simplify_expr(result);
}

expr* create_term(int op, expr *left, expr *right) {
    term t;
    t.left = malloc(sizeof(expr));
    t.right = malloc(sizeof(expr));
    *(t.left) = *left;
    *(t.right) = *right;
    t.operator = op;

    expr *result = malloc(sizeof(expr));
    result->used = TERM_INDEX;
    result->data.term_v = malloc(sizeof(term));
    *(result->data.term_v) = t;
    return simplify_expr(result);
}

statement* cblock_to_stmt(code_block *cblock) {
    statement *result = malloc(sizeof(statement));
    result->used = CBLOCK_INDEX;
    result->data.code_block = malloc(sizeof(code_block));
    *(result->data.code_block) = *cblock;
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

statement* create_if_stmt(expr *cond, code_block *if_part, code_block *else_part) {
    if_stmt if_s;
    if_s.condition = malloc(sizeof(expr));
    *(if_s.condition) = *cond;
    if_s.if_part = malloc(sizeof(statement));;
    *(if_s.if_part) = *if_part;
    if (else_part) {
        if_s.else_part = malloc(sizeof(statement));;
        *(if_s.else_part) = *else_part;
    } else if_s.else_part = 0;

    statement *result = malloc(sizeof(result));
    result->used = IF_INDEX;
    result->data.if_stmt = malloc(sizeof(if_stmt));
    *(result->data.if_stmt) = if_s;
    return result;
}

statement* create_for_stmt(code_block* cblock, expr *counter_id, expr *def_val, expr *max_val) {
    for_stmt tmp;
    tmp.stmt_size = cblock->stmt_size;
    tmp.stmts = cblock->stmts;
    tmp.counter_id = counter_id->data.id_v;
    tmp.def_val = malloc(sizeof(expr));
    tmp.max_val = malloc(sizeof(expr));
    *(tmp.def_val) = def_val ? *def_val : *(zero_expr());
    *(tmp.max_val) = *max_val;

    statement *result = malloc(sizeof(result));
    result->used = FOR_INDEX;
    result->data.for_stmt = malloc(sizeof(for_stmt));
    *(result->data.for_stmt) = tmp;
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
    if(stmt && cblock) {
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

expr* zero_expr() {
    plsm_dtype tmp;
    tmp.used = NUM_INDEX;
    tmp.data.num_v = 0;

    expr *result = malloc(sizeof(expr));
    result->used = PLSM_INDEX;
    result->data.plsm_v = tmp;
    return result;
}

plsm_dtype null_val() {
    plsm_dtype result;
    result.used = NULL_INDEX;
    return result;
}

plsm_dtype get_expr_val(expr *expr, map *m) {
    switch (expr->used) {
    case PLSM_INDEX: return expr->data.plsm_v;
    case TERM_INDEX: return eval_term(expr->data.term_v, m);
    case CTERM_INDEX: return eval_cterm(expr->data.cterm_v, m);
    case COND_INDEX: return eval_cond(expr->data.cond_v, m);
    case ID_INDEX: return map_get(m, expr->data.id_v);
    default: return null_val();
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

plsm_dtype eval_cond(condition *cond, map *m) {
    plsm_dtype left;
    plsm_dtype right;
    if (cond->left->used == PLSM_INDEX)
        left = cond->left->data.plsm_v;
    else left = get_expr_val(cond->left, m);

    if (cond->right) {
        if (cond->right->used == PLSM_INDEX)
            right = cond->right->data.plsm_v;
        else right = get_expr_val(cond->right, m);
    } else right = null_val();

    return calc_cond(cond->operator, left, right);
}

statement* create_while_stmt(expr *cond, code_block *body) {
    while_stmt while_s;
    while_s.condition = malloc(sizeof(expr));
    while_s.body = malloc(sizeof(code_block));
    *(while_s.condition) = *cond;
    *(while_s.body) = *body;

    statement *result = malloc(sizeof(statement));
    result->used = WHILE_INDEX;
    result->data.while_stmt = malloc(sizeof(while_stmt));
    *(result->data.while_stmt) = while_s;
    return result;
}

void exec_if_stmt(if_stmt *if_s, map *m) {
    if (get_expr_val(if_s->condition, m).data.num_v)
        exec_code_block(if_s->if_part, m);
    else if (if_s->else_part) 
        exec_code_block(if_s->else_part, m);
}

void exec_for_stmt(for_stmt *for_s, map *m) {
    int var_defined = 0;
    var_defined = map_get(m, for_s->counter_id).used > 0;
    map_set(m, for_s->counter_id, get_expr_val(for_s->def_val, m));
    for (double d = get_expr_val(for_s->def_val, m).data.num_v;
        d < get_expr_val(for_s->max_val, m).data.num_v;
        d = map_get(m, for_s->counter_id).data.num_v) {
        for(unsigned long i = 0; i < for_s->stmt_size; i++) {
            exec_stmt(for_s->stmts[i], m);
        }
        map_set(m, for_s->counter_id, incr(map_get(m, for_s->counter_id)));
    }
    if(!var_defined) map_remove(m, for_s->counter_id);
}

void exec_while_stmt(while_stmt *while_s, map *m) {
    while (get_expr_val(while_s->condition, m).data.num_v)
        exec_code_block(while_s->body, m);
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
    case IF_INDEX:
        exec_if_stmt(stmt->data.if_stmt, m);
        break;
    case FOR_INDEX:
        exec_for_stmt(stmt->data.for_stmt, m);
        break;
    case CBLOCK_INDEX:
        exec_code_block(stmt->data.code_block, m);
        break;
    case WHILE_INDEX:
        exec_while_stmt(stmt->data.while_stmt, m);
        break;
    }
}


void exec_program(code_block *code) {
    for (unsigned long i = 0; i < code->stmt_size; i++)
        exec_stmt(code->stmts[i], code->var_scope);
}

void exec_code_block(code_block *code, map *m) {
    code->var_scope = m;
    for (unsigned long i = 0; i < code->stmt_size; i++)
        exec_stmt(code->stmts[i], code->var_scope);
}

