#pragma once

#include "types.hh"

#include <vector>
#include <string>

#include "type.hh"

class Engine;
class Instruction;

class Value {
public:
  Type *type;

  Value(Type *type) : type(type) {}

  virtual ~Value() = default;

  virtual inline std::string toString() = 0;

  virtual inline bool isTruthy() = 0;
  virtual inline bool isConstant() { return false; };
};

class Constant : public Value {
public:
  Constant(Type *type) : Value(type) {}

  virtual ~Constant() = default;

  virtual inline bool isConstant() { return true; }
};

class UndefinedValue : public Constant {
public:
  UndefinedValue() : Constant(Type::getUndefinedType()) {}

  inline std::string toString() override { return "Undefined"; }

  inline bool isTruthy() override { return false; }
};

class IntegerValue : public Constant {
private:
  plsm_int_t value;

public:
  IntegerValue(plsm_int_t value)
      : Constant(Type::getIntegerType()), value(value) {}

  inline plsm_int_t getValue() { return value; }

  inline std::string toString() override { return std::to_string(value); }

  inline bool isTruthy() override { return value != 0; }
};

class FloatValue : public Constant {
private:
  plsm_float_t value;

public:
  FloatValue(plsm_float_t value)
      : Constant(Type::getFloatType()), value(value) {}

  inline plsm_float_t getValue() { return value; }

  inline std::string toString() override { return std::to_string(value); }

  inline bool isTruthy() override { return value != 0.0; }
};

class BooleanValue : public Constant {
private:
  plsm_bool_t value;

public:
  BooleanValue(plsm_bool_t value)
      : Constant(Type::getBooleanType()), value(value) {}

  inline plsm_bool_t getValue() { return value; }

  inline std::string toString() override { return value ? "True" : "False"; }

  inline bool isTruthy() override { return value == true; }
};

class FunctionValue : public Constant {
private:
  plsm_size_t argc;
  std::vector<Instruction *> instructions;

public:
  FunctionValue(plsm_size_t argc,
                const std::vector<Instruction *> &instructions)
      : Constant(Type::getFunctionType()), argc(argc), instructions(instructions) {}

  inline plsm_size_t getArgc() { return argc; }

  inline Instruction *getInstruction(plsm_size_t index) {
    return index >= instructions.size() ? nullptr : instructions[index];
  }

  void call(Engine *engine);

  inline std::string toString() override { return "Function Value"; }

  inline bool isTruthy() override { return true; }
};
