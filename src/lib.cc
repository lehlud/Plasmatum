#include "lib.hh"
#include "defines.hh"

#include <codecvt>
#include <cstdlib>
#include <iostream>
#include <locale>
#include <string>

inline double fmod(double a, double b) { return a - b * trunc(a / b); }

inline plsm_val null_plsm_val() { return (plsm_val){TYPE_NULL, nullptr}; }

inline plsm_val int_plsm_val(int64_t value) {
  int64_t *valueptr = (int64_t *)std::malloc(sizeof(int64_t));
  *valueptr = value;
  return (plsm_val){TYPE_INT, (int8_t *)valueptr};
}

inline plsm_val float_plsm_val(double value) {
  double *valueptr = (double *)std::malloc(sizeof(double));
  *valueptr = value;
  return (plsm_val){TYPE_FLOAT, (int8_t *)valueptr};
}

inline plsm_val string_plsm_val(char32_t *value) {
  size_t size = 0;
  while (*value++ != 0)
    size++;
  value -= size;

  char32_t **valueptr = (char32_t **)std::malloc(sizeof(char32_t *) * size);
  *valueptr = value;
  return (plsm_val){TYPE_STRING, (int8_t *)valueptr};
}

typedef plsm_val (*binexpr_func)(plsm_val a, plsm_val b);

plsm_val nullbinexpr(plsm_val a, plsm_val b) { return null_plsm_val(); }

plsm_val iiadd(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) + *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val ifadd(plsm_val a, plsm_val b) {
  double result = *((int64_t *)a.value) + *((double *)b.value);
  return float_plsm_val(result);
}

plsm_val fiadd(plsm_val a, plsm_val b) {
  double result = *((double *)a.value) + *((int64_t *)b.value);
  return float_plsm_val(result);
}

plsm_val ffadd(plsm_val a, plsm_val b) {
  double result = *((double *)a.value) + *((double *)b.value);
  return float_plsm_val(result);
}

plsm_val iisub(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) - *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val ifsub(plsm_val a, plsm_val b) {
  double result = *((int64_t *)a.value) - *((double *)b.value);
  return float_plsm_val(result);
}

plsm_val fisub(plsm_val a, plsm_val b) {
  double result = *((double *)a.value) - *((int64_t *)b.value);
  return float_plsm_val(result);
}

plsm_val ffsub(plsm_val a, plsm_val b) {
  double result = *((double *)a.value) - *((double *)b.value);
  return float_plsm_val(result);
}

plsm_val iimul(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) * *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val ifmul(plsm_val a, plsm_val b) {
  double result = *((int64_t *)a.value) * *((double *)b.value);
  return float_plsm_val(result);
}

plsm_val fimul(plsm_val a, plsm_val b) {
  double result = *((double *)a.value) * *((int64_t *)b.value);
  return float_plsm_val(result);
}

plsm_val ffmul(plsm_val a, plsm_val b) {
  double result = *((double *)a.value) * *((double *)b.value);
  return float_plsm_val(result);
}

plsm_val iidiv(plsm_val a, plsm_val b) {
  double result = *((int64_t *)a.value) / *((int64_t *)b.value);
  if (fmod(result, 1) == 0.0)
    return int_plsm_val((int64_t)result);
  else
    return float_plsm_val(result);
}

plsm_val ifdiv(plsm_val a, plsm_val b) {
  double result = *((int64_t *)a.value) / *((double *)b.value);
  return float_plsm_val(result);
}

plsm_val fidiv(plsm_val a, plsm_val b) {
  double result = *((double *)a.value) / *((int64_t *)b.value);
  return float_plsm_val(result);
}

plsm_val ffdiv(plsm_val a, plsm_val b) {
  double result = *((double *)a.value) / *((double *)b.value);
  return float_plsm_val(result);
}

plsm_val iimod(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) % *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val ifmod(plsm_val a, plsm_val b) {
  double result = fmod(*((int64_t *)a.value), *((double *)b.value));
  return float_plsm_val(result);
}

