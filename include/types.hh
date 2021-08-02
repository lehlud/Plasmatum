#pragma once

#include <stdint.h>

namespace plsm {

typedef uint_fast64_t plsm_size_t;

typedef int_fast64_t plsm_int_t;
typedef double plsm_float_t;
typedef bool plsm_bool_t;

template<typename Base, typename T>
inline bool instanceof(const T *pointer) {
    return dynamic_cast<const Base*>(pointer) != nullptr;
}

}
