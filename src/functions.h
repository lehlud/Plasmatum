#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define NULL_INDEX 0
#define NUM_INDEX 1
#define CHAR_INDEX 2
#define BOOL_INDEX 3

/*
 * This union is used to store the data in the one
 * data type (which actually is a struct) that exists
 * in Plasmatum.
 * 
 * double       -> at least as much storage as float
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
    int size;
} map;

map map_init();
plsm_dtype map_get(map*, char*);
void map_set(map*, char*, plsm_dtype);
void map_remove(map*, char*);



char *readfile(char*);

#endif
