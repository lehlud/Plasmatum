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
                    exit(1);    // TODO: IMPLEMENT ERROR MESSAGE
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
                    exit(1);    // TODO: IMPLEMENT ERROR MESSAGE
                    break;
            }
            break;

        default:
            exit(1);            // TODO: IMPLEMENT ERROR MESSAGE
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
                    exit(1);    // TODO: IMPLEMENT ERROR MESSAGE
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
                    exit(1);    // TODO: IMPLEMENT ERROR MESSAGE
                    break;
            }
            break;

        default:
            exit(1);            // TODO: IMPLEMENT ERROR MESSAGE
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
                    exit(1);    // TODO: IMPLEMENT ERROR MESSAGE
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
                    exit(1);    // TODO: IMPLEMENT ERROR MESSAGE
                    break;
            }
            break;

        default:
            exit(1);            // TODO: IMPLEMENT ERROR MESSAGE
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
                    exit(1);    // TODO: IMPLEMENT ERROR MESSAGE
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
                    exit(1);    // TODO: IMPLEMENT ERROR MESSAGE
                    break;
            }
            break;

        default:
            exit(1);            // TODO: IMPLEMENT ERROR MESSAGE
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
                    exit(1);    // TODO: IMPLEMENT ERROR MESSAGE
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
                    exit(1);    // TODO: IMPLEMENT ERROR MESSAGE
                    break;
            }
            break;

        default:
            exit(1);            // TODO: IMPLEMENT ERROR MESSAGE
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
                    exit(1);    // TODO: IMPLEMENT ERROR MESSAGE
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
                    exit(1);    // TODO: IMPLEMENT ERROR MESSAGE
                    break;
            }
            break;

        default:
            exit(1);            // TODO: IMPLEMENT ERROR MESSAGE
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
            exit(1);            // TODO: IMPLEMENT ERROR MESSAGE 
            break;
    }
}
