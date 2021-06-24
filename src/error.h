#pragma once

#include <stdint.h>

#define ERR_DEFAULT 1
#define ERR_LEXER 2
#define ERR_PARSER 3
#define ERR_COMPILER 4

void error(uint8_t type, char *msg);
void error_expected(uint8_t type, char *exp, char *got);
