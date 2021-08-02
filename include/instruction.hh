#pragma once

#include "types.hh"

#include <functional>
#include <string>

namespace plsm {

class Type;
class Engine;
class Constant;

class Instruction {
public:
  virtual ~Instruction() = default;
  virtual plsm_size_t execute(Engine *engine) = 0;

  virtual inline std::string toString() = 0;

  virtual bool isReturn() { return false; }
  virtual bool isFunctionFinish() { return false; }
};

class ReturnInstruction : public Instruction {
public:
  plsm_size_t execute(Engine *) override { return 1; }

  virtual inline std::string toString() override { return "RETURN"; }

  bool isReturn() override { return true; }
};

class JumpInstruction : public Instruction {
private:
  plsm_size_t destination;

public:
  JumpInstruction(plsm_size_t destination) : destination(destination) {}

  plsm_size_t execute(Engine *engine) override;

  virtual inline std::string toString() override { return "JUMP"; }

  plsm_size_t getDestination() { return destination; };
};

class LoadConstInstruction : public Instruction {
private:
  Constant *value;

public:
  LoadConstInstruction(Constant *value) : value(value) {}

  virtual inline std::string toString() override { return "LOAD_CONST"; }

  plsm_size_t execute(Engine *engine) override;
};

class LoadGlobalInstruction : public Instruction {
private:
  std::string id;

public:
  LoadGlobalInstruction(const std::string &id) : id(id) {}

  virtual inline std::string toString() override { return "LOAD_GLOBAL"; }

  plsm_size_t execute(Engine *engine) override;
};

class CastInstruction : public Instruction {
private:
  Type *type;

public:
  CastInstruction(Type *type) : type(type) {}

  virtual inline std::string toString() override { return "CAST"; }

  plsm_size_t execute(Engine *engine) override;
};

class CustomInstruction : public Instruction {
private:
  std::function<plsm_size_t(Engine *)> executeFunction;

public:
  CustomInstruction(const std::function<plsm_size_t(Engine *)> &executeFunction)
      : executeFunction(executeFunction) {}

  virtual inline std::string toString() override { return "CUSTOM"; }

  plsm_size_t execute(Engine *engine) override;
};

class AddInstruction : public Instruction {
public:
  virtual inline std::string toString() override { return "ADD"; }

  plsm_size_t execute(Engine *engine) override;
};

class SubInstruction : public Instruction {
public:
  virtual inline std::string toString() override { return "SUB"; }

  plsm_size_t execute(Engine *engine) override;
};

class MulInstruction : public Instruction {
public:
  virtual inline std::string toString() override { return "MUL"; }

  plsm_size_t execute(Engine *engine) override;
};

class DivInstruction : public Instruction {
public:
  virtual inline std::string toString() override { return "DIV"; }

  plsm_size_t execute(Engine *engine) override;
};

class ModInstruction : public Instruction {
public:
  virtual inline std::string toString() override { return "MOD"; }

  plsm_size_t execute(Engine *engine) override;
};

class CallInstruction : public Instruction {
private:
  plsm_size_t argc;

public:
  CallInstruction(plsm_size_t argc) : argc(argc) {}

  virtual inline std::string toString() override { return "CALL"; }

  plsm_size_t execute(Engine *engine) override;
};

class FunctionStartInstruction : public Instruction {
private:
  plsm_size_t argc;

public:
  FunctionStartInstruction(plsm_size_t argc) : argc(argc) {}

  virtual inline std::string toString() override { return "FUNC_START"; }

  plsm_size_t execute(Engine *engine) override;
};

class FunctionFinishInstruction : public Instruction {
public:
  virtual inline std::string toString() override { return "FUNC_FINISH"; }

  plsm_size_t execute(Engine *) override { return 1; }

  bool isFunctionFinish() override { return true; }
};

class DefineGlobalInstruction : public Instruction {
private:
  std::string id;

public:
  DefineGlobalInstruction(const std::string &id) : id(id) {}

  virtual inline std::string toString() override { return "DEF_GLOBAL"; }

  plsm_size_t execute(Engine *engine) override;
};

}
