#pragma once

#include <vector>

#include "types.hh"

class Type;
class Engine;
class Instruction;

class Value {
public:
  Type *type;

  Value(Type *type) : type(type) {}

  virtual ~Value() = default;

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
  UndefinedValue() : Constant(nullptr) {}

  inline bool isTruthy() override { return false; }
};

class IntegerValue : public Constant {
private:
  int_t value;

public:
  IntegerValue(int_t value) : Constant(nullptr), value(value) {}

  inline bool isTruthy() override { return value != 0; }
};

class FloatValue : public Constant {
private:
  float_t value;

public:
  FloatValue(float_t value) : Constant(nullptr), value(value) {}

  inline bool isTruthy() override { return value != 0.0; }
};

class BooleanValue : public Constant {
private:
  bool_t value;

public:
  BooleanValue(bool_t value) : Constant(nullptr), value(value) {}

  inline bool isTruthy() override { return value == true; }
};

class FunctionValue : public Constant {
private:
  fast_size_t argc;
  std::vector<Instruction *> instructions;

public:
  FunctionValue(fast_size_t argc, const std::vector<Instruction *> &instructions)
      : Constant(nullptr), argc(argc), instructions(instructions) {}

  inline fast_size_t getArgc() { return argc; }

  inline Instruction *getInstruction(fast_size_t index) {
    return index >= instructions.size() ? nullptr : instructions[index];
  }

  inline bool isTruthy() override { return true; }

  void call(Engine *engine);
};
