#include "builtins.h"

type_t num_type = {
    .name = (int8_t *)"Num",
    .attrs = NULL,
    .destructor = NULL,
    .add = __plsm_Num_add,
    .sub = __plsm_Num_sub,
    .mul = __plsm_Num_mul,
    .div = __plsm_Num_div,
    .mod = __plsm_Num_mod,
    .hash_code = hash_code(num_type),
};

extern "C" Num_t *__plsm_construct_Num(_Float64 value) {
    auto result = (Num_t *) malloc(sizeof(Num_t));
    result->type = &num_type;
    result->value.value = value;
    return result;
}

value_t *__plsm_Num_add(value_t *a, value_t *b) {
    if (a->type != &num_type || b->type != &num_type) return NULL;

    return (value_t *)__plsm_construct_Num(
        ((Num_t *) a->data)->value.value +
        ((Num_t *) b->data)->value.value);
}

value_t *__plsm_Num_sub(value_t *a, value_t *b) {
    if (a->type != &num_type || b->type != &num_type) return NULL;

    return (value_t *)__plsm_construct_Num(
        ((Num_t *) a->data)->value.value -
        ((Num_t *) b->data)->value.value);
}

value_t *__plsm_Num_mul(value_t *a, value_t *b) {
    if (a->type != &num_type || b->type != &num_type) return NULL;

    return (value_t *)__plsm_construct_Num(
        ((Num_t *) a->data)->value.value *
        ((Num_t *) b->data)->value.value);
}

value_t *__plsm_Num_div(value_t *a, value_t *b) {
    if (a->type != &num_type || b->type != &num_type) return NULL;

    return (value_t *)__plsm_construct_Num(
        ((Num_t *) a->data)->value.value /
        ((Num_t *) b->data)->value.value);
}

value_t *__plsm_Num_mod(value_t *a, value_t *b) {
    if (a->type != &num_type || b->type != &num_type) return NULL;

    return (value_t *)__plsm_construct_Num(
        fmodf64(((Num_t *) a->data)->value.value,
        ((Num_t *) b->data)->value.value));
}
