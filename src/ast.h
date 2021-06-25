#pragma once

#include <stdint.h>

typedef long long int_t;
typedef long double float_t;

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
#define ET_ASSIGNEXPR 33
#define ET_LOOPEXPR 34
#define ET_CALLEXPR 35
#define ET_FUNCTION 36

struct expr_t {
  uint8_t used;
  union {
    int_t int_v;
    float_t float_v;

    char *identifier;
    ifexpr *ifexpr;
    binexpr *binexpr;
    assignexpr *assignexpr;
    loopexpr *loopexpr;
    callexpr *callexpr;
    function *function;
  };
};

expr *create_int(int_t int_v);
expr *create_float(float_t float_v);
expr *create_identifier(char *identifier);
expr *create_ifexpr(expr *cond, expr *true_v, expr *false_v);
expr *create_binexpr(uint8_t type, expr *left, expr *right);
expr *create_assignexpr(char *id, expr *value);
expr *create_loopexpr(expr *cond, expr *body);
expr *create_callexpr(expr *callee, expr **args);
expr *create_function(char **args, expr *body);
