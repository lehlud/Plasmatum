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
// * ADD
plsm_dtype add(plsm_dtype a, plsm_dtype b) {
    plsm_dtype result;
    switch (a.used) {
        case CHAR_INDEX:
        case NUM_INDEX:
            switch (b.used) {
                case CHAR_INDEX:
                case NUM_INDEX:
                    result.used = NUM_INDEX;
                    result.data.num_v = a.data.num_v + b.data.num_v;
                    break;
                case BOOL_INDEX:
                    result.used = NUM_INDEX;
                    result.data.num_v = a.data.num_v + (b.data.num_v ? 1 : 0);
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
                    result.data.num_v = (b.data.num_v ? 1 : 0) + b.data.num_v;
                    break;
                case BOOL_INDEX:
                    result.used = BOOL_INDEX;
                    result.data.num_v = (a.data.num_v + b.data.num_v) ? 1 : 0;
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

// * SUBTRACT
plsm_dtype sub(plsm_dtype a, plsm_dtype b) {
    plsm_dtype result;
    switch (a.used) {
        case CHAR_INDEX:
        case NUM_INDEX:
            switch (b.used) {
                case CHAR_INDEX:
                case NUM_INDEX:
                    result.used = NUM_INDEX;
                    result.data.num_v = a.data.num_v - b.data.num_v;
                    break;
                case BOOL_INDEX:
                    result.used = NUM_INDEX;
                    result.data.num_v = a.data.num_v - (b.data.num_v ? 1 : 0);
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
                    result.data.num_v = (b.data.num_v ? 1 : 0) - b.data.num_v;
                    break;
                case BOOL_INDEX:
                    result.used = BOOL_INDEX;
                    result.data.num_v = (a.data.num_v - b.data.num_v) ? 1 : 0;
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

// * MULTIPLY
plsm_dtype mul(plsm_dtype a, plsm_dtype b) {
    plsm_dtype result;
    switch (a.used) {
        case CHAR_INDEX:
        case NUM_INDEX:
            switch (b.used) {
                case CHAR_INDEX:
                case NUM_INDEX:
                    result.used = NUM_INDEX;
                    result.data.num_v = a.data.num_v * b.data.num_v;
                    break;
                case BOOL_INDEX:
                    result.used = NUM_INDEX;
                    result.data.num_v = a.data.num_v * (b.data.num_v ? 1 : 0);
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
                    result.data.num_v = (b.data.num_v ? 1 : 0) * b.data.num_v;
                    break;
                case BOOL_INDEX:
                    result.used = BOOL_INDEX;
                    result.data.num_v = a.data.num_v && b.data.num_v;
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
 * This function's name is 'pdiv' because there
 * already is a function called 'div' in the
 * 'math.h' library.
 */
plsm_dtype pdiv(plsm_dtype a, plsm_dtype b) {
    plsm_dtype result;
    switch (a.used) {
        case CHAR_INDEX:
        case NUM_INDEX:
            switch (b.used) {
                case CHAR_INDEX:
                case NUM_INDEX:
                    result.used = NUM_INDEX;
                    result.data.num_v = a.data.num_v / b.data.num_v;
                    break;
                case BOOL_INDEX:
                    result.used = NUM_INDEX;
                    result.data.num_v = a.data.num_v / (b.data.num_v ? 1 : 0);
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
                    result.data.num_v = (b.data.num_v ? 1 : 0) / b.data.num_v;
                    break;
                case BOOL_INDEX:
                    result.used = BOOL_INDEX;
                    result.data.num_v = (a.data.num_v / b.data.num_v) ? 1 : 0;
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
 * This function's name is 'pmod' because there
 * already is a function called 'mod' in the
 * 'math.h' library.
 */
plsm_dtype pmod(plsm_dtype a, plsm_dtype b) {
    plsm_dtype result;
    switch (a.used) {
        case CHAR_INDEX:
        case NUM_INDEX:
            switch (b.used) {
                case CHAR_INDEX:
                case NUM_INDEX:
                    result.used = NUM_INDEX;
                    result.data.num_v = fmod(a.data.num_v, b.data.num_v);
                    break;
                case BOOL_INDEX:
                    result.used = NUM_INDEX;
                    result.data.num_v = fmod(a.data.num_v, (b.data.num_v ? 1 : 0));
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
                    result.data.num_v = fmod((b.data.num_v ? 1 : 0), b.data.num_v);
                    break;
                case BOOL_INDEX:
                    result.used = BOOL_INDEX;
                    result.data.num_v = fmod(a.data.num_v, b.data.num_v) ? 1 : 0;
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
 * This function's name is 'ppow' because there
 * already is a function called 'pow' in the
 * 'math.h' library.
 */
plsm_dtype ppow(plsm_dtype a, plsm_dtype b) {
    plsm_dtype result;
    switch (a.used) {
        case CHAR_INDEX:
        case NUM_INDEX:
            switch (b.used) {
                case CHAR_INDEX:
                case NUM_INDEX:
                    result.used = NUM_INDEX;
                    result.data.num_v = pow(a.data.num_v, b.data.num_v);
                    break;
                case BOOL_INDEX:
                    result.used = NUM_INDEX;
                    result.data.num_v = pow(a.data.num_v, b.data.num_v ? 1 : 0);
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
                    result.data.num_v = pow(b.data.num_v ? 1 : 0, b.data.num_v);
                    break;
                case BOOL_INDEX:
                    result.used = BOOL_INDEX;
                    result.data.num_v = pow(a.data.num_v, b.data.num_v) ? 1 : 0;
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
            printf("%c", (int) val.data.num_v);
            break;
        case BOOL_INDEX:
            printf("%s", val.data.num_v ? "true" : "false");
            break;
        default:
            printf("null");
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

map map_init() {
    map m;
    m.keys = malloc(0);
    m.size = 0;
    m.values = malloc(0);
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


expr create_cast(int cast_to, expr value) {
    cast_term cast_term;
    cast_term.cast_to = cast_to;
    cast_term.value = &value;
    expr result;
    result.used = CTERM_INDEX;
    result.data.cterm_v = cast_term;
    return result;
}

expr create_term(int op, expr left, expr right) {
    term term;
    term.left = &left;
    term.right = &right;
    term.operator = op;
    expr result;
    result.used = TERM_INDEX;
    result.data.term_v = term;
    return result;
}

statement empty_stmt() {
    statement result;
    result.used = -1;
    return result;
}

statement create_output(expr *value, int prod_nl){
    statement result;
    output_stmt output_stmt;
    output_stmt.prod_newline = prod_nl;
    if (value) output_stmt.value = *value;
    else output_stmt.value.used = -1;
    result.used = OUTPUT_INDEX;
    result.data.output_stmt = output_stmt;
    return result;
}

statement create_decl_assign(expr id, expr *value) {
    decl_assign_stmt decl_assign;
    decl_assign.id = id.data.id_v;
    if (value) decl_assign.value = *value;
    else decl_assign.value.used = -1;
    statement result;
    result.used = DECL_ASS_INDEX;
    result.data.decl_assign_stmt = decl_assign;
    return result;
}


code_block init_cblock(statement stmt) {
    code_block cblock;
    cblock.var_scope = map_init();
    cblock.stmts = malloc(sizeof(statement));
    cblock.stmts[0] = stmt;
    cblock.stmt_size = 1;
    return cblock;
}

code_block append_cblock(code_block cblock, statement stmt) {
    if(stmt.used >= 0) {
        cblock.stmts = realloc(
            cblock.stmts,
            (cblock.stmt_size + 1) * sizeof(statement)
        );
        cblock.stmts[cblock.stmt_size] = stmt;
        cblock.stmt_size = cblock.stmt_size + 1;
    }
    return cblock;
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



void exec_stmt(statement stmt, map *m) {
    switch (stmt.used) {
        case OUTPUT_INDEX:
            printf("output statement! (%d)\n", stmt.data.output_stmt.value.used);
            if (stmt.data.output_stmt.value.used >= 0) {
                plsm_dtype tmp;
                term t1;
                switch (stmt.data.output_stmt.value.used) {
                    case PLSM_INDEX:
                        printval(stmt.data.output_stmt.value.data.plsm_v);
                        break;
                    case TERM_INDEX:
                        t1 = stmt.data.output_stmt.value.data.term_v;
                        printf("a\n");
                        eval_term(t1, m);
                        printf("b\n");
                        tmp = eval_term(t1, m);
                        printf("b\n");
                        printval(tmp);
                        break;
                    case CTERM_INDEX:
                        printval(eval_cterm(stmt.data.output_stmt.value.data.cterm_v, m));
                        break;
                    case ID_INDEX:
                        printval(map_get(m, stmt.data.output_stmt.value.data.id_v));
                        break;
                }
            }
            if (stmt.data.output_stmt.prod_newline)
                printf("\n");
            break;
        case DECL_ASS_INDEX:
            if(stmt.data.decl_assign_stmt.value.used < 0)
                map_remove(m, stmt.data.decl_assign_stmt.id);
            else {
                plsm_dtype val;
                switch (stmt.data.decl_assign_stmt.value.used) {
                    case PLSM_INDEX:
                        val = stmt.data.decl_assign_stmt.value.data.plsm_v;
                        break;
                    case TERM_INDEX:
                        val = eval_term(stmt.data.decl_assign_stmt.value.data.term_v, m);
                        break;
                    case CTERM_INDEX:
                        val = eval_cterm(stmt.data.decl_assign_stmt.value.data.cterm_v, m);
                        break;
                    case ID_INDEX:
                        val = map_get(m, stmt.data.decl_assign_stmt.value.data.id_v);
                        break;
                }
                map_set(m, stmt.data.decl_assign_stmt.id, val);
            }
            break;
        default:
            break;
    }
}

void exec_code_block(code_block code) {
    for (unsigned long i = 0; i < code.stmt_size; i++)
        exec_stmt(code.stmts[i], &(code.var_scope));
}



plsm_dtype eval_term(term t, map *m) {
    printf("evaluating term...");
    plsm_dtype left;
    plsm_dtype right;
    switch (t.left->used) {
        case ID_INDEX:
            left = map_get(m, t.left->data.id_v);
            break;
        case TERM_INDEX:
            left = eval_term(t.left->data.term_v, m);
            break;
        case PLSM_INDEX:
            left = t.left->data.plsm_v;
            break;
        case CTERM_INDEX:
            left = eval_cterm(t.left->data.cterm_v, m);
            break;
    }
    switch (t.right->used) {
        case ID_INDEX:
            right = map_get(m, t.right->data.id_v);
            break;
        case TERM_INDEX:
            right = eval_term(t.right->data.term_v, m);
            break;
        case PLSM_INDEX:
            right = t.right->data.plsm_v;
            break;
        case CTERM_INDEX:
            right = eval_cterm(t.right->data.cterm_v, m);
            break;
    }
    switch (t.operator) {
        case ADD_OP: return add(left, right);
        case SUB_OP: return sub(left, right);
        case MUL_OP: return mul(left, right);
        case DIV_OP: return pdiv(left, right);
        case MOD_OP: return pmod(left, right);
        case POW_OP: return ppow(left, right);
    }
    plsm_dtype result;
    result.used = NULL_INDEX;
    return result;
}

plsm_dtype eval_cterm(cast_term ct, map *m) {
    plsm_dtype value;
    switch (ct.value->used) {
        case ID_INDEX:
            value = map_get(m, ct.value->data.id_v);
            break;
        case TERM_INDEX:
            value = eval_term(ct.value->data.term_v, m);
            break;
        case PLSM_INDEX:
            value = ct.value->data.plsm_v;
            break;
        case CTERM_INDEX:
            value = eval_cterm(ct.value->data.cterm_v, m);
            break;
    }
    return cast(value, ct.cast_to);
}
