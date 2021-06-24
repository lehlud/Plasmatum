#include "utils.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *plsm_sprintf(const char *format, ...) {
  va_list args;
  va_start(args, format);

  size_t size = vsnprintf(NULL, 0, format, args) + 1;

  va_end(args);
  va_start(args, format);

  char *result = malloc(size * sizeof(char));
  vsprintf(result, format, args);

  va_end(args);

  return result;
}

void plsm_str_appendc(char *str, char c) {
  size_t len = strlen(str) + 1;
  str = (char*) realloc(str, len * sizeof(char));
  sprintf(str, "%s%c", str, c);
}

char *readfile(const char *fname) {
  FILE *file = fopen(fname, "r");

  if (!file) {
    char *msg;
    sprintf(msg, "cannot read from file '%s'", fname);
    error(ERR_DEFAULT, msg);
  }

  fseek(file, 0, SEEK_END);
  size_t fsize = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *text = malloc(fsize);

  fread(text, 1, fsize, file);
  fclose(file);

  return text;
}

int is_digit(char c) { return c >= '0' && c <= '9'; }

int is_letter(char c) {
  return c >= 'A' && c <= 'z';
}

int is_idchar(char c) {
  return is_digit(c) || is_letter(c) || c == '_'; 
}

int is_special(char c) {
  return c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' ||
         c == '*' || c == '?' || c == ':' || c == '+' || c == '-' || c == '*' ||
         c == '/' || c == '%' || c == '<' || c == '>' || c == '=';
}

int is_whitespace(char c) { return c >= 0 && c <= ' '; }
