#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define NUM_INDEX 1
#define CHAR_INDEX 2
#define BOOL_INDEX 3

union plsm_dtype_union {
    double num_v;
};

typedef struct plsm_dtype {
    int used;
    union plsm_dtype_union data;
} plsm_dtype;


plsm_dtype add(plsm_dtype, plsm_dtype);
plsm_dtype sub(plsm_dtype, plsm_dtype);
plsm_dtype mul(plsm_dtype, plsm_dtype);
plsm_dtype pdiv(plsm_dtype, plsm_dtype);
plsm_dtype pmod(plsm_dtype, plsm_dtype);
plsm_dtype ppow(plsm_dtype, plsm_dtype);

plsm_dtype cast(plsm_dtype, int);

void printval(plsm_dtype val);

#endif
