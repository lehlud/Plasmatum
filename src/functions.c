#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"


// * TERM STUFF
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
                    result.data.num_v = (a.data.num_v * b.data.num_v) ? 1 : 0;
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

plsm_dtype cast(plsm_dtype val, int type) {
    plsm_dtype result;
    result.used = type;
    result.data = val.data;
    return result;
}


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
            printf("%d", val.data.num_v ? 1 : 0);
            break;
        default:
            printf("null");
            break;
    }
}

map map_init() {
    map m;
    m.keys = malloc(0);
    m.values = malloc(0);
    return m;
}

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

plsm_dtype map_get(map *m, char *key) {
    for (int i = 0; i < m->size; i++) {
        if (strcmp(key, m->keys[i]) == 0) {
            return m->values[i]; 
        }
    }
    plsm_dtype result;
    return result;
}

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
