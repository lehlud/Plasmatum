#include "ast.h"

#include <stdlib.h>

expr *create_int(int_t int_v) {
  expr *temp_expr = malloc(sizeof(expr));
  temp_expr->used = ET_INT;
  temp_expr->int_v = int_v;

  return temp_expr;
}

expr *create_float(float_t float_v) {
  expr *temp_expr = malloc(sizeof(expr));
  temp_expr->used = ET_FLOAT;
  temp_expr->float_v = float_v;

  return temp_expr;
}

expr *create_identifier(char *identifier) {
  expr *temp_expr = malloc(sizeof(expr));
  temp_expr->used = ET_IDENTIFIER;
  temp_expr->identifier = identifier;

  return temp_expr;
}

expr *create_ifexpr(expr *cond, expr *true_v, expr *false_v) {
  ifexpr *temp_ifexpr = malloc(sizeof(ifexpr));
  temp_ifexpr->cond = cond;
  temp_ifexpr->true_v = true_v;
  temp_ifexpr->false_v = false_v;

  expr *temp_expr = malloc(sizeof(expr));
  temp_expr->used = ET_IFEXPR;
  temp_expr->ifexpr = temp_ifexpr;

  return temp_expr;
}

expr *create_binexpr(uint8_t type, expr *left, expr *right) {
  binexpr *temp_binexpr = malloc(sizeof(binexpr));
  temp_binexpr->type = type;
  temp_binexpr->left = left;
  temp_binexpr->right = right;

  expr *temp_expr = malloc(sizeof(expr));
  temp_expr->used = ET_BINEXPR;
  temp_expr->binexpr = temp_binexpr;

  return temp_expr;
}

expr *create_assignexpr(char *id, expr *value) {
  assignexpr *temp_assignexpr = malloc(sizeof(assignexpr));
  temp_assignexpr->id = id;
  temp_assignexpr->value = value;

  expr *temp_expr = malloc(sizeof(expr));
  temp_expr->used = ET_ASSIGNEXPR;
  temp_expr->assignexpr = temp_assignexpr;

  return temp_expr;
}

expr *create_loopexpr(expr *cond, expr *body) {
  loopexpr *temp_loopexpr = malloc(sizeof(loopexpr));
  temp_loopexpr->cond = cond;
  temp_loopexpr->body = body;

  expr *temp_expr = malloc(sizeof(expr));
  temp_expr->used = ET_LOOPEXPR;
  temp_expr->loopexpr = temp_loopexpr;

  return temp_expr;
}

expr *create_callexpr(expr *callee, expr **args) {
  callexpr *temp_callexpr = malloc(sizeof(callexpr));
  temp_callexpr->callee = callee;
  temp_callexpr->args = args;

  expr *temp_expr = malloc(sizeof(expr));
  temp_expr->used = ET_CALLEXPR;
  temp_expr->callexpr = temp_callexpr;

  return temp_expr;
}

expr *create_function(char **args, expr *body) {
  function *temp_function = malloc(sizeof(function));
  temp_function->args = args;
  temp_function->body = body;

  expr *temp_expr = malloc(sizeof(expr));
  temp_expr->used = ET_FUNCTION;
  temp_expr->function = temp_function;

  return temp_expr;
}
