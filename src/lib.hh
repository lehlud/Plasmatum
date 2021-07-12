#pragma once

#include "defines.hh"

extern "C" plsm_val null_plsm_val(int64_t value);
extern "C" plsm_val int_plsm_val(int64_t value);
extern "C" plsm_val float_plsm_val(double value);
extern "C" plsm_val string_plsm_val(char32_t *value);

extern "C" plsm_val print(int64_t count, plsm_val *value);
extern "C" plsm_val println(int64_t count, plsm_val *value);
