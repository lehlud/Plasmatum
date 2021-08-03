#include "value.hh"

#include "engine.hh"
#include "instruction.hh"

#include <iostream>

namespace plsm {

plsm_float_t value::as_floating_point() { return ((floating_point *)this)->getValue(); }
plsm_int_t value::as_integer() { return ((integer *)this)->getValue(); }
plsm_bool_t value::as_boolean() { return ((boolean *)this)->getValue(); }

undefined *undefined::copy() {
  return new undefined();
}

integer *integer::copy() {
  return new integer(int_v);
}

floating_point *floating_point::copy() {
  return new floating_point(float_v);
}

boolean *boolean::copy() {
  return new boolean(bool_v);
}



} // namespace plsm
