#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define NULL_INDEX 0
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


typedef struct map {
    char **keys;
    plsm_dtype *values;
    int size;
} map;

map map_init();
plsm_dtype map_get(map*, char*);
void map_set(map*, char*, plsm_dtype);
void map_remove(map*, char*);

#endif
