#pragma once

typedef struct num_t {
    double value;
} num_t;

num_t *__plsm_construct_Num(double value);

num_t *__plsm_add_Num_Num(num_t *a, num_t *b);
num_t *__plsm_sub_Num_Num(num_t *a, num_t *b);
num_t *__plsm_mul_Num_Num(num_t *a, num_t *b);
num_t *__plsm_div_Num_Num(num_t *a, num_t *b);
num_t *__plsm_mod_Num_Num(num_t *a, num_t *b);
