#include "lexerparser.h"
#include "error.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token *create_token(uint8_t type, char *string) {
  token *t = malloc(sizeof(token));
  t->type = type;
  t->string = string;
  return t;
}

token *create_eof_token() { return create_token(TT_EOF, "end of file"); }

token *lex_token(char *text, size_t *index) {
#define getc(idx) idx >= len ? -1 : text[idx]
  size_t len = strlen(text);

  char c = getc(*index), c1;
  while (is_whitespace(c) && c != -1) {
    *index += 1;
    c = getc(*index);
  }

  if (c == -1) {
    return create_eof_token();
  }

  switch (c) {
  case '(':
    *index += 1;
    return create_token(TT_PO, "(");
  case ')':
    *index += 1;
    return create_token(TT_PC, ")");
  case '[':
    *index += 1;
    return create_token(TT_BO, "[");
  case ']':
    *index += 1;
    return create_token(TT_BC, "]");
  case '{':
    *index += 1;
    return create_token(TT_CO, "{");
  case '}':
    *index += 1;
    return create_token(TT_CC, "}");
  case '?':
    *index += 1;
    return create_token(TT_QM, "?");
  case ':':
    *index += 1;
    return create_token(TT_COL, ":");
  case '+':
    *index += 1;
    return create_token(TT_ADD, "+");
  case '-':
    *index += 1;
    return create_token(TT_SUB, "-");
  case '*':
    *index += 1;
    return create_token(TT_MUL, "*");
  case '/':
    *index += 1;
    return create_token(TT_DIV, "/");
  case '=':
    *index += 1;
    return create_token(TT_ASS, "=");
  }

  char *temp_str = malloc(sizeof(char));
  temp_str[0] = 0;

  if (is_digit(c)) {
    plsm_str_appendc(&temp_str, c);
    *index += 1;
    c = getc(*index);

    while (is_digit(c)) {
      plsm_str_appendc(&temp_str, c);
      *index += 1;
      c = getc(*index);
    }

    if (c != '.') {
      return create_token(TT_INT, temp_str);
    }
  }

  if (c == '.') {
    if (!is_digit(getc(*index + 1))) {
      error_expected(ERR_LEXER, "digit after dot", plsm_sprintf("'%c'", c));
    }

    plsm_str_appendc(&temp_str, c);
    *index += 1;
    c = getc(*index);

    while (is_digit(c)) {
      plsm_str_appendc(&temp_str, c);
      *index += 1;
      c = getc(*index);
    }

    return create_token(TT_FLOAT, temp_str);
  }

  if (is_letter(c) || c == '_') {
    plsm_str_appendc(&temp_str, c);
    *index += 1;
    c = getc(*index);

    while (is_idchar(c)) {
      plsm_str_appendc(&temp_str, c);
      *index += 1;
      c = getc(*index);
    }

    return create_token(TT_ID, temp_str);
  }

  if (c == -1) {
    return create_eof_token();
  }

  error(ERR_LEXER, plsm_sprintf("unexpected input ('%c')", c));

  return create_eof_token();

#undef getc
}

token **lex(char *text) {
  size_t init_token_size = 1;
  size_t token_size = init_token_size;
  token **result = malloc(token_size * sizeof(token *));

  size_t index = 0;

  token *t;
  while ((t = lex_token(text, &index))->type != TT_EOF) {
    result = realloc(result, ++token_size * sizeof(token *));
    result[token_size - init_token_size - 1] = t;
  }

  result[token_size - 1] = 0;

  return result;
}

expr *parse_expr(token **tokens, size_t *index) {
  token *t = tokens[*index];

  expr *result = 0;

  if (t == 0) {
    return 0;
  } else if (t->type == TT_ID) {
    if (plsm_streq(t->string, "define")) {
      *index += 1;
    } else {
      *index += 1;
      result = create_identifier(t->string);
    }
  } else if (t->type == TT_INT) {
    *index += 1;
    result = create_int(strtoll(t->string, 0, 10));
  } else if (t->type == TT_FLOAT) {
    *index += 1;
    result = create_float(strtold(t->string, 0));
  }

  return parse_opt_expr(result, tokens, index);
}

expr *parse_opt_expr(expr *temp_expr, token **tokens, size_t *index) {
  token *t = tokens[*index];

  if (t == 0) {
    return 0;
  } else if (t->type >= TT_ADD && t->type <= TT_DIV) {
    *index += 1;
    expr *right = parse_expr(tokens, index);
    return create_binexpr(t->type - 60, temp_expr, right);
  } else if (t->type == TT_QM) {
    *index += 1;
    expr *true_v = parse_expr(tokens, index);
    
    if (t->type != TT_COL) {
      error_expected(ERR_PARSER, "':'", plsm_sprintf("'%s'", t->string));
    }

    *index += 1;

    expr *false_v = parse_expr(tokens, index);

    return create_ifexpr(temp_expr, true_v, false_v);
  }


  return 0;
}

expr **parse(token **tokens) {
  size_t index = 0;
  size_t init_exprs_size = 1;
  size_t exprs_size = init_exprs_size;
  expr **result = malloc(exprs_size * sizeof(expr*));

  expr *e;
  while ((e = parse_expr(tokens, &index)) != 0) {
    result = realloc(result, ++exprs_size * sizeof(expr*));
    result[exprs_size - init_exprs_size - 1] = e;
  }

  result[exprs_size - 1] = 0;

  return result;
}