plsm_val fimod(plsm_val a, plsm_val b) {
  double result = fmod(*((double *)a.value), *((int64_t *)b.value));
  return float_plsm_val(result);
}

plsm_val ffmod(plsm_val a, plsm_val b) {
  double result = fmod(*((double *)a.value), *((double *)b.value));
  return float_plsm_val(result);
}

plsm_val iieq(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) == *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val ifeq(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) == *((double *)b.value);
  return int_plsm_val(result);
}

plsm_val fieq(plsm_val a, plsm_val b) {
  int64_t result = *((double *)a.value) == *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val ffeq(plsm_val a, plsm_val b) {
  int64_t result = *((double *)a.value) == *((double *)b.value);
  return int_plsm_val(result);
}

plsm_val iine(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) != *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val ifne(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) != *((double *)b.value);
  return int_plsm_val(result);
}

plsm_val fine(plsm_val a, plsm_val b) {
  int64_t result = *((double *)a.value) != *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val ffne(plsm_val a, plsm_val b) {
  int64_t result = *((double *)a.value) != *((double *)b.value);
  return int_plsm_val(result);
}

plsm_val iigt(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) > *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val ifgt(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) > *((double *)b.value);
  return int_plsm_val(result);
}

plsm_val figt(plsm_val a, plsm_val b) {
  int64_t result = *((double *)a.value) > *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val ffgt(plsm_val a, plsm_val b) {
  int64_t result = *((double *)a.value) > *((double *)b.value);
  return int_plsm_val(result);
}

plsm_val iilt(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) < *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val iflt(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) < *((double *)b.value);
  return int_plsm_val(result);
}

plsm_val filt(plsm_val a, plsm_val b) {
  int64_t result = *((double *)a.value) < *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val fflt(plsm_val a, plsm_val b) {
  int64_t result = *((double *)a.value) < *((double *)b.value);
  return int_plsm_val(result);
}

plsm_val iige(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) >= *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val ifge(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) >= *((double *)b.value);
  return int_plsm_val(result);
}

