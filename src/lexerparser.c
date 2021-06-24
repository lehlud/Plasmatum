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

token *create_eof() {return create_token(TT_EOF, "end of file");}

token *lex_token(char *text, size_t *index) {
#define getc(idx) idx >= len ? -1 : text[idx]
  size_t len = strlen(text);

  char c = getc(*index), c1;
  while (is_whitespace(c) && c != -1) {
    *index += 1;
    c = getc(*index);
  }

  if (c == -1) {
    return create_eof();
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
    plsm_str_appendc(temp_str, c);
    *index += 1;
    c = getc(*index);

    while (is_digit(c)) {
      plsm_str_appendc(temp_str, c);
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

    plsm_str_appendc(temp_str, c);
    *index += 1;
    c = getc(*index);

    while (is_digit(c)) {
      plsm_str_appendc(temp_str, c);
      *index += 1;
      c = getc(*index);
    }

    return create_token(TT_FLOAT, temp_str);
  }

  if (is_letter(c) || c == '_') {
    plsm_str_appendc(temp_str, c);
    *index += 1;
    c = getc(*index);

    while (is_idchar(c)) {
      plsm_str_appendc(temp_str, c);
      *index += 1;
      c = getc(*index);
    }

    return create_token(TT_ID, temp_str);
  }

  if (c == -1) {
    return create_eof();
  }

  error(ERR_LEXER, plsm_sprintf("unexpected input ('%c')", c));

  return create_eof();

#undef getc
}

token **lex(char *text) {
  size_t token_size = 0;
  token **result = malloc(0);

  size_t index = 0;

  token *t;
  while ((t = lex_token(text, &index))->type != TT_EOF) {
    result = realloc(result, ++token_size * sizeof(token*));
    result[token_size - 1] = t;
  }

  result = realloc(result, ++token_size * sizeof(token*));
  result[token_size - 1] = 0;

  return result;
}

expr **parse(token **tokens) {
  return 0;
}
