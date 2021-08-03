#pragma once

#include "types.hh"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "type.hh"
#include "function.hh"

namespace plsm {

class instruction;
class execution_engine;

class value {
public:
  Type *type;

  value(Type *type) : type(type) {}

  virtual ~value() = default;

  virtual inline std::string to_string() = 0;

  plsm_int_t as_integer();
  plsm_bool_t as_boolean();
  plsm_float_t as_floating_point();

  virtual value *copy() = 0;

  virtual inline bool is_truthy() = 0;
  virtual inline bool is_constant() { return false; };

  virtual inline bool is_floating_point() { return false; }
  virtual inline bool is_integer() { return false; }
  virtual inline bool is_boolean() { return false; }
  virtual inline bool is_function() { return false; }
};

class constant : public value {
public:
  constant(Type *type) : value(type) {}
  virtual ~constant() = default;

  virtual inline bool is_constant() { return true; }
};

class undefined : public constant {
public:
  undefined() : constant(Type::getUndefinedType()) {}

  undefined *copy() override;

  inline std::string to_string() override { return "Undefined"; }

  inline bool is_truthy() override { return false; }
};

class integer : public constant {
private:
  plsm_int_t int_v;

public:
  integer(plsm_int_t int_v) : constant(Type::getIntegerType()), int_v(int_v) {}

  integer *copy() override;

  inline plsm_int_t getValue() { return int_v; }

  inline std::string to_string() override { return std::to_string(int_v); }

  inline bool is_truthy() override { return int_v != 0; }
  inline bool is_integer() override { return true; }
};

class floating_point : public constant {
private:
  plsm_float_t float_v;

public:
  floating_point(plsm_float_t float_v)
      : constant(Type::getFloatType()), float_v(float_v) {}

  floating_point *copy() override;

  inline plsm_float_t getValue() { return float_v; }

  inline std::string to_string() override { return std::to_string(float_v); }

  inline bool is_truthy() override { return float_v != 0.0; }
  inline bool is_floating_point() override { return true; }
};

class boolean : public constant {
private:
  plsm_bool_t bool_v;

public:
  boolean(plsm_bool_t bool_v)
      : constant(Type::getBooleanType()), bool_v(bool_v) {}

  boolean *copy() override;

  inline plsm_bool_t getValue() { return bool_v; }

  inline std::string to_string() override { return bool_v ? "True" : "False"; }

  inline bool is_truthy() override { return bool_v == true; }
  inline bool is_boolean() override { return true; }
};

class function_pointer : public constant {
private:
  function *_function;

public:
  function_pointer(function *_function)
      : constant(Type::getFunctionType()), _function(_function) {}

  function_pointer *copy() override {
    return new function_pointer(_function);
  }

  inline function *get_function() { return _function; }

  inline void call(execution_engine *engine) {
    _function->call(engine);
  }

  inline std::string to_string() override { return "function"; }

  inline bool is_truthy() override { return true; }
  inline bool is_function() override { return true; }
};

} // namespace plsm
