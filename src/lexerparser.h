#pragma once

#include "ast.h"

#include <stdlib.h>

#define TT_EOF 0    // end of file
#define TT_INT 1    // integer literal
#define TT_FLOAT 2  // float literal

#define TT_ID 10    // identifier

#define TT_PO 20    // '('
#define TT_PC 21    // ')'
#define TT_BO 22    // '['
#define TT_BC 23    // ']'
#define TT_CO 24    // '{'
#define TT_CC 25    // '}'

#define TT_QM 30    // '?'
#define TT_COL 31   // ':'

#define TT_ASS 40   // '='

#define TT_ADD 60   // '+'
#define TT_SUB 61   // '-'
#define TT_MUL 62   // '*'
#define TT_DIV 63   // '/'

typedef struct token_t {
  uint8_t type;
  char *string;
} token;

token *create_token(uint8_t type, char *string);
token *create_eof_token();

token **lex(char *text);

expr *parse_expr(token **tokens, size_t *index);
expr *parse_opt_expr(expr *temp_expr, token **tokens, size_t *index);

expr **parse(token **tokens);

