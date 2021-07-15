#pragma once

#define INT_SIZE 64
#define CHAR_SIZE 8

#include <stdint.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>

typedef struct _plsm_val {
    int8_t type;
    int8_t *value;
} plsm_val;

#define TYPE_NULL 0
#define TYPE_INT 1
#define TYPE_FLOAT 2
#define TYPE_STRING 3
#define TYPE_LIST 4
#define TYPE_MAP 5
