#pragma once

#include "defines.hh"

extern "C" plsm_val __plsm_add(plsm_val a, plsm_val b);
extern "C" plsm_val __plsm_sub(plsm_val a, plsm_val b);
extern "C" plsm_val __plsm_mul(plsm_val a, plsm_val b);
extern "C" plsm_val __plsm_div(plsm_val a, plsm_val b);
extern "C" plsm_val __plsm_mod(plsm_val a, plsm_val b);
extern "C" plsm_val __plsm_eq(plsm_val a, plsm_val b);
extern "C" plsm_val __plsm_ne(plsm_val a, plsm_val b);
extern "C" plsm_val __plsm_gt(plsm_val a, plsm_val b);
extern "C" plsm_val __plsm_lt(plsm_val a, plsm_val b);
extern "C" plsm_val __plsm_ge(plsm_val a, plsm_val b);
extern "C" plsm_val __plsm_le(plsm_val a, plsm_val b);

extern "C" plsm_val __plsm_getarg(uint64_t count, plsm_val *args, uint64_t index);

extern "C" int64_t __plsm_logical(plsm_val value);

extern "C" void __plsm_free(plsm_val value);
extern "C" plsm_val __plsm_memcpy(plsm_val value);

extern "C" plsm_val __plsm_print(int64_t count, plsm_val *args);
extern "C" plsm_val __plsm_println(int64_t count, plsm_val *args);