plsm_val fige(plsm_val a, plsm_val b) {
  int64_t result = *((double *)a.value) >= *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val ffge(plsm_val a, plsm_val b) {
  int64_t result = *((double *)a.value) >= *((double *)b.value);
  return int_plsm_val(result);
}

plsm_val iile(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) <= *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val ifle(plsm_val a, plsm_val b) {
  int64_t result = *((int64_t *)a.value) <= *((double *)b.value);
  return int_plsm_val(result);
}

plsm_val file(plsm_val a, plsm_val b) {
  int64_t result = *((double *)a.value) <= *((int64_t *)b.value);
  return int_plsm_val(result);
}

plsm_val ffle(plsm_val a, plsm_val b) {
  int64_t result = *((double *)a.value) <= *((double *)b.value);
  return int_plsm_val(result);
}

binexpr_func add_funcs[3][3] = {{nullbinexpr, nullbinexpr, nullbinexpr},
                                {nullbinexpr, iiadd, ifadd},
                                {nullbinexpr, fiadd, ffadd}};

binexpr_func sub_funcs[3][3] = {{nullbinexpr, nullbinexpr, nullbinexpr},
                                {nullbinexpr, iisub, ifsub},
                                {nullbinexpr, fisub, ffsub}};

binexpr_func mul_funcs[3][3] = {{nullbinexpr, nullbinexpr, nullbinexpr},
                                {nullbinexpr, iimul, ifmul},
                                {nullbinexpr, fimul, ffmul}};

binexpr_func div_funcs[3][3] = {{nullbinexpr, nullbinexpr, nullbinexpr},
                                {nullbinexpr, iidiv, ifdiv},
                                {nullbinexpr, fidiv, ffdiv}};

binexpr_func mod_funcs[3][3] = {{nullbinexpr, nullbinexpr, nullbinexpr},
                                {nullbinexpr, iimod, ifmod},
                                {nullbinexpr, fimod, ffmod}};

binexpr_func eq_funcs[3][3] = {{nullbinexpr, nullbinexpr, nullbinexpr},
                               {nullbinexpr, iieq, ifeq},
                               {nullbinexpr, fieq, ffeq}};

binexpr_func ne_funcs[3][3] = {{nullbinexpr, nullbinexpr, nullbinexpr},
                               {nullbinexpr, iine, ifne},
                               {nullbinexpr, fine, ffne}};

binexpr_func gt_funcs[3][3] = {{nullbinexpr, nullbinexpr, nullbinexpr},
                               {nullbinexpr, iigt, ifgt},
                               {nullbinexpr, figt, ffgt}};

binexpr_func lt_funcs[3][3] = {{nullbinexpr, nullbinexpr, nullbinexpr},
                               {nullbinexpr, iilt, iflt},
                               {nullbinexpr, filt, fflt}};

binexpr_func ge_funcs[3][3] = {{nullbinexpr, nullbinexpr, nullbinexpr},
                               {nullbinexpr, iige, ifge},
                               {nullbinexpr, fige, ffge}};

binexpr_func le_funcs[3][3] = {{nullbinexpr, nullbinexpr, nullbinexpr},
                               {nullbinexpr, iile, ifle},
                               {nullbinexpr, file, ffle}};

extern "C" plsm_val plsm_add(plsm_val a, plsm_val b) {
  return add_funcs[a.type][b.type](a, b);
}

extern "C" plsm_val plsm_sub(plsm_val a, plsm_val b) {
  return sub_funcs[a.type][b.type](a, b);
}

extern "C" plsm_val plsm_mul(plsm_val a, plsm_val b) {
  return mul_funcs[a.type][b.type](a, b);
}

extern "C" plsm_val plsm_div(plsm_val a, plsm_val b) {
  return div_funcs[a.type][b.type](a, b);
}

extern "C" plsm_val plsm_mod(plsm_val a, plsm_val b) {
  return mod_funcs[a.type][b.type](a, b);
}

extern "C" plsm_val plsm_eq(plsm_val a, plsm_val b) {
  return eq_funcs[a.type][b.type](a, b);
}

extern "C" plsm_val plsm_ne(plsm_val a, plsm_val b) {
  return ne_funcs[a.type][b.type](a, b);
}

extern "C" plsm_val plsm_gt(plsm_val a, plsm_val b) {
  return gt_funcs[a.type][b.type](a, b);
}

extern "C" plsm_val plsm_lt(plsm_val a, plsm_val b) {
  return lt_funcs[a.type][b.type](a, b);
}

extern "C" plsm_val plsm_ge(plsm_val a, plsm_val b) {
  return ge_funcs[a.type][b.type](a, b);
}

extern "C" plsm_val plsm_le(plsm_val a, plsm_val b) {
  return le_funcs[a.type][b.type](a, b);
}

extern "C" plsm_val plsm_getarg(uint64_t count, plsm_val *args,
                                uint64_t index) {
  if (index >= count)
    return null_plsm_val();
  else
    return args[index];
}

typedef int64_t (*logical_func)(plsm_val value);

int64_t null_logical(plsm_val value) { return 0; }

int64_t int_logical(plsm_val value) { return *((int64_t *)value.value); }

int64_t float_logical(plsm_val value) { return *((double *)value.value); }

int64_t string_logical(plsm_val value) {
  return *((uint32_t **)value.value)[0];
}

logical_func logical_funcs[] = {null_logical, int_logical, float_logical,
                                string_logical};

extern "C" int64_t plsm_logical(plsm_val value) {
  return logical_funcs[value.type](value);
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
  uint32_t *u32val = (uint32_t *)value;
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

extern "C" plsm_val plsm_print(int64_t count, plsm_val *args) {
  if (count == 0)
    return int_plsm_val(0);

  int64_t result = 0;
  for (int64_t i = 0; i < count - 1; i++) {
    result += plsm_printv(args[i]);
    result += plsm_prints("; ");
  }
  result += plsm_printv(args[count - 1]);
  return int_plsm_val(result);
}

extern "C" plsm_val plsm_println(int64_t count, plsm_val *args) {
  plsm_val result = plsm_print(count, args);
  std::cout << std::endl;
  return result;
}
