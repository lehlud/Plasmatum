#include "builtins.h"

#include <math.h>
#include <stdlib.h>

num_t *__plsm_construct_Num(double value) {
    num_t *num = (num_t *)malloc(sizeof(num_t));
    num->value = value;

    return num;
}

num_t *__plsm_add_Num_Num(num_t *a, num_t *b) {
    return __plsm_construct_Num(a->value + b->value);
}

num_t *__plsm_sub_Num_Num(num_t *a, num_t *b) {
    return __plsm_construct_Num(a->value - b->value);
}

num_t *__plsm_mul_Num_Num(num_t *a, num_t *b) {
    return __plsm_construct_Num(a->value * b->value);
}

num_t *__plsm_div_Num_Num(num_t *a, num_t *b) {
    return __plsm_construct_Num(a->value / b->value);
}

num_t *__plsm_mod_Num_Num(num_t *a, num_t *b) {
    return __plsm_construct_Num(fmod(a->value, b->value));
}