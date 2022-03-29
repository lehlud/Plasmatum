#pragma once

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

typedef void (*destructor)(value_t *);
typedef value_t *(*binop_func)(value_t *, value_t *);

typedef struct type_t {
    int8_t *name;
    int8_t **attrs;

    destructor destructor;

    binop_func add;
    binop_func sub;
    binop_func mul;
    binop_func div;
    binop_func mod;

    uint64_t hash_code;
} type_t;

typedef struct value_t {
    type_t *type;
    void *data;
} value_t;

typedef struct Num_t {
    type_t *type;
    struct { _Float64 value; } value;
} Num_t;

static constexpr inline uint64_t hash_code(type_t type) {
    uint64_t hash = 0;

    hash = hash * 31 + (uint64_t) type.name;
    for (int i = 0; type.attrs[i] != NULL; i++) {
        hash = hash * 31 + (uint64_t) type.attrs[i];
    }
    hash = hash * 31 + (uint64_t) type.destructor;
    hash = hash * 31 + (uint64_t) type.add;
    hash = hash * 31 + (uint64_t) type.sub;
    hash = hash * 31 + (uint64_t) type.mul;
    hash = hash * 31 + (uint64_t) type.div;
    hash = hash * 31 + (uint64_t) type.mod;

    return hash;
}

Num_t *__plsm_construct_Num(_Float64 value);

value_t *__plsm_Num_add(value_t *a, value_t *b);
value_t *__plsm_Num_sub(value_t *a, value_t *b);
value_t *__plsm_Num_mul(value_t *a, value_t *b);
value_t *__plsm_Num_div(value_t *a, value_t *b);
value_t *__plsm_Num_mod(value_t *a, value_t *b);
