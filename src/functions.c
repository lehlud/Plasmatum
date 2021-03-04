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
 * Please dont look at the code, just skip to the next
 * comment if you just want to know how Plasmatum works.
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
 * 
 * 'map_set' basically just checks if there already is a
 * entry corresponding to the passed key and if so overwrites
 * the entry, else it would just create a new entry.
 */
void map_set(map *m, char *key, plsm_dtype value) {
    for (int i = 0; i < m->size; i++) {
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
    for (int i = 0; i < m->size; i++) {
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
    for (int i = 0; i < m->size; i++) {
        if (strcmp(key, m->keys[i]) == 0) {
            for(int i1 = i; i1 < m->size - 1; i++)
                m->keys[i] = m->keys[i + 1];
            for(int i1 = i; i1 < m->size - 1; i++)
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


