#include "lib.hh"

#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

extern "C" plsm_val null_plsm_val(int64_t value) { return (plsm_val){TYPE_NULL, nullptr}; }

extern "C" plsm_val int_plsm_val(int64_t value) {
  int64_t *valueptr = (int64_t *)std::malloc(sizeof(int64_t));
  *valueptr = value;
  return (plsm_val){TYPE_INT, (int8_t *)valueptr};
}

extern "C" plsm_val float_plsm_val(double value) {
  double *valueptr = (double *)std::malloc(sizeof(double));
  *valueptr = value;
  return (plsm_val){TYPE_FLOAT, (int8_t *)valueptr};
}

extern "C" plsm_val string_plsm_val(char32_t *value) {
  char32_t **valueptr = (char32_t **)std::malloc(sizeof(char32_t *));
  *valueptr = value;
  return (plsm_val){TYPE_STRING, (int8_t *)valueptr};
}

inline int64_t plsm_prints(const std::string &string) {
  std::cout << string;
  return string.size();
}

int64_t print_null(int8_t *value) { return plsm_prints("Null"); }

int64_t print_int(int8_t *value) {
  return plsm_prints(std::to_string(*(int64_t *)value));
}

int64_t print_float(int8_t *value) {
  return plsm_prints(std::to_string(*(double *)value));
}

int64_t print_string(int8_t *value) {
  uint32_t *u32val = *(uint32_t **)value;
  std::u32string u32string((char32_t *)u32val);

  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;

  return plsm_prints(conv.to_bytes(u32string));
}

int64_t print_list(int8_t *value) { return plsm_prints("<List>"); }

int64_t print_map(int8_t *value) { return plsm_prints("<Map>"); }

typedef int64_t (*printfunc)(int8_t *);

printfunc printfuncs[] = {
    print_null, print_int, print_float, print_string, print_list, print_map,
};

inline int64_t plsm_printv(plsm_val value) {
  if (value.type >= sizeof(printfuncs) / sizeof(printfunc) || value.type < 0) {
    return plsm_prints("<error-type>");
  } else {
    return printfuncs[value.type](value.value);
  }
}

extern "C" plsm_val print(int64_t count, plsm_val *args) {
  if (count == 0) return int_plsm_val(0);

  int64_t result = 0;
  for (int64_t i = 0; i < count - 1; i++) {
    result += plsm_printv(args[i]);
    result += plsm_prints("; ");
  }
  result += plsm_printv(args[count - 1]);
  return int_plsm_val(result);
}

extern "C" plsm_val println(int64_t count, plsm_val *args) {
  plsm_val result = print(count, args);
  std::cout << std::endl;
  return result;
}
