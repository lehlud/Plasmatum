#pragma once

#include <stdarg.h>
#include <stdlib.h>
int plsm_streq(const char *s1, const char *s2);
char *plsm_sprintf(const char *format, ...);
void plsm_str_appendc(char **str, char c);

char *readfile(const char *fname);

int is_digit(char c);
int is_letter(char c);
int is_idchar(char c);
int is_special(char c);
int is_whitespace(char c);

#define array_push(size, type, array, element) \
  array = (type *) realloc(array, ++size * sizeof(element)); \
  array[size - 1] = element
