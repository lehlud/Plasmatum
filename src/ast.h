#pragma once

#include <stdint.h>

typedef struct expr_t expr;

typedef struct ifexpr_t {
  expr *cond, *true_v, *false_v;
} ifexpr;

#define BET_ADD 1
#define BET_SUB 2
#define BET_MUL 3
#define BET_DIV 4

typedef struct binexpr_t {
  uint8_t type;
  expr *left, *right;
} binexpr;

typedef struct assignexpr_t {
  char *id;
  expr *value;
} assignexpr;

typedef struct loopexpr_t {
  expr *cond, *body;
} loopexpr;

typedef struct callexpr_t {
  expr *callee, **args;
} callexpr;

typedef struct function_t {
  char **args;
  expr *body;
} function;

#define ET_INT 1
#define ET_FLOAT 2

#define ET_IDENTIFIER 30
#define ET_IFEXPR 31
#define ET_BINEXPR 32
#define ET_FUNCTION 33

struct expr_t {
  uint8_t used;
  union {
    long long int_v;
    long double float_v;

    char *identifier;
    ifexpr *ifexpr;
    binexpr *binexpr;
    function *function;
  };
};
