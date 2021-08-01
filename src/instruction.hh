#pragma once

#include "types.hh"

#include <string>
#include <functional>

class Type;
class Engine;
class Constant;

class Instruction {
public:
  virtual ~Instruction() = default;
  virtual plsm_size_t execute(Engine *engine) = 0;

  virtual bool isReturn() { return false; }
  virtual bool isFunctionFinish() { return false; }
};

class ReturnInstruction : public Instruction {
public:
  plsm_size_t execute(Engine *) override { return 1; }

  bool isReturn() override { return true; }
};

class JumpInstruction : public Instruction {
private:
  plsm_size_t destination;

public:
  JumpInstruction(plsm_size_t destination) : destination(destination) {}

  plsm_size_t execute(Engine *engine) override;

  plsm_size_t getDestination() { return destination; };
};

class LoadConstInstruction : public Instruction {
private:
  Constant *value;

public:
  LoadConstInstruction(Constant *value) : value(value) {}

  plsm_size_t execute(Engine *engine) override;
};

class LoadGlobalInstruction : public Instruction {
private:
  std::string id;

public:
  LoadGlobalInstruction(const std::string &id) : id(id) {}

  plsm_size_t execute(Engine *engine) override;
};

class CastInstruction : public Instruction {
private:
  Type *type;

public:
  CastInstruction(Type *type) : type(type) {}

  plsm_size_t execute(Engine *engine) override;
};

class CustomInstruction : public Instruction {
private:
  std::function<plsm_size_t(Engine *)> executeFunction;

public:
  CustomInstruction(const std::function<plsm_size_t(Engine *)> &executeFunction)
      : executeFunction(executeFunction) {}

  plsm_size_t execute(Engine *engine) override;
};

class AddInstruction : public Instruction {
public:
  plsm_size_t execute(Engine *engine) override;
};

class SubInstruction : public Instruction {
public:
  plsm_size_t execute(Engine *engine) override;
};

class MulInstruction : public Instruction {
public:
  plsm_size_t execute(Engine *engine) override;
};

class DivInstruction : public Instruction {
public:
  plsm_size_t execute(Engine *engine) override;
};

class ModInstruction : public Instruction {
public:
  plsm_size_t execute(Engine *engine) override;
};

class CallInstruction : public Instruction {
private:
  plsm_size_t argc;

public:
  CallInstruction(plsm_size_t argc) : argc(argc) {}

  plsm_size_t execute(Engine *engine) override;
};

class FunctionStartInstruction : public Instruction {
private:
  plsm_size_t argc;

public:
  FunctionStartInstruction(plsm_size_t argc) : argc(argc) {}

  plsm_size_t execute(Engine *engine) override;
};

class FunctionFinishInstruction : public Instruction {
public:
  plsm_size_t execute(Engine *) override { return 1; }

  bool isFunctionFinish() override { return true; }
};

class DefineGlobalInstruction : public Instruction {
private:
  std::string id;

public:
  DefineGlobalInstruction(const std::string &id) : id(id) {}

  plsm_size_t execute(Engine *engine) override;
};
