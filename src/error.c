#include "error.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *errtype_tostr(uint8_t type) {
  switch (type) {
  case ERR_DEFAULT:
    return "error";
  case ERR_LEXER:
    return "lexer error";
  case ERR_PARSER:
    return "parser error";
  case ERR_COMPILER:
    return "compiler error";
  }

  return "unknown error";
}

void error(uint8_t type, char *msg) {
  char *errtype = errtype_tostr(type);

  char *format = "\x1B[91m\x1B[1m%s:\x1B[0m %s";
  char *error_text = plsm_sprintf(format, errtype, msg);
  puts(error_text);

  exit(1);
}

void error_expected(uint8_t type, char *exp, char *got) {
  char *msg;
  sprintf(msg, "expected: %s, got: %s", exp, got);
  error(type, msg);
}
